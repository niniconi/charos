#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "winmgr.h"
#include "cat/cat.h"
#include "charos.h"

char BACKGROUND         [4] = " ";
char BAR                [4] = "█";
char WIN_TOP            [4] = "─";
char WIN_LEFT           [4] = "│";
char WIN_RIGHT          [4] = "│";
char WIN_BOTTOM         [4] = "─";
char WIN_TOP_RIGHT      [4] = "┐";
char WIN_BOTTOM_RIGHT   [4] = "┘";
char WIN_TOP_LEFT       [4] = "┌";
char WIN_BOTTOM_LEFT    [4] = "└";
char WIN_CLOSE_BTN      [4] = "x";
char WIN_MAXIMIZE_BTN   [4] = "☐";
char WIN_MINIMIZE_BTN   [4] = "_";

struct framebuf *framebuf=NULL;

struct windowsManager * wm = NULL;

char defualt_win_icon[6] = {
    'C','o','n',
    'W','I','N'
};
win *default_wins[20]={
    [WS_DESKTOP] = NULL,
    [WS_TASKBAR] = NULL
};

struct windowsManager* init_WM(void){
    //init framebuf
    framebuf = malloc(sizeof(struct framebuf));

    wm = malloc(sizeof(struct windowsManager) + WM_LENGTH*sizeof(struct window));
    if(wm == NULL)return NULL;
    wm->length = WM_LENGTH;
    wm->top = -1;
    wm->is_commit = 0;
    INIT_BITSMAP(framebuf->bitmaps,4096*4096);
    return wm;
}
win * malloc_window(struct windowsManager * wm){
    if(wm->top < -1) return NULL;
    wm->top++;
    if(wm->top >= wm->length) return NULL;
    struct window *w = &(wm->windows[wm->top]);
    WIN_SET_DEFAULT_ATTR(w);

    INIT_BITSMAP(w->bitmaps, w->width*w->height);

    set_d_mem(w,BLACK_BG,WHITE,BACKGROUND);
    return w;
}
int set_d_mem(win *w,char bgcolor,char color,char *c){
    if(w == NULL)return 1;
    int x,y;
    for(y=0;y<WIN_HEIGHT;y++){
        for(x=0;x<WIN_WIDTH;x++){
            SET_CHAR(w->d_mem[x][y].c,c);
            w->d_mem[x][y].color      = color;
            w->d_mem[x][y].background = bgcolor;
        }
    }
    return 0;
}
win * malloc_window_with_property(struct windowsManager * wm,char bgcolor,char color,char *c){
    if(wm->top < -1) return NULL;
    wm->top++;
    if(wm->top >= wm->length) return NULL;
    struct window *w = &(wm->windows[wm->top]);
    WIN_SET_DEFAULT_ATTR(w);

    INIT_BITSMAP(w->bitmaps, w->width*w->height);

    set_d_mem(w,bgcolor,color,c);
    return w;
}

int free_window(struct windowsManager *wm,win *winptr){
    if(wm == NULL || winptr == NULL)return 1;
    int i;
    for(i=0;i<=wm->top;i++){
        if(winptr == &(wm->windows[i]))break;
    }
    wm->windows[i] = wm->windows[wm->top];
    wm->top--;
    return 0;
}
int free_top_window(struct windowsManager *wm){
    if(wm == NULL)return 1;
    wm->top--;
    return 0;
}

int draw_all_windows(struct windowsManager * wm){
    int i = 0;
    for(;i<=wm->top;i++){
        if(draw_windows(&(wm->windows[i]))) return -1;
    }
    return i;
}

