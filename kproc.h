#pragma once

#include <unistd.h>
#include <dirent.h>
#include "stdint.h"
#include <fcntl.h>
#include "ktypes.h"
#include "kio.h"
#include "kvec.h"

static inline pid_t proc_getpid(char* name){

  DIR *dir = opendir("/proc");

  struct dirent *entry;

  while((entry = readdir(dir)) != NULL){

      if(isDigit(entry->d_name[0])){

          char path[256];         
          _printin(path, sizeof(path), "/proc/%cs/cmdline", entry->d_name);

          int fd = open(path, O_RDONLY);
          if(fd != -1){

            char cmd[4096];
            usize bytes_read = read(fd, cmd, sizeof(cmd));
        
            if(bytes_read > 0){

                cmd[bytes_read] = '\0';

                usize backslash = 0;
                usize j = 0;
                while(j < bytes_read && cmd[j] != '\0'){

                        if(cmd[j] == '/'){

                            backslash = j + 1;
                            
                        }
                        j++;
                }
                char foundname[4096];
        
                usize i = 0;
                while(cmd[backslash] != '\0' && cmd[backslash] != '\n'){
                      foundname[i] = cmd[backslash];
                      i++;
                      backslash++;
                }
                foundname[i] = '\0';
        
                if(cmpChar(foundname, name)){
            
                    pid_t pid = ctol(entry->d_name);
                    close(fd);
                    closedir(dir);
                    return pid;
                }
            }
            close(fd);
          }
      }
  }
  closedir(dir);
  return -1;
}
static inline ssize_t proc_readmem(pid_t pid, void* buffer, usize size, uintptr_t address){

        char path[64];
        _printin(path, sizeof(path), "/proc/%d/mem", pid);
        
        int fd = open(path, O_RDONLY);
        if(fd == -1){
            _print("Error in proc_readmem: The pid is non-existent\n");
            return -2;
        }
        ssize_t bytes_read = pread(fd, buffer, size, address);

        close(fd);
        return bytes_read;

}
static inline ssize_t proc_writemem(pid_t pid, void* buffer, usize size, uintptr_t address){

    char path[64];
    _printin(path, sizeof(path), "/proc/%d/mem", pid);

    int fd = open(path, O_WRONLY);
    if(fd == -1){
        _print("Error in proc_readmem: The pid is non-existent\n");
        return -2;
    }
    ssize_t bytes_read = pwrite(fd, buffer, size, address);

    close(fd);
    return bytes_read;
}
static inline void proc_scanmem(pid_t pid,alist *list, int target_value){

    char path[64];
    _printin(path, sizeof(path), "/proc/%d/mem", pid);
    int fd = open(path, O_RDONLY);

    char map_path[64];
    _printin(map_path, sizeof(map_path), "/proc/%d/maps", pid);

    FILE *file = fopen(map_path, "r");

    if(!file){
        _print("Error in proc_scanmem: Couldnt open file \n");
        close(fd);
        return;
    }
    char line[512]; 
    
    while(fgets(line, sizeof(line), file)){

        uintptr_t start, end;
        char perms[5];

        if(sscanf(line, "%lx-%lx %4s", &start, &end, perms) == 3){

            if(perms[0] != 'r' || perms[1] != 'w' || perms[3] != 'p')continue;

            uintptr_t current = start;
            
            usize buffer_size = 1024 * 8;

            if((current + sizeof(target_value)) < end && current + buffer_size > end){

                buffer_size = end - current;
            }
            
            if(buffer_size > 1024 * 8)buffer_size = 1024 * 8;
            buffer_size = align(buffer_size, 8);
            
            int buffer[1024 * 8/ sizeof(target_value)];
            
            while(current + buffer_size <= end){

                if(buffer_size > 1024 * 8)buffer_size = 1024 * 8;
                
                if(pread(fd, buffer, buffer_size, current) == buffer_size){

                    int digit_amount = buffer_size / sizeof(target_value);

                    for(usize i = 0; i < digit_amount; i++){

                        if(buffer[i] == target_value){

                            unsigned long found_address = current + (i * sizeof(target_value));
                        
                            push_alist(list, found_address);
                        }
                    }
                }
                
                current = current + buffer_size;
                if((current + sizeof(target_value)) < end && current + buffer_size > end){

                    buffer_size = end - current;
                    buffer_size = align(buffer_size, 8);
                   
                }
            }
        }
    }
    close(fd);
    return;
}
static inline void proc_rescanmem(pid_t pid, alist *list, int target_value){

    if(list->length == 0){
        _print("Error in proc_rescanmem: The alist is empty, nothing to rescan for\n");
        return;
    }
    char path[64];
    _printin(path, sizeof(path), "/proc/%d/mem", pid);
    int fd = open(path, O_RDONLY);

    usize write = 0;
    usize i = 0;
    int buffer;
    
    while(i < list->length){

        if(pread(fd, &buffer, sizeof(buffer), list->data[i]) == sizeof(buffer)){

            if(buffer == target_value){
                list->data[write] = list->data[i];
                write++;
            }
        }
        i++;
    }
    list->length = write;
    close(fd);
}

