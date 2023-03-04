#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "winmgr.h"
#include "cat/cat.h"
#include "charos.h"

unsigned int width = 0;
unsigned int height = 0;

fps flush_f = 60;//fps
fps listen_f = 32;
fps time_f = 32;
fps exeute_f = 32;
fps flush_win_f = 32;

int type =
    WT_COTROL_BTN+
    WT_TITLE+
    WT_FRAME+
    WT_ACTIVE+
    WT_ICON;


int main(int argc,char **argv){
    int k;
    while((k=getopt(argc, argv,"vh")) != -1){
        switch (k){
            case 'v':
                printf("version 0.0.1\n");
                return 0;
            case 'h':
                printf("\twasd 上下左右移动\n");
                printf("\tn 新建窗口\n");
                printf("\tx 关闭窗口\n");
                printf("\thjkl 改变大小\n");
                printf("\tq 退出\n");
                return 0;
        }
    }
    init();
    return 0;
}

void init(void){
    //get screen size
    struct winsize size;
    ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
    width = size.ws_col;
    height = size.ws_row;
    //display startup logo
    struct cat_buf * cb = read_cat("./cat/charos3.cat");
    system("clear");
    printf("%s\n",cb->buf);
    printf("作者:niniconi\ngithub:https://github.com/niniconi\n....%ds",0);
    fflush(stdout);
    for(int i=0;i<STOP_ON_STARTUP;i++){
        printf("\b\b%ds",i);
        fflush(stdout);
        usleep(1000*1000);
    }
    putchar('\n');

    if(init_WM() != NULL)INFO(__FUNCTION__,"init wm success");
    if(init_SM() != NULL)INFO(__FUNCTION__,"init sm success");
    if(init_event_manager() != NULL)INFO(__FUNCTION__,"init event mgr success");

    win *desktop = malloc_window_with_property(wm,RED_BG,WHITE,BACKGROUND);
    desktop->x=0;
    desktop->y=0;
    desktop->width=width;
    desktop->height=height;
    desktop->type=0;
    default_wins[WS_DESKTOP] = desktop;

    win *task_bar = malloc_window_with_property(wm,WHITE_BG,BLACK,BACKGROUND);
    task_bar->x=0;
    task_bar->y=height-2;
    task_bar->width=width;
    task_bar->height=2;
    task_bar->type=0;
    default_wins[WS_TASKBAR] = task_bar;

    //init thread
    pthread_t render_t;
    pthread_t event_listen_t;
    pthread_t event_execute_t;
    pthread_t system_time_t;
    pthread_t flush_win_size_t;
    int render_t_stat         = pthread_create(&render_t       ,NULL,(void *)render_thread      ,&flush_f);
    int event_listen_t_stat   = pthread_create(&event_listen_t ,NULL,(void *)event_listener     ,&listen_f);
    int event_execute_t_stat  = pthread_create(&event_execute_t,NULL,(void *)event_execute      ,&exeute_f);
    int system_time_t_stat    = pthread_create(&system_time_t  ,NULL,(void *)system_time        ,&time_f);
    int flush_win_size_t_stat = pthread_create(&flush_win_size_t,NULL,(void *)flush_win_size    ,&flush_win_f);


    //register system event
    register_key('n',(void *)new_win,(void *)&type);
    register_key('a',(void *)win_move_left,NULL);
    register_key('s',(void *)win_move_down,NULL);
    register_key('d',(void *)win_move_right,NULL);
    register_key('w',(void *)win_move_up,NULL);
    register_key('q',(void *)quit,NULL);
    register_key('x',(void *)win_close,NULL);
    register_key('h',(void *)win_width_add,NULL);
    register_key('l',(void *)win_width_sub,NULL);
    register_key('j',(void *)win_height_sub,NULL);
    register_key('k',(void *)win_height_add,NULL);
    register_key('u',(void *)win_maximize,NULL);
    register_key('i',(void *)win_minimize,NULL);
    register_key('r',(void *)win_reduction,NULL);

    while(1){
        usleep(1000000000);
    }
}
int flush_win_size(fps *f){
    if(*f <= 0){
        pthread_detach(pthread_self());
        pthread_exit(NULL);
    }
    int us = FPS(*f);
    struct winsize size;
    while (1) {
        ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
        width = size.ws_col;
        height = size.ws_row;
        default_wins[WS_DESKTOP]->height=height;
        default_wins[WS_DESKTOP]->width=width;
        default_wins[WS_TASKBAR]->width=width;
        default_wins[WS_TASKBAR]->y=height-2;
        usleep(us);
    }
    return 0;
}

