#ifndef __CHAROS_H
#define __CHAROS_H
#define STOP_ON_STARTUP 2

#define INFO(label,args...)   printf("[info ][%s]:%s\n",label,##args)
#define DEBUG(label,args...)  printf("[debug][%s]:%s\n",label,##args)
#define ERROR(label,args...)  printf("[error][%s]:%s\n",label,##args)
/* #define WIDTH 64 */
/* #define HEIGHT 32 */
extern unsigned int width;
extern unsigned int height;

#define WIN_WIDTH                256
#define WIN_HEIGHT               192

#define DEFAULT_WIN_X             3
#define DEFAULT_WIN_Y             3
#define DEFAULT_WIN_W             20//width
#define DEFAULT_WIN_H             20//height

#define WM_LENGTH                 255

//draw string function's mode
#define DS_HIDE          0b00000001

#define BLACK      30 
#define BLACK_BG   40
#define RED        31
#define RED_BG     41
#define GREEN      32
#define GREEN_BG   42
#define YELLOW     33
#define YELLOW_BG  43
#define BLUE       34
#define BLUE_BG    44
#define MAGENTA    35
#define MAGENTA_BG 45
#define CYAN       36
#define CYAN_BG    46
#define WHITE      37
#define WHITE_BG   47
#define NULL_COL   0

#define EVENT_NUL      0x00 //NULL 空字符
#define EVENT_SOH      0x01 //SOH (Start Of Headling) 标题开始
#define EVENT_STX      0x02 //STX (Start Of Text) 正文开始
#define EVENT_ETX      0x03 //ETX (End Of Text) 正文结束
#define EVENT_EOT      0x04 //EOT (End Of Transmission) 传输结束
#define EVENT_ENQ      0x05 //ENQ (Enquiry) 请求
#define EVENT_ACK      0x06 //ACK (Acknowledge) 回应/响应/收到通知
#define EVENT_BEL      0x07 //BEL (Bell) 响铃
#define EVENT_BS       0x08  //BS (Backspace) 退格
#define EVENT_HT       0x09 //HT (Horizontal Tab) 水平制表符
#define EVENT_LF_NL    0x0A //LF/NL(Line Feed/New Line) 换行键
#define EVENT_VT       0x0B //VT (Vertical Tab) 垂直制表符
#define EVENT_FF_NP    0x0C //FF/NP (Form Feed/New Page) 换页键
#define EVENT_CR       0x0D //CR (Carriage Return) 回车键
#define EVENT_SO       0x0E //SO (Shift Out) 不用切换
#define EVENT_SI       0x0F //SI (Shift In) 启用切换
#define EVENT_DLE      0x10 //DLE (Data Link Escape) 数据链路转义
#define EVENT_DC1_XON  0x11 //DC1/XON (Device Control 1/Transmission On) 设备控制1/传输开始
#define EVENT_DC2      0x12 //DC2 (Device Control 2) 设备控制2
#define EVENT_DC3_XOFF 0x13 //DC3/XOFF (Device Control 3/Transmission Off) 设备控制3/传输中断
#define EVENT_DC4      0x14 //DC4 (Device Control 4) 设备控制4
#define EVENT_NAK      0x15 //NAK (Negative Acknowledge) 无响应/非正常响应/拒绝接收
#define EVENT_SYN      0x16 //SYN (Synchronous Idle) 同步空闲
#define EVENT_ETB      0x17 //ETB (End of Transmission Block) 传输块结束/块传输终止
#define EVENT_CAN      0x18 //CAN (Cancel) 取消
#define EVENT_EM       0x19 //EM (End of Medium) 已到介质末端/介质存储已满/介质中断
#define EVENT_SUB      0x1A //SUB (Substitute) 替补/替换
#define EVENT_ESC      0x1B //ESC (Escape) 逃离/取消
#define EVENT_FS       0x1C //FS (File Separator) 文件分割符
#define EVENT_GS       0x1D //GS (Group Separator) 组分隔符/分组符
#define EVENT_RS       0x1E //RS (Record Separator) 记录分离符
#define EVENT_US       0x1F //US (Unit Separator) 单元分隔符
#define EVENT_SPACE    0x20 //(Space) 空格
#define EVENT_DEL      0x7F //DEL (Delete) 删除

#define DEFAULT_TITLE_BG WHITE_BG
#define DEFAULT_TITLE_COLOR BLACK


#define EVENT_KEY_BUF_SIZE           32
#define EVENT_MOUSE_BUF_SIZE         8

#define KEY_NULL                     (char)-1
#define KEY_PUT_FAILED               1
#define KEY_PUT_SUCCESS              0

#define EVENT_MGR_KEY_EVENT_COUNT    32
#define EVENT_MGR_MOUSE_EVENT_COUNT  16

#define FPS(f) ((1000*1000)/(f))
#define SET_CHAR(d,s) \
    do{ \
        d[0] = s[0];\
        d[1] = s[1];\
        d[2] = s[2];\
        d[3] = s[3];\
    }while(0)

#define WIN_SET_DEFAULT_ATTR(w) \
    do{ \
        w->x           = DEFAULT_WIN_X;\
        w->y           = DEFAULT_WIN_Y;\
        w->width       = DEFAULT_WIN_W;\
        w->height      = DEFAULT_WIN_H;\
        w->type        = WINDOWS_DEFAULT_TYPE;\
        w->title_bg    = DEFAULT_TITLE_BG;\
        w->title_color = DEFAULT_TITLE_COLOR;\
    }while(0)

