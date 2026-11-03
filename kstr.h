#pragma once


#include <stdio.h>
#include "ktypes.h"
#include "kio.h"
#include "kmem.h"
#include "kvec.h"


typedef struct string{

    char* data;
    usize length;
    usize size;


}string;
static inline usize len(char* text){
    usize i = 0;
    while(text[i] != '\0'){
        i++;
    }
    return i;
}
static inline void _freeString(string* text){

    if(text == NULL || text->data == NULL){
        _print("Error in _freeString: The string is NULL\n");
        return;
    }
    _free(text->data);
    string empty = {NULL, 0, 0};
    *text = empty;
    return;

}
static inline bool isDigit(char number){
    if(number >= '0' && number <= '9'){
        return true;
    }
    else{
        return false;
    }
}
static inline usize findChar(string* text, usize start_of_scan, char character){

    if(text == NULL || text->data == NULL){
        _print("Error in findChar: There is a NULL pointer\n");
        return (usize) - 1;
    }
    if(start_of_scan >= text->length){
        _print("Error in findChars: Index is out of bounds\n");
        return (usize) - 1;
    }
    for(usize i = start_of_scan; i < text->length; i++){
        if(text->data[i] == character){
            return i;
        }
    }
    return (usize) - 1;
    
}
static inline long ctol(char* text){

    long result = 0;

    for(long i = 0; i < len(text); i++){
        if(!isDigit(text[i])){
            _print("Error in stol: The text isn't a number\n");
            return 0;
        }
        result = (result * 10) + text[i] -'0';
    }
    return result;
}
static inline long ctoi(char* text){

    int result = 0;

    for(int i = 0; i < len(text); i++){
        if(!isDigit(text[i])){
            _print("Error in stol: The text isn't a number\n");
            return 0;
        }
        result = (result * 10) + text[i] -'0';
    }
    return result;
}
static inline int stoi(string* text){

      return ctoi(text->data);
}
static inline long stol(string* text){

    return ctol(text->data);
}
static inline void expandStr(string* text, usize inc_amount){
    if(inc_amount == 0){
        _print("Error in expandStr: Invalid size\n");
        return;
    }
    text->size += inc_amount;

    char* temp = (char*)_realloc(text->data, text->size);
    if(temp == NULL){
        _print("Error in expandStr: Couldn't _realloc temp\n");
        return;
    }
    text->data = temp;
    temp = NULL;
    return;

}

static inline string newString(char* text){

    usize length = len(text);
    usize size = length + 1;
    
    if(length == 0){
        size = 4;
    }


    char* data = (char*)_malloc(size);
    for(usize i = 0; i < size; i++){
        data[i] = text[i];
    }
    data[length] = '\0';
    string string = {data, length, size};
    return string;

}

static inline void addchar(string* text, usize index, char character){

      
    if(text->length + 1 >= text->size || text->size < sizeof(character)){

        text->size *= 2;
        char* temp = (char*)_realloc(text->data, text->size);

        if(temp == NULL){
            _print("Error in addchar: Couldnt _realloc temp\n"); 
            return;
        }

        text->data = temp;
    }
    if(index >= text->size){


        
        usize i = text->length;


        text->size = index + 2;
        text->length = index + 1;

        char* temp = (char*)_realloc(text->data, text->size);

        if(temp == NULL){
            _print("Error in addchar: Failed to _realloc temp in index\n");
            return;
        }
        
        text->data = temp;

        text->data[text->length] = '\0';
        temp = NULL;

        while(i < text->length){
            text->data[i] = ' ';
            i++;
        }  
        
    }
    
    else if(index >= text->length){

        while(text->length < index){
            text->data[text->length] = ' ';
            text->length++;
        }
        text->length++;
        text->data[text->length] = '\0';


    }
    text->data[index] = character;
    
    
}

static inline void changeStr(string* text, char* newtext){
    _freeString(text);
    *text = newString(newtext);
    return;
}

static inline void changeCase(char* character, char operation){

    if(character == NULL){
        _print("Error in changeCase: There is a NULL pointer \n");
        return;
    }
    else if(operation == '+' && *character >= 'a' && *character <= 'z'){
        *character-= 32;
    }
    else if(operation == '-' && *character >= 'A' && *character <= 'Z'){
        *character+= 32;
    }
    return;

}

static inline void toUpperChar(char* character){
    if(character == NULL){
        _print("Error in toUpperChar: There is a NULL pointer\n");
        return;
    }
    changeCase(character, '+');
    return;
}

static inline void toLowerChar(char* character){
    if(character == NULL){
        _print("Error in toLowerChar: There is a NULL pointer\n");
        return;
    }
    changeCase(character, '-');
    return;
}

static inline void toUpperStr(string* text){
    if(text == NULL || text->data == NULL){
        _print("Error in toUpperStr: There is a NULL pointer\n");
        return;
    }

    for(usize i = 0; i < text->length; i++){
        changeCase(&text->data[i], '+');
    }
    return;
}

static inline void toLowerStr(string* text){
    if(text == NULL || text->data == NULL){
        _print("Error in toLowerStr: There is a NULL pointer\n");
        return;
    }

    for(usize i = 0; i < text->length; i++){
        changeCase(&text->data[i], '-');
    }
    return;
}