int draw_windows(win* winptr){
    //draw bits maps
    /* setbitmaps(framebuf->bitmaps, winptr->bitmaps, 4096, winptr->height, winptr->width); */

    int x = winptr->x,
        y = winptr->y,
        win_width = winptr->width,
        win_height = winptr->height;
    char flag = winptr->type;
    if(flag & WT_MINIMIZE && !(flag & WT_MAXIMIZE)) goto no_render;
    if(flag & WT_MAXIMIZE && !(flag & WT_MINIMIZE)){
        x=0;
        y=0;
        win_width=width;
        win_height=height;
    }
    if(flag & WT_FRAME){
        for(int indexy = y+1;indexy<y+win_height-1;indexy++){
            SET_CHAR(framebuf->buf[x                ][indexy].c,WIN_LEFT);
            SET_CHAR(framebuf->buf[x+win_width-1        ][indexy].c,WIN_RIGHT);
        }
        for(int indexx = x+1;indexx<x+win_width-1;indexx++){
            SET_CHAR(framebuf->buf[indexx][y                 ].c,WIN_TOP);
            SET_CHAR(framebuf->buf[indexx][y+win_height-1        ].c,WIN_BOTTOM);
        }
        SET_CHAR(framebuf->buf[x                ][y                 ].c,WIN_TOP_LEFT);
        SET_CHAR(framebuf->buf[x                ][y+win_height-1        ].c,WIN_BOTTOM_LEFT);
        SET_CHAR(framebuf->buf[x+win_width-1        ][y                 ].c,WIN_TOP_RIGHT);
        SET_CHAR(framebuf->buf[x+win_width-1        ][y+win_height-1        ].c,WIN_BOTTOM_RIGHT);
    }
    if(flag & WT_COTROL_BTN){
        if(flag & WT_FRAME){
            SET_CHAR(framebuf->buf[x+win_width-2][y+1               ].c,WIN_CLOSE_BTN);
            SET_CHAR(framebuf->buf[x+win_width-3][y+1               ].c,WIN_MAXIMIZE_BTN);
            SET_CHAR(framebuf->buf[x+win_width-4][y+1               ].c,WIN_MINIMIZE_BTN);
        }else{
            SET_CHAR(framebuf->buf[x+win_width-1][y                 ].c,WIN_CLOSE_BTN);
            SET_CHAR(framebuf->buf[x+win_width-2][y                 ].c,WIN_MAXIMIZE_BTN);
            SET_CHAR(framebuf->buf[x+win_width-3][y                 ].c,WIN_MINIMIZE_BTN);
        }
    }

    int i,j,xsize,ysize,xstart,ystart;
    if(flag & WT_TITLE || flag & WT_COTROL_BTN){
        if(flag & WT_FRAME){
            xsize=win_width-2;
            ysize=win_height-3;
        }else{
            xsize=win_width;
            ysize=win_height-1;
        }
    }else{
        if(flag & WT_FRAME){
            xsize=win_width-2;
            ysize=win_height-2;
        }else{
            xsize=win_width;
            ysize=win_height;
        }
    }
    if(flag & WT_TITLE || flag & WT_COTROL_BTN){
        if(flag & WT_FRAME){
            xstart=x+1;
            ystart=y+2;
        }else{
            xstart=x;
            ystart=y+1;
        }
    }else{
        if(flag & WT_FRAME){
            xstart=x+1;
            ystart=y+1;
        }else{
            xstart=x;
            ystart=y;
        }
    }
    for(i=0;i<xsize;i++){
        for(j=0;j<ysize;j++){
            int m=i+xstart,n=j+ystart;
            SET_CHAR(framebuf->buf[m][n].c,winptr->d_mem[i][j].c);
            framebuf->buf[m][n].color      = winptr->d_mem[i][j].color;
            framebuf->buf[m][n].background = winptr->d_mem[i][j].background;
        }
    }
    if(flag & WT_TITLE){
        if(flag & WT_FRAME){
            draw_string(winptr->title,x+1,y+1,win_width-5,1,1,winptr->title_bg,winptr->title_color);
        }else{
            draw_string(winptr->title,x  ,y  ,win_width-3,1,1,winptr->title_bg,winptr->title_color);
        }
    }
no_render:
    //draw icon
    if(flag & WT_ICON){
        win *task_bar = &(wm->windows[1]);
        task_bar->d_mem[0][0].c[0] = defualt_win_icon[0];
        task_bar->d_mem[1][0].c[0] = defualt_win_icon[1];
        task_bar->d_mem[2][0].c[0] = defualt_win_icon[2];
        task_bar->d_mem[0][1].c[0] = defualt_win_icon[3];
        task_bar->d_mem[1][1].c[0] = defualt_win_icon[4];
        task_bar->d_mem[2][1].c[0] = defualt_win_icon[5];
    }
    return 0;
}

