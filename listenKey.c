#include <stdio.h>
#include <termios.h>

struct termios new_config;
struct termios default_config;

void init_key_listener(){
	tcgetattr(0,&default_config);
	new_config = default_config;
	new_config.c_lflag &= (~ICANON);//不使用回车即可获取输入
	//new_config.c_lflag |= ECHO;//显示输入
	new_config.c_lflag &= (~ECHO);//不显示输入
	new_config.c_lflag |= ISIG;
	new_config.c_cc[VTIME] = 0;
	new_config.c_cc[VMIN] = 1;
	tcsetattr(0,TCSANOW,&new_config);
}
void close_key_listener(){
	tcsetattr(0,TCSANOW,&default_config);
}

char key_listener_get_char(){
	char temp = 0;
	scanf("%c",&temp);
	return temp;
}