static inline void updateStr(string* text){
    if(text == NULL || text->data == NULL){
        _print("Error in updateStr: The string is NULL\n");
        return;
    }
    text->length = len(text->data);

}
static inline void rmvcharC(char* text, usize index){
    
    if(index >= len(text)){
        _print("Error in rmvcharC: Index is out of bounds %llu\n", (usize)index);
        return;
    }
    while(text[index] != '\0'){
        text[index] = text[index + 1];
        index++;
    }
}
static inline void rmvchar(string* text, usize index){
    
    rmvcharC(text->data, index);
    updateStr(text);
}

static inline void resizeStr(string* text, usize newsize){

    if(newsize == text->size){
        return;
    }

    if(newsize < text->size){

        text->data[newsize] = '\0';
        text->size = newsize + 1;
        text->length = newsize;
        char* temp = (char*)_realloc(text->data, text->size);

        if(temp == NULL && text->size > 0){
            _print("Error in resizeStr: Couldn't _realloc temp\n");
            return;
        }
        
        text->data = temp;
        temp = NULL;
        
    }
    else{

        text->size = newsize + 2;
        text->length = newsize + 1;
        char* temp = (char*)_realloc(text->data, text->size);

        if(temp == NULL){
            _print("Error in resizeStr: Failed to _realloc in second temp\n");
            return;
        }

        text->data = temp;
        temp = NULL;
        return;

    }

}
static inline bool cmpChar(char* first, char* second){
    if(first == NULL || second == NULL){
        _print("Error in cmpChar: There is a NULL pointer\n");
        return false;
    }
    if(len(first) != len(second)){
        return false;
    }

    for(usize i = 0; i < len(first); i++){

        if(first[i] != second[i]){
            return false;
        }

    }
    return true;
}
static inline bool cmpStr(string* first, string* second){
    
    if(first == NULL  || second == NULL ){
        _print("Error in cmpStr: There is a NULL pointer\n");
        return false;
    }

    return cmpChar(first->data, second->data);
}
static inline void copyStr(string* dest, string* src){
    
    if(dest == NULL || dest->data == NULL || src == NULL || src->data == NULL){
        _print("Error in copyStr: There is a NULL pointer\n");
        return;
    }
    if(dest->data == src->data){
        return;
    }
    _freeString(dest);
    *dest = newString(src->data);
    
}
static inline void shrinkStr(string* text){
    if(text->length + 1 == text->size){
        return;
    }
    
    text->size = text->length + 1;
    char* temp = (char*)_realloc(text->data, text->size);
    if(temp == NULL){
        _print("Error in shrinkStr: Couldn't _realloc temp\n");
        return;
    }
    text->data = temp;
    temp = NULL;
    return;
}
static inline void rmvExactchar(string* text, char character, usize amount_of_chars){

    usize i = 0;
    usize deletedchars = 0;
    while(text->data[i] != '\0' && deletedchars < amount_of_chars){

        if(text->data[i] == character){
            rmvchar(&(*text), i);
            deletedchars++;
            continue;
        }
        i++;
    }


}
static inline void appendStr(string* dest, string* src){

    usize neededSpace = src->length + dest->length + 1;
    if(dest->size < neededSpace){

        dest->size = neededSpace;
        char* temp = (char*)_realloc(dest->data, dest->size);
        if(temp == NULL){
            _print("Error in combining strings: couldnt _realloc temp\n");
            return;
        }
        dest->data = temp;

   }
   usize j = 0;
   usize newlength = dest->length;

   while(j < src->length){
        
        dest->data[dest->length + j] = src->data[j];

        j++;
   }
   newlength += src->length;
   dest->length = newlength;
   dest->data[dest->length] = '\0';
    
}
static inline string combineStr(string* first, string* second){
    string result;

    result.size = first->length + second->length + 1;
    result.length = 0;
    result.data = (char*)_malloc(result.size);
    
    if(result.data == NULL){
        _print("Error in combineStr: Failed to _malloc result.data\n");
        string empty = {NULL, 0, 0};
        return empty;
    }
    result.data[0] = '\0';
    appendStr(&result, first);
    appendStr(&result, second);
    return result;

}

static inline void readfile(string* dest, FILE* file){

    fseek(file, 0,  SEEK_END);
    usize size = ftell(file);
    rewind(file);

    resizeStr(dest, size);

    usize size_read = fread(dest->data, 1, size, file);


}
static inline bool isin(char character, char* text){

    for(usize i = 0; i < len(text); i++){
        if(text[i] == character){
            return true;
        }
    }
    return false;
}
static inline string ustos(usize number){

    if(number == 0){
        string text = newString("0");
        return text;
    }
    number = (ssize)number;
    
    ssize temp = number;
    ssize len = 0;

    while(temp > 0){
        len++;
        temp /= 10;
    }

    

    string text = newString("");

    for(ssize i = len - 1; i >= 0; i--){

        ssize singledigit = number % 10;
        addchar(&text, i, singledigit + '0');
        number /= 10;
    }
    
    return text;

}
static inline string itos(int number){

    if(number == 0){
        string text = newString("0");
        return text;
    }
    int temp = number;
    int len = 0;

    while(temp > 0){
        len++;
        temp /= 10;
    }

    

    string text = newString("");

    for(int i = len - 1; i >= 0; i--){

        int singledigit = number % 10;
        addchar(&text, i, singledigit + '0');
        number /= 10;
    }
    
    return text;

}
static inline void fill_chars(char* src, char* dest){

    for(usize i = 0; i <= len(dest); i++){
        src[i] = dest[i];
    }
}