int draw_string(string *s,int x_position,int y_position,int xsize,int ysize,char mode,char bg_color,char color){
    if(s == NULL)return 1;
    int y=y_position,x=x_position;
    for(int i=0;i<s->length;i++){
        if(s->s[i] == '\n'){
            x=x_position;
            y++;
        }else{
            if(s->s[i] != '\0'){
                framebuf->buf[x][y].c[0] = s->s[i];
                framebuf->buf[x][y].color = color;
                framebuf->buf[x][y].background = bg_color;
            }
        }
        x++;
        if((x-x_position) >= xsize){
            x=x_position;
            y++;
        }
        if(mode & DS_HIDE && (y-y_position) >= ysize)return 0;
    }
    return 0;
}

int win_draw_string(win *w,string *s,int x_position,int y_position,int xsize,int ysize,char mode,char bg_color,char color){
    if(s == NULL && w == NULL)return 1;
    int y=y_position,x=x_position;
    for(int i=0;i<s->length;i++){
        if(s->s[i] == '\n'){
            x=x_position;
            y++;
        }else{
            if(s->s[i] != '\0'){
                w->d_mem[x][y].c[0] = s->s[i];
                w->d_mem[x][y].color = color;
                w->d_mem[x][y].background = bg_color;
                fillbitmaps(w->bitmaps,w->width,x,y,1);
            }
        }
        x++;
        if((x-x_position) >= xsize){
            x=x_position;
            y++;
        }
        if(mode & DS_HIDE && (y-y_position) >= ysize)return 0;
    }
    return 0;
}

int win_draw_line(win *w,int x,int y,int xend,int yend,struct pixel *line){
    if(w == NULL)return 1;
    if(x<0||y<0)return 1;
    if(xend<0||yend<0)return 1;
    if(x>=WIN_WIDTH||y>=WIN_HEIGHT)return 1;
    if(xend>=WIN_WIDTH||yend>=WIN_HEIGHT)return 1;

    int xsize=xend-x,ysize=yend-y,k=ysize/xsize,indexx,indexy;
    if(xsize>ysize){
        for(indexx = x;indexx<xend;indexx++){
            int tmp_y = k*(indexx-x)+y;
            w->d_mem[indexx][tmp_y] = *line;
        }
    }else{
        for(indexy = y;indexy<yend;indexy++){
            int tmp_x = (indexy-y)/k + x;
            w->d_mem[tmp_x][indexy] = *line;
        }
    }
    return 0;
}
int win_draw_point(win *w,int x,int y,struct pixel * point){
    if(w == NULL)return 1;
    w->d_mem[x][y] = *point;
    return 0;
}
int win_draw_circle(win *w,int x,int y,int r,struct pixel * point){
    if(r <= 0)return 1;
    const int angle = 360;
    const double pi = 2*acos(-1);
    for(int i=0;i<angle;i++){
        double dy = sin(pi*i/angle)*r;
        double dx = cos(pi*i/angle)*r*2;
        int rx=(int)dx+x,ry=(int)dy+y;
        if((rx>=0 && y>=0) && (rx<WIN_WIDTH && ry<WIN_HEIGHT)){
            w->d_mem[rx][ry] = *point;
        }
    }
    return 0;
}
