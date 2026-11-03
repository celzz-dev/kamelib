#pragma once

#include <unistd.h>
#include <stdarg.h>
#include "ktypes.h"

static inline void _print(char* text, ...);
#include "kvec.h"
#include "kstr.h"


static inline void _print(char* text, ...){

  va_list args;
  va_start(args, text);

  usize i = 0;

  while(text[i] != '\0'){

    if(text[i] == '%'){

      if(text[i + 1] == 'd'){

        int number = va_arg(args, int);
        string numtoprint = itos(number);
        
        write(STDOUT_FILENO, numtoprint.data, numtoprint.length);
        _freeString(&numtoprint);
        
        i+=2;
        continue;
      }
      if(text[i + 1] == 's'){

        string text = va_arg(args, string);

        write(STDOUT_FILENO, text.data, text.length);

        i+=2;
        continue;
      }
      if(text[i + 1] == 'v'){

        vector vec = va_arg(args, vector);

        for(usize i = 0; i < vec.length; i++){
            write(STDOUT_FILENO, vec.items[i].data, vec.items[i].length);
            write(STDOUT_FILENO, " ", 1);
        }

        i+=2;
        continue;
      }
      if(text[i + 1] == 'u' && text[i + 2] == 's'){

        usize number = va_arg(args, usize);
        string numtoprint = ustos(number);

        write(STDOUT_FILENO, numtoprint.data, numtoprint.length);
        _freeString(&numtoprint);

        i+=3;
        continue;
      }
      if(text[i + 1] == 'c' && text[i + 2] == 's'){

        char* word = va_arg(args, char*);
        write(STDOUT_FILENO, word, len(word));

        i+=3;
        continue;
      }
      if(text[i + 1] == 'c'){

        char character = va_arg(args, int);
        write(STDOUT_FILENO, &character, 1);
        i+=2;
        continue;
      }
        }
    write(STDOUT_FILENO, &text[i], 1);
    i++;
  }
  va_end(args);
  
}
static inline void _printin(char* dest, usize size, char* src, ...){

    va_list args;
    va_start(args, src);

    usize i = 0;
    usize location = 0;
    
    while(location < size && src[i] != '\0'){


        if(src[i] == '%' && src[i + 1] == 'd'){

              int number = va_arg(args, int);
              string text = itos(number);

              usize j = 0;
              while(location < size && j < text.length){
                
                    dest[location] = text.data[j];
                    location++;
                    j++;
              }
              _freeString(&text);
              i+=2;
              continue;
        }
        if(src[i] == '%' && src[i + 1] == 's'){

              string text = va_arg(args, string);
              usize j = 0;
              
              while(location < size && j < text.length){

                    dest[location] = text.data[j];
                    location++;
              } 
              i+=3;
              continue;
        }
        if(src[i] == '%' && src[i + 1] == 'v'){

              vector vec = va_arg(args, vector);

              usize v = 0;
              usize j = 0;

              while(location < size && v < vec.length){              

                  while(location < size && j < vec.items[v].length){
                  
                      dest[location] = vec.items[v].data[j];
                      location++;
                      j++;
                  }
                  dest[location] = ' ';
                  location++;
                  v++;
                  j = 0;
              }
              i+=3;
              continue;
        }
        if(src[i] == '%' && src[i + 1] == 'c' && src[i + 2] == 's'){

              char* text = va_arg(args, char*);
              for(usize j = 0; j < len(text); j++){

                    dest[location] = text[j];
                    location++;
              }
              i+=3;
              continue;
        }
        if(src[i] == '%' && src[i + 1] == 'c'){

              char character = va_arg(args, int);
              dest[location] = character;
              location++;
              i+=2;
              continue;              
        }
        dest[location] = src[i];
        location++;
        i++;
    }
    dest[location] = '\0';
} 

