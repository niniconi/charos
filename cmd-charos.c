#include <stdlib.h>
#include <stdio.h>
#include "charos.h"
void quit(void){
    int i;
    if(wm != NULL){
        for(i = 0;i<wm->top;i++){
            free(wm->windows[i].bitmaps);
        }
        free(wm);
        INFO(__FUNCTION__,"free wm");
    }
    if(sm != NULL){
        int i,length=sm->top;
        for(i=0;i<=length;i++){
            free(sm->string_ptr[i]);
            INFO(__FUNCTION__,"\tfree str");
        }
        free(sm);
        INFO(__FUNCTION__,"free sm");
    }
    printf("\033[?25h");
    exit(EXIT_SUCCESS);
}
