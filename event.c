#include <termio.h>
#include <stdio.h>
#include <stdlib.h>
#include "charos.h"


//the buffer of the mouse and key
EKIB key_buf;

EMIB mouse_buf;

event_mgr * em=NULL;

char get_key(){
    if(key_buf.null_count < EVENT_KEY_BUF_SIZE){
        if(key_buf.r<EVENT_KEY_BUF_SIZE){
            char c = key_buf.buf[key_buf.r];
            key_buf.r++;
            key_buf.null_count++;
            return c;
        }else{
            char c = key_buf.buf[key_buf.r];
            key_buf.r = 0;
            key_buf.null_count++;
            return c;
        }
    }
    return KEY_NULL;
}

int put_key(char key){
    if(key_buf.null_count > 0){
        if(key_buf.w<EVENT_KEY_BUF_SIZE){
            key_buf.buf[key_buf.w] = key;
            key_buf.w++;
            key_buf.null_count--;
            return KEY_PUT_SUCCESS;
        }else{
            if(key_buf.w > EVENT_KEY_BUF_SIZE)
                key_buf.w = EVENT_KEY_BUF_SIZE;
            key_buf.buf[key_buf.w] = key;
            key_buf.w = 0;
            key_buf.null_count--;
            return KEY_PUT_SUCCESS;
        }
    }
    return KEY_PUT_FAILED;
}

event_mgr * init_event_manager(){
    event_mgr *mgr = malloc(sizeof(event_mgr) + EVENT_MGR_KEY_EVENT_COUNT*sizeof(struct key_event));
    if(mgr == NULL)return NULL;
    mgr->key_event_count=EVENT_MGR_KEY_EVENT_COUNT;
    mgr->key_event_top=-1;
    em = mgr;
    return mgr;
}
int register_key(char key,void*function,void *param){
    if(em->key_event_top >= em->key_event_count)return 1;
    for(int i=0;i<=em->key_event_top;i++){
        if(em->events[i].key == key)return 1;
    }
    em->key_event_top++;
    em->events[em->key_event_top].key = key;
    em->events[em->key_event_top].function = function;
    em->events[em->key_event_top].param = param;
    return 0;
}

void init_key_buf(){
    key_buf.r=0;
    key_buf.w=0;
    key_buf.null_count=EVENT_KEY_BUF_SIZE;
}
void init_mouse_buf(){
    mouse_buf.r=0;
    mouse_buf.w=0;
    mouse_buf.null_count=EVENT_MOUSE_BUF_SIZE;
}
