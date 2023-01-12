#pragma once
struct cat_buf{
    int size;
    int col_num;
    char buf[0];
};
struct cat_buf* read_cat(const char * file);
int file_size(const char * file);
