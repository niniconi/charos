#include <stdlib.h>
#include <stdio.h>
#include "charos.h"

strManager *sm = NULL;

strManager* init_SM(void){
    sm = malloc(sizeof(strManager) + DEFAULT_STRING_MANAGER_SIZE * sizeof(struct string *));
    if(sm == NULL)
        return NULL;
    sm->top = -1;
    sm->length = DEFAULT_STRING_MANAGER_SIZE;
    return sm;
}
strManager* enlarge_sm(void){
    if(sm != NULL){
        /* printf("%s:size=%lu\n",__FUNCTION__,sizeof(strManager) + (sm->length + STRUBG_MANAGER_GRAIN_SIZE)*sizeof(struct string *)); */
        sm = realloc(sm,sizeof(strManager) + (sm->length + STRUBG_MANAGER_GRAIN_SIZE)*sizeof(struct string *));
        sm->length += STRUBG_MANAGER_GRAIN_SIZE;
        return sm;
    }
    return NULL;
}
int push_string(string *str){
    if(str == NULL)return 1;
    sm->top++;
    if(sm->top >= sm->length){
        sm = enlarge_sm();
    }
    sm->string_ptr[sm->top] = str;
    return 0;
}
string* new_string(const char *s,int size){
    string *ret = malloc(size+sizeof(string));
    if(ret == NULL)return NULL;
    ret->length = size;
    for(int i=0;i<size;i++){
        ret->s[i] = s[i];
    }
    push_string(ret);
    return ret;
}
string* new_string_n(int size){
    string *ret = malloc(size+sizeof(string));
    if(ret == NULL)return NULL;
    ret->length = size;
    push_string(ret);
    return ret;
}
string* new_string_z(const char*s){
    int size = 0;
    while(*s != '\0'){
        size++;
        s++;
    }
    s -= size;
    string *ret = malloc(size+1+sizeof(string));
    if(ret == NULL)return NULL;
    ret->length = size+1;
    for(int i=0;i<size+1;i++){
        ret->s[i] = s[i];
    }
    push_string(ret);
    return ret;
}
int read_properties(const char *path){
    FILE *properties = fopen(path,"r");
    return -1;
}

inline int isfillbitmaps(char *maps,int width,int x,int y){
    return *(maps + (y*width + x)/sizeof(char)) & (128 >> (y*width + x) % sizeof(char));
}
inline void fillbitmaps(char *maps,int width,int x,int y,int value){
    if(value)
        *(maps + (y*width + x)/sizeof(char)) |= 128 >> ((y*width + x) % sizeof(char));
    else
        *(maps + (y*width + x)/sizeof(char)) &= ~(128 >> ((y*width + x) % sizeof(char)));
}
inline void setbitmaps(char *master,char *slave,int master_xsize,int slave_xsize,int slave_ysize){
    int x,y;
    for (x=0 ; x<slave_xsize; x++) {
        for (y=0; y<slave_ysize; y++) {
            //TODO : 做一些优化
            *(master + (y*master_xsize + x)/sizeof(char)) |= *(slave + (y*slave_xsize + x)/sizeof(char));
        }
    }
}
