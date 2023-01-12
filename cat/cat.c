#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "cat.h"

struct cat_buf* read_cat(const char * file){
    int size = file_size(file);
    FILE * cat = fopen(file,"r");
    if(!cat) return NULL;
    struct cat_buf* cat_b = malloc(sizeof(struct cat_buf)+size*sizeof(char));
    cat_b->size = size;
    int c,i=0,col_num=0;
    while((c=fgetc(cat)) != EOF){
        cat_b->buf[i] = c;
        if(col_num == -1) goto no_get_col;
        if(c != '\n'){
            col_num++;
        }else{
            cat_b->col_num = col_num;
            col_num = -1;
        }
no_get_col:
        i++;
    }
    fclose(cat);
    return cat_b;
}
int file_size(const char* filename)
{
    struct stat statbuf;
    stat(filename,&statbuf);
    int size=statbuf.st_size; 
    return size;
}
