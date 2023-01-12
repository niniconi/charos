#include <stdlib.h>
#include <stdio.h>
#include "charos.h"
void quit(void){
    if(wm != NULL){
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
    exit(EXIT_SUCCESS);
}