#define WIN_SET_POSITION(w,x,y) \
    do{ \
        w->x = x; \
        w->y = y; \
    }while(0)

#define INIT_BITSMAP(ptr,size) \
    do{ \
        (ptr)=malloc((size)/8 + 1); \
        memset((ptr), 0xff, ((size)/8 + 1)); \
    }while(0)

//window type field, set the bit is to open this option
#define WINDOWS_DEFAULT_TYPE   7
#define WT_COTROL_BTN          0b00000001
#define WT_TITLE               0b00000010
#define WT_FRAME               0b00000100
#define WT_ACTIVE              0b00001000//when this window is active ,if display
#define WT_MAXIMIZE            0b00010000
#define WT_MINIMIZE            0b00100000
#define WT_ICON                0b01000000

extern char BACKGROUND         [4];
extern char BAR                [4];
extern char WIN_TOP            [4];
extern char WIN_LEFT           [4];
extern char WIN_RIGHT          [4];
extern char WIN_BOTTOM         [4];
extern char WIN_TOP_RIGHT      [4];
extern char WIN_BOTTOM_RIGHT   [4];
extern char WIN_TOP_LEFT       [4];
extern char WIN_BOTTOM_LEFT    [4];
extern char WIN_CLOSE_BTN      [4];
extern char WIN_MAXIMIZE_BTN   [4];
extern char WIN_MINIMIZE_BTN   [4];

struct application{
};
struct applicationManager{
    int count;
    struct application applications[0];
};

struct string{
    int length;
    char s[0];
};
typedef struct string string;
struct string_manager{
    int length;
    int top;
    struct string *string_ptr[0];
};
typedef struct string_manager strManager;

struct pixel{
    char c[4];//the char
    char color;
    char background;
};

struct window{
    int x,y;
    int width,height;
    char type;
    string *title;
    char title_bg,title_color;
    char icon[6];
    //display mem
    struct pixel d_mem[WIN_WIDTH][WIN_HEIGHT];
    char * bitmaps;
};

struct windowsManager{
    int length;
    int top;
    char is_commit;
    struct window windows[0];
};

struct event_key_input_buf{
    char buf[EVENT_KEY_BUF_SIZE];
    unsigned int r,w,null_count;
};

struct event_mouse_input_buf{
    char buf[EVENT_MOUSE_BUF_SIZE];
    unsigned int r,w,null_count;
};

struct key_event{
    char key;
    void *param;
    void *function;
};

struct event_manager{
    int key_event_count;
    int key_event_top;
    struct key_event events[0];
};
struct framebuf{
    struct pixel buf[4096][4096];
    char *bitmaps;
};

typedef struct event_manager          event_mgr;



typedef struct window                 win;
typedef unsigned int                  fps;
typedef struct event_key_input_buf    EKIB;
typedef struct event_mouse_input_buf  EMIB;

extern strManager * sm;
extern event_mgr * em;
extern struct windowsManager * wm;
extern struct framebuf *framebuf;
extern win *default_wins[20];

extern inline int isfillbitmaps(char *maps,int width,int x,int y);
extern inline void fillbitmaps(char*maps,int width,int x,int y,int value);
extern inline void setbitmaps(char *master,char *slave,int master_xsize,int slave_xsize,int slave_ysize);


void init(void);
int flush_win_size(fps *f);
int render_thread(fps *f);
int event_listener(fps *f);
int event_execute();
int system_time(fps *f);
int fillbox(int x,int y,int width,int height,char *c,char bgcolor,char color);
int fillchar(int x,int y,char *c,char bgcolor,char color);
void flush(void);

int draw_windows(win* winptr);
int draw_all_windows(struct windowsManager * wm);
struct windowsManager* init_WM(void);
win* malloc_window(struct windowsManager *wm);
win* malloc_window_with_property(struct windowsManager *wm,char bgcolor,char color,char *c);
int set_d_mem(win *w,char bgcolor,char color,char *c);
int free_window(struct windowsManager *wm,win *winptr);
int free_top_window(struct windowsManager *wm);
int draw_string(string *s,int x_position,int y_position,int xsize,int ysize,char mode,char bg_color,char color);
int win_draw_string(win *w,string *s,int x_position,int y_position,int xsize,int ysize,char mode,char bg_color,char color);
int win_draw_line(win *w,int x,int y,int xend,int yend,struct pixel *line);
int win_draw_point(win *w,int x,int y,struct pixel * point);
int win_draw_circle(win *w,int x,int y,int r,struct pixel * point);

#define DEFAULT_STRING_MANAGER_SIZE 32
#define STRUBG_MANAGER_GRAIN_SIZE 8
string* new_string(const char *s,int size);
string* new_string_n(int size);
string* new_string_z(const char*s);
int push_string(string *str);
strManager* init_SM(void);
strManager* enlarge_sm(void);
int read_properties(const char *path);
char get_key();
int put_key(char);
void init_key_buf();
void init_mouse_buf();
event_mgr *init_event_manager();
int register_key(char key,void*function,void *param);
#define clear_key_buf() init_key_buf
#define clear_mouse_buf() init_mouse_buf
void init_key_listener(void);
void close_key_listener(void);
char key_listener_get_char(void);

//cmd function
void quit(void);
#define ACTIVE_WIN (&(wm->windows[wm->top]))
void new_win(int *type);
void win_move_up(void);
void win_move_down(void);
void win_move_left(void);
void win_move_right(void);
void win_close(void);
void win_width_add(void);
void win_width_sub(void);
void win_height_add(void);
void win_height_sub(void);
void win_maximize(void);
void win_minimize(void);
void win_reduction(void);
#endif
