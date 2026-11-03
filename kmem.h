#pragma once

#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include "ktypes.h"


#define page_size 4096
#define mem_alignment 8

typedef struct block_header{

    unsigned long long size;
    int is_free;
    struct block_header* next;
    struct block_header* prev;

}block_header;

block_header* head_of_blocks = NULL;
static inline void merge_block(block_header *current);
static inline usize align(usize usizeo_align, usize alignment_size);

static inline bool are_close(block_header* first, block_header* second){

    if(first == NULL || second == NULL){
        return false;
    }
    if((char*)first - (char*)second != second->size + sizeof(block_header) ){
        return false;
    }
    return true;
}
static inline void* get_os_mem(usize size){

    usize neededsize = size + sizeof(block_header);
    usize actualsize = align(neededsize, page_size);
    
    void* raw_memory = mmap(NULL, actualsize, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    
    if(raw_memory == MAP_FAILED){
        printf("Error in get_os_mem: Couldn't map memory \n");
        return NULL;
    }
    block_header* block = (block_header*)raw_memory;

    block->size = actualsize - sizeof(block_header);
    block->is_free = 0;
    block->next = NULL;
    block->prev = NULL;

    return block;
} 

static inline void* _malloc(usize size){
    
    size = align(size, mem_alignment);


    block_header* current = head_of_blocks;
    block_header* prev = NULL;

    while(current != NULL){

        if(current->is_free && current->size >= size){

            if(current->size >= size + sizeof(block_header) + 8){

                block_header* new_block = (block_header*)((char*)(current + 1) + size);

                // if(new_block >= current->next){
                //     current->is_free = 0;
                //     return (void*)(current + 1);
                // }
                new_block->size = current->size - size - sizeof(block_header);
                new_block->is_free = 1;
                new_block->next = current->next;
                new_block->prev = current;

                if(new_block->next != NULL){
                    new_block->next->prev = new_block;
                }
                current->size = size;
                current->next = new_block;
                
            }
            current->is_free = 0;
            return (void*)(current + 1);
        }

        prev = current;

        current = current->next;
    }
    block_header* new_block = (block_header*)get_os_mem(size);

    
    if(new_block == NULL){
        printf("Error in _malloc: get_os_mem failed \n");
        return NULL;
    }
    if(new_block->size >= size + sizeof(block_header) + 8){

        block_header* newer_block = (block_header*)((char*)(new_block + 1) + size);
        
        newer_block->size = new_block->size - size - sizeof(block_header);
        newer_block->is_free = 1;
        newer_block->next = new_block->next;

        if(newer_block->next != NULL){
            newer_block->next->prev = newer_block;
        }

        newer_block->prev = new_block;

        new_block->size = size;
        new_block->next = newer_block;
        

    }
    if(head_of_blocks == NULL){
        head_of_blocks = new_block;
    }
    else{
        prev->next = new_block;
        new_block->prev = prev;
    }
    return (void*)(new_block + 1); 
     
}
static inline void _free(void* address){

    if(address == NULL){
        printf("Error in _free: The address is NULL \n");
        return;
    }
    block_header* block = (block_header*)address - 1;
    block->is_free = 1;

    merge_block(block);

   
}
static inline void merge_block(block_header* current){

        if(current == NULL){return;}

        while(current->next != NULL && current->next->is_free && are_close(current->next, current)){

            current->size = current->size + current->next->size + sizeof(block_header);
            current->is_free = 1;

            current->next = current->next->next;

            if(current->next != NULL){
                current->next->prev = current;
            }
        }
        while(current->prev != NULL && current->prev->is_free && are_close(current, current->prev)){

            block_header* previous = current->prev;

            previous->size = previous->size + current->size + sizeof(block_header);
            previous->is_free = 1;

            previous->next = current->next;

            if(previous->next != NULL){
                previous->next->prev = previous;
            }
            current = previous;
        }
    
}

static inline void* _realloc(void* address, usize new_size){

    if(address == NULL){
        address = _malloc(new_size);
        return address;
    }
    if(new_size == 0){
        _free(address);
        return NULL;
    }
    new_size = align(new_size, mem_alignment);
    

    block_header* header = (block_header*)address - 1;

   
    if(header->size < new_size && header->next != NULL && header->next->is_free && new_size <= (header->size + header->next->size + sizeof(block_header)) && are_close(header->next, header)){

        header->size = header->size + header->next->size + sizeof(block_header);
        header->is_free = 0;
        header->next = header->next->next;

        if(header->next != NULL){
            header->next->prev = header;
        }
        if(header->size >= new_size + sizeof(block_header) + 8){


            block_header* second_header = (block_header*)((char*)address + new_size);
                        
            second_header->size = header->size - new_size - sizeof(block_header);
            second_header->is_free = 1;
            second_header->next = header->next;
            second_header->prev = header;

            if(second_header->next != NULL){

                second_header->next->prev = second_header;
            }
            header->next = second_header;
            header->size = new_size;
        }
        return address;         
    }
    if(header->size < new_size && header->prev != NULL && header->prev->is_free && new_size <= (header->prev->size + header->size + sizeof(block_header)) && are_close(header, header->prev)){

        block_header* first_header = header->prev;

        first_header->size = header->size + header->prev->size + sizeof(block_header);
        first_header->is_free = 0;
        first_header->next = header->next;
        first_header->prev = header->prev->prev;

        if(first_header->prev != NULL){
            first_header->prev->next = first_header;
        }
        if(first_header->next != NULL){
            first_header->next->prev = first_header;
        }
        if(first_header->size >= new_size + sizeof(block_header) + 8){

            block_header* second_header = (block_header*)((char*)(first_header + 1) + new_size);

            second_header->size = first_header->size - new_size - sizeof(block_header);
            second_header->is_free = 1;
            second_header->next = first_header->next;
            second_header->prev = first_header;

            first_header->next = second_header;
            first_header->size = new_size;
                            
            if(second_header->next != NULL){
                second_header->next->prev = second_header;
            }
                           
        }
        usize i = 0;
        char* first_block = (char*)(first_header + 1);
        char* usable_address = (char*)address;
        
        while(i < header->size){

            first_block[i] = usable_address[i];
            i++;                
        }
        return first_block;            

            
    }
    
     if(header->size < new_size && header->next != NULL && header->next->is_free &&
        header->prev != NULL && header->prev->is_free &&
        new_size <= (header->prev->size + header->size + header->next->size + sizeof(block_header) + sizeof(block_header)) &&
        are_close(header->next, header) && are_close(header, header->prev)){

        usize combined_size = header->prev->size + header->size + header->next->size + sizeof(block_header) + sizeof(block_header);

        header->prev->size = combined_size;
        header->prev->is_free = 0;
        header->prev->next = header->next->next;

        if(header->prev->next != NULL){

            header->prev->next->prev = header->prev;            
        }
        if(header->prev->size >= new_size + sizeof(block_header) + 8){


        
            block_header* second_header = (block_header*)((char*)(header->prev + 1) + new_size);

            second_header->size = header->prev->size - new_size - sizeof(block_header);
            second_header->is_free = 1;
            second_header->next = header->prev->next;
            second_header->prev = header->prev;

            header->prev->size = new_size;
            header->prev->next = second_header;
                        
            if(second_header->next != NULL){
                second_header->next->prev = second_header;
            }
        }
        char* first_block = (char*)(header->prev + 1);
        char* usable_address = (char*)address;

        usize i = 0;

        while(i < header->size){

            first_block[i] = usable_address[i];
            i++;
        }
        return first_block;
                  
    }
    if(new_size > header->size){

        char* new_memory = (char*)_malloc(new_size);
        char* usable_address = (char*)address;
        
        block_header* new_header = (block_header*)new_memory - 1;

        new_header->is_free = 0;
        usize i = 0;

        while(i < header->size){

            new_memory[i] = usable_address[i];
            i++;
        }

        
        header->is_free = 0;
        _free(address);
        
        return new_memory;
    }
    else if(new_size <= header->size){

        if(header->size >= new_size + sizeof(block_header) + 8){

            block_header* new_header = (block_header*)((char*)address + new_size);

            new_header->size = header->size - new_size - sizeof(block_header);
            new_header->is_free = 1;
            new_header->next = header->next;

            if(header->next != NULL){
                header->next->prev = new_header;
            }
            header->size = new_size;
            header->next = new_header;
            header->is_free = 0;
            new_header->prev = header;

            return address;
            
        }
        else{

            return address;
        }

        
    }
    return address;

    
}


static inline usize align(usize usizeo_align, usize alignment_size){

    return ((usizeo_align + (alignment_size - 1)) / alignment_size) * alignment_size;
}
static inline void debug_heap(){

    block_header* current = head_of_blocks;
    int count = 0;

    while(current != NULL){

        printf("%d Size: %llu Is_free: %d ", count, current->size, current->is_free);
        printf("\n");
        current = current->next;
        count++;
    }
}