int event_listener(fps *f){
    if(*f <= 0){
        pthread_detach(pthread_self());
        pthread_exit(NULL);
    }
    int us = FPS(*f);
    init_key_buf();
    init_mouse_buf();
    init_key_listener();
    while(1){
        char c = key_listener_get_char();
        put_key(c);
        usleep(us);
    }
    return 0;
}
int event_execute(fps*f){
    if(*f <= 0){
        pthread_detach(pthread_self());
        pthread_exit(NULL);
    }
    int us = FPS(*f);
    while(1){
        char k = get_key();
        if(k != KEY_NULL){
            for(int i=0;i<=em->key_event_top;i++){
                struct key_event ke = em->events[i];
                if(k == ke.key)
                    ((void (*)(void *))ke.function)(ke.param);//exeute the function
            }
        }
        usleep(us);
    }
    return 0;
}
int render_thread(fps *f){
    if(*f <= 0){
        pthread_detach(pthread_self());
        pthread_exit(NULL);
    }
    printf("\033[?25l");
    int us = FPS(*f);
    while(1){
        /* system("clear"); */
        printf("\033[0;0H");
        draw_all_windows(wm);
        flush();
        usleep(us);
    }
    return 0;
}
int fillchar(int x,int y,char *c,char bgcolor,char color){
    if(x>=width || x<0)return 1;
    if(y>=height || y<0)return 1;
    SET_CHAR(framebuf->buf[x][y].c,c);
    framebuf->buf[x][y].color = color;
    framebuf->buf[x][y].background = bgcolor;
    return 0;
}
int fillbox(int x,int y,int width,int height,char *c,char bgcolor,char color){
    if(x >= width)return 1;
    if(x < 0)x=0;
    if(y >= height)return 1;
    if(y < 0)y=0;
    int endX=x+width;
    int endY=y+height;
    if(endX >= width)endX=width;
    if(endY >= height)endY=height;
    for(int j=y;j<endY;j++){
        for(int i=x;i<endX;i++){
            SET_CHAR(framebuf->buf[i][j].c,c);
            framebuf->buf[i][j].background = bgcolor;
            framebuf->buf[i][j].color = color;
        }
    }
    return 0;
}

void flush(void){
    int x,y;
    for(y=0;y<height;y++){
        for(x=0;x<width;x++){
            if(isfillbitmaps(framebuf->bitmaps, 4096, x, y)){
                printf("\033[%d;%dm%s\033[0m",
                    framebuf->buf[x][y].color,
                    framebuf->buf[x][y].background,
                    framebuf->buf[x][y].c);
                /* fillbitmaps(framebuf->bitmaps, 4096, x, y,0); */
            }
        }
    }
}
int system_time(fps *f){
    fps us = FPS(*f);
    time_t now;
    string *time_str = new_string_n(25);
    while(1){
        time(&now);
        if(now != (time_t)(-1)){
           char *str = asctime(gmtime(&now));
           int i=0;
           while(*str != '\0'){
               time_str->s[i] = *str;
               i++;
               str++;
           }
           if(time_str != NULL && wm->top >= 1){
               win *w = &(wm->windows[1]);
               win_draw_string(w,time_str,w->width-26,w->height-2,25,1,1,WHITE_BG,BLACK);
           }
        }
        usleep(us);
    }
}
