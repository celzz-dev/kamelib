#pragma once
#include <stdio.h>
#include "ktypes.h"
#include "kmem.h"
#include "kstr.h"


typedef struct{
    string* items;
    usize length;
    usize size; 
}vector;

typedef struct{

    unsigned long* data;
    usize length;
    usize size;
            
}alist;


static inline vector split_by(string* text, char character);
static inline void push_bulk(vector* vec, char* text, char seperator);


static inline alist init_alist(usize size){
    
    unsigned long* temp = (unsigned long*)_malloc(size * sizeof(unsigned long));
    
    alist vect = {temp, 0, size};
    return vect;
}
static inline void push_alist(alist* list, unsigned long item){

    if(list->length >= list->size){

        usize new_size = list->size * 2;
        unsigned long* temp = (unsigned long*)_realloc(list->data, new_size * sizeof(unsigned long));

        if(temp == NULL){
            _print("Error in push_alist: Couldn't _realloc\n");
            return;
        }
        list->size = new_size;
        list->data = temp;

    }
    list->data[list->length] = item;
    
    list->length++;

}

static inline void rmv_alist(alist* list, usize index){

    
    while(index + 1 < list->length){

        list->data[index] = list->data[(index + 1) ];
        index++;
    }
    list->length--;

}

static inline void resize_alist(alist* list, usize size){

    if(list->size == size){
        return;
    }
    unsigned long* temp = (unsigned long*)_realloc(list->data, size);
    if(temp == NULL){
        _print("Error in resize_alist: Failed to _realloc temp\n");
        return;
    }
    list->data = temp;
    if(list->size > size && list->length > size){
        list->length = size;
    }
    list->size = size;
}

static inline vector newVector(usize inital_size){

    vector vec;
    vec.size = inital_size;
    vec.length = 0;
    vec.items = (string*)_malloc(vec.size * sizeof(string));
    
    if(vec.items == NULL){
        printf("Error in newVector: Failed to _malloc items\n");
    }
    return vec;
}

static inline void resizeVec(vector* vec, usize new_size){

    if(vec == NULL || vec->items == NULL){
        printf("Error in resizeVec: There is a NULL pointer\n");
        return;
    }
    if(new_size < vec->length){
        for(usize i = new_size; i < vec->length; i++){
            _freeString(&vec->items[i]);
        }
        vec->length = new_size;
    }
    vec->size = new_size;
    string* temp = (string*)_realloc(vec->items, vec->size * sizeof(string));
    if(temp == NULL){
        printf("Error in resizeVec: Couldn't _realloc temp\n");
        return;
    }
    vec->items = temp;
    return;

}
static inline void expandVec(vector* vec, usize size){
    
    if(vec == NULL || vec->items == NULL){
        printf("Error in expandVec: There is a NULL pointer\n");
        return;
    }
    usize actualsize = size + vec->size;
    resizeVec(vec, actualsize);
    return;
}
static inline void shrinkVec(vector* vec){

    if (vec == NULL || vec->items == NULL){
        printf("Error in shrinkVec: There is a NULL pointer\n");
        return;
    }
    if(vec->size == vec->length){
        return;
    }
    if(vec->length == 0){
        vec->size = 1;
        resizeVec(vec, vec->size);
        return;
    }

    resizeVec(vec, vec->length);
    return;
}
static inline void _freeVector(vector* vec){

    if(vec == NULL || vec->items == NULL){
        printf("Error in _freeVector: Vector is NULL\n");
        return;
    }

    for(usize i = 0; i < vec->length; i++){
        _freeString(&vec->items[i]);
    }

    _free(vec->items);

    vec->items = NULL;
    vec->length = 0;
    vec->size = 0;
    
}
static inline void cpush_back(vector* dest, char* src){

    if(dest == NULL || dest->items == NULL){
        printf("Error in cpush_back: Something is asigned NULL\n");
        return;
    }
    string newsrc = newString(src);
    if(dest->length >= dest->size){

        dest->size = (dest->size == 0) ? 4 : dest->size * 2;

        string* temp = (string*)_realloc(dest->items, dest->size * sizeof(string));

        if(temp == NULL){
            printf("Error in cpush_back: Couldn't _realloc temp\n");
        }

        dest->items = temp;
        temp = NULL;

    }
    dest->items[dest->length] = newsrc;
    dest->length += 1;
}
static inline bool isinVec(vector* vec, string* text, usize* index){

    for(usize i = 0; i < vec->length; i++){
        if(cmpStr(&vec->items[i], text)){
            *index = i;
            return true;
        }
    }
    *index = (usize) - 1;
    return false;
    

}
static inline void push_back(vector* dest, string *src){

    if(src->data == NULL){
        printf("Error in push_back: Something is asigned NULL\n");
        return;
    }
    cpush_back(dest, src->data);
}

