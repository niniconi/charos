#include "charos.h"

void new_win(int *type){
    win *w = malloc_window(wm);
    string *str = new_string("Console",8);
    w->type=*type;
    w->title=str;
}
void win_close(){
    if(wm->top >= 2)
        free_window(wm,&(wm->windows[wm->top]));
}
void win_move_up(void){
    win *w = ACTIVE_WIN;
    if(w->y>0)
        w->y--;
}
void win_move_down(void){
    win *w = ACTIVE_WIN;
    if(w->y+w->height<height)
        w->y++;
}
void win_move_right(void){
    win *w = ACTIVE_WIN;
    if(w->x+w->width<width)
        w->x++;
}
void win_move_left(void){
    win *w = ACTIVE_WIN;
    if(w->x>0)
        w->x--;
}

void win_width_add(void){
    win *w = ACTIVE_WIN;
    w->width++;
}
void win_width_sub(void){
    win *w = ACTIVE_WIN;
    if(w->width > 10)
        w->width--;
}
void win_height_add(void){
    win *w = ACTIVE_WIN;
    w->height++;
}
void win_height_sub(void){
    win *w = ACTIVE_WIN;
    if(w->height > 4)
        w->height--;
}
void win_reduction(void){
    win *w = ACTIVE_WIN;
    w->type &= ~WT_MINIMIZE;
    w->type &= ~WT_MAXIMIZE;
}
void win_maximize(void){
    win *w = ACTIVE_WIN;
    if(w->type & WT_MAXIMIZE && !(w->type & WT_MINIMIZE)){
        win_reduction();
    }else{
        w->type |= WT_MAXIMIZE;
        w->type &= ~WT_MINIMIZE;
    }
}
void win_minimize(void){
    win *w = ACTIVE_WIN;
    if(w->type & WT_MINIMIZE && !(w->type & WT_MAXIMIZE)){
        win_reduction();
    }else{
        w->type |= WT_MINIMIZE;
        w->type &= ~WT_MAXIMIZE;
    }
}
