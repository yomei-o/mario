#ifndef _MYPAD_H_
#define _MYPAD_H_


#define MYPAD_START		1
#define MYPAD_SELECT	2
#define MYPAD_UP		3
#define MYPAD_DOWN		4
#define MYPAD_RIGHT		5
#define MYPAD_LEFT		6
#define MYPAD_A			7
#define MYPAD_B			8
#define MYPAD_RESET		9


#ifdef __cplusplus
extern "C"
{
#endif


void mypad_init(void);
void mypad_done(void);

int mypad_ishit_start(void);
int mypad_ishit_select(void);
int mypad_ishit_up(void);
int mypad_ishit_down(void);
int mypad_ishit_right(void);
int mypad_ishit_left(void);
int mypad_ishit_a(void);
int mypad_ishit_b(void);
int mypad_ishit_reset(void);

int mypad_ishit(int c);
void mypad_sleep(int ms);
void mypad_linux_key(char* buf);


#ifdef __cplusplus
}
#endif


#endif