static inline string pop_back(vector* vec){

    if(vec == NULL || vec->items == NULL){
        printf("Error in pop_back: A pointer is NULL\n");
        string empty = {"", 0, 0};
        return empty;
    }

    
    vec->length--;
    string popped_string = newString(vec->items[vec->length].data);
    _freeString(&vec->items[vec->length]);
    
    return popped_string;
}

static inline void insert_at(vector* vec, string* text, usize index){


    if(vec == NULL || vec->items == NULL || text == NULL || text->data == NULL){
        printf("Error in insert_at: There is a NULL pointer");
        return;
    }
    if(index > vec->length){
        printf("Error in insert_at: Index is out of bounds\n");
        return;
    }
    if(vec->length >= vec->size){

        vec->size = (vec->size == 0) ? 4: vec->size * 2;
        string* temp = (string*)_realloc(vec->items, vec->size * sizeof(string));
        if(temp == NULL){
            printf("Error in insert_at: Couldn't _realloc temp\n");
            return;
        }
        vec->items = temp;
        temp = NULL;
    }

    for(usize i = vec->length; i > index; i--){
        vec->items[i] = vec->items[i-1];
    }
    string newtext = newString(text->data);
    vec->items[index] = newtext;
    vec->length++;
    return;
}

static inline void remove_at(vector* vec, usize index){

    if(vec == NULL || vec->items == NULL){
        printf("Error in remove_at: Vector is NULL\n");
        return;
    }
    if(index > vec->length){
        printf("Error in remove_at: Index is out of bounds\n");
        return;
    }
    if(index == vec->length){
        pop_back(vec);
        return;
    }
    _freeString(&vec->items[index]);
    usize i = index;
    while(i < (vec->length - 1)){
        vec->items[i] = vec->items[i + 1];
        i++;
    }
    vec->length--;

    string empty = {NULL, 0 ,0};
    vec->items[vec->length] = empty;
}

static inline vector split_by(string* text, char character){

    if(text == NULL || text->data == NULL){
        printf("Error in split: There is a NULL pointer\n");
        vector empty = {NULL, 0, 0};
        return empty;
    }
    if(text->length == 0 || text->size == 0){
        vector halfempty = newVector(4);
        return halfempty;
    }

    usize marker = 0;
    usize i = findChar(text, 0, character);

    if(i == (usize) - 1){
        vector one_only = newVector(4);
        push_back(&one_only, text);
        return one_only;
    }
    vector vec = newVector(4);

    while(i != (usize) - 1){
        
        
        string newtext = newString("");

        usize dest = 0;

        for(usize j = marker; j < i; j++){
            
            addchar(&newtext, dest, text->data[j]);
            dest++;
        }
        
        push_back(&vec, &newtext);
        _freeString(&newtext);
        
        marker = i + 1;
        i = findChar(text, marker, character);

    }
    if(marker < text->length){
        
        string newtext = newString("");

        usize dest = 0;

        for(usize j = marker; j < text->length; j++){
            addchar(&newtext, dest, text->data[j]);
            dest++;
        }
        
        push_back(&vec, &newtext);
        _freeString(&newtext);

    }

    return vec;
}

static inline void push_bulk(vector* vec, char* text, char seperator){

    if(vec == NULL || vec->items == NULL || text == NULL){
        printf("Error in push_bulk: There is a NULL pointer\n");
        return;
    }

    string rawtext = newString(text);
    vector temp = split_by(&rawtext, seperator);
    _freeString(&rawtext);

    for(usize i = 0; i < temp.length; i++){

        push_back(vec, &temp.items[i]);

    }
    _freeVector(&temp);
    return;
    
} 

static inline vector split(string* text){

    vector vec = split_by(text, ' ');

    return vec;

}

static inline void showVec(vector* vec){

    if(vec == NULL || vec->items == NULL){
        printf("Error in showVec: There is a NULL pointer\n");
        return;
    }
    for(usize i = 0; i < vec->length; i++){
        printf("%s ", vec->items[i].data);
    }
    printf("\n");
}
static inline string vtos(vector *vec){

    string fullstring = newString("");

    for(usize i = 0; i < vec->length; i++){
        
        appendStr(&fullstring, &vec->items[i]);
        
    }
    return fullstring;
}
