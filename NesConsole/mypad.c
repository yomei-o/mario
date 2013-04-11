#include<stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef unix
#include <unistd.h>
#include <fcntl.h>
#endif


#include "mypad.h"

#ifndef IDAM
#define KEYDEVICE "/dev/input/event1"
#else
#define KEYDEVICE "/dev/input/keypad"
#endif
static int handle=-1;

void mypad_init(void)
{
#ifdef _WIN32
	handle=-1;
#endif
#ifdef linux
	handle=open(KEYDEVICE,O_RDONLY | O_NONBLOCK);
	fprintf(stderr,"open handle=%d",handle);
#endif
}
void mypad_done(void)
{
#ifdef _WIN32
	handle=-1;
#endif
#ifdef linux
	if(handle>=0)
	{
		close(handle);
		handle=-1;
	}
#endif

}



static int mypad_linux_start=0;
static int mypad_linux_select=0;
static int mypad_linux_up=0;
static int mypad_linux_down=0;
static int mypad_linux_right=0;
static int mypad_linux_left=0;
static int mypad_linux_a=0;
static int mypad_linux_b=0;
static int mypad_linux_reset=0;


void mypad_linux_key(char* buf)
{
#ifndef IDAM
	if(buf[10]==0x39 && buf[11]==0 && buf[12]==1)mypad_linux_start=1;
	if(buf[10]==0x39 && buf[11]==0 && buf[12]==0)mypad_linux_start=0;
	if(buf[10]==0x1d && buf[11]==0 && buf[12]==1)mypad_linux_select=1;
	if(buf[10]==0x1d && buf[11]==0 && buf[12]==0)mypad_linux_select=0;
	if(buf[10]==0x67 && buf[11]==0 && buf[12]==1)mypad_linux_up=1;
	if(buf[10]==0x67 && buf[11]==0 && buf[12]==0)mypad_linux_up=0;
	if(buf[10]==0x6c && buf[11]==0 && buf[12]==1)mypad_linux_down=1;
	if(buf[10]==0x6c && buf[11]==0 && buf[12]==0)mypad_linux_down=0;
	if(buf[10]==0x6a && buf[11]==0 && buf[12]==1)mypad_linux_right=1;
	if(buf[10]==0x6a && buf[11]==0 && buf[12]==0)mypad_linux_right=0;
	if(buf[10]==0x69 && buf[11]==0 && buf[12]==1)mypad_linux_left=1;
	if(buf[10]==0x69 && buf[11]==0 && buf[12]==0)mypad_linux_left=0;
	if(buf[10]==0x10 && buf[11]==0 && buf[12]==1)mypad_linux_reset=1;
	if(buf[10]==0x10 && buf[11]==0 && buf[12]==0)mypad_linux_reset=0;
#else
	if(buf[10]==0x25 && buf[11]==0 && buf[12]==1)mypad_linux_start=1;
	if(buf[10]==0x25 && buf[11]==0 && buf[12]==0)mypad_linux_start=0;
	if(buf[10]==0x23 && buf[11]==0 && buf[12]==1)mypad_linux_select=1;
	if(buf[10]==0x23 && buf[11]==0 && buf[12]==0)mypad_linux_select=0;
	if(buf[10]==0x2c && buf[11]==0 && buf[12]==1)mypad_linux_up=1;
	if(buf[10]==0x2c && buf[11]==0 && buf[12]==0)mypad_linux_up=0;
	if(buf[10]==0x2f && buf[11]==0 && buf[12]==1)mypad_linux_down=1;
	if(buf[10]==0x2f && buf[11]==0 && buf[12]==0)mypad_linux_down=0;
	if(buf[10]==0x2e && buf[11]==0 && buf[12]==1)mypad_linux_right=1;
	if(buf[10]==0x2e && buf[11]==0 && buf[12]==0)mypad_linux_right=0;
	if(buf[10]==0x2d && buf[11]==0 && buf[12]==1)mypad_linux_left=1;
	if(buf[10]==0x2d && buf[11]==0 && buf[12]==0)mypad_linux_left=0;
	if(buf[10]==0x1c && buf[11]==0 && buf[12]==1)mypad_linux_reset=1;
	if(buf[10]==0x1c && buf[11]==0 && buf[12]==0)mypad_linux_reset=0;
	if(buf[10]==0x24 && buf[11]==0 && buf[12]==1)mypad_linux_a=1;
	if(buf[10]==0x24 && buf[11]==0 && buf[12]==0)mypad_linux_a=0;
	if(buf[10]==0x21 && buf[11]==0 && buf[12]==1)mypad_linux_b=1;
	if(buf[10]==0x21 && buf[11]==0 && buf[12]==0)mypad_linux_b=0;
#endif
}


#ifdef linux
void mypad_linux_keyread(void)
{
	unsigned char buf[16];
	int ret;

	if(handle>=0)
	{
		ret=read(handle,buf,16);
		//printf("ret=%d ",ret);
		if(ret==16)
		{
			mypad_linux_key((char*)buf);
			fprintf(stderr,">>%02x %02x %02x %02x<<\n",buf[10],buf[11],buf[12],buf[13]);
		}
	}
}
#endif

int mypad_ishit_start(void)
{
#ifdef _WIN32
	return (GetKeyState(VK_SPACE)&0x8000)!=0;
#endif
#ifdef linux
	mypad_linux_keyread();
	return mypad_linux_start;
#endif
	return 0;
}


int mypad_ishit_select(void)
{
#ifdef _WIN32
	return (GetKeyState(VK_CONTROL)&0x8000)!=0;
#endif
#ifdef linux
	mypad_linux_keyread();
	return mypad_linux_select;
#endif
	return 0;
}


int mypad_ishit_up(void)
{
#ifdef _WIN32
	return ((GetKeyState(VK_NUMPAD8)&0x8000)!=0)||((GetKeyState(VK_UP)&0x8000)!=0);
#endif
#ifdef linux
	mypad_linux_keyread();
	return mypad_linux_up;
#endif
	return 0;
}


int mypad_ishit_down(void)
{
#ifdef _WIN32
	return ((GetKeyState(VK_NUMPAD2)&0x8000)!=0)||((GetKeyState(VK_DOWN)&0x8000)!=0);
#endif
#ifdef linux
	mypad_linux_keyread();
	return mypad_linux_down;
#endif
	return 0;
}


int mypad_ishit_right(void)
{
#ifdef _WIN32
	return ((GetKeyState(VK_NUMPAD6)&0x8000)!=0)|| ((GetKeyState(VK_RIGHT)&0x8000)!=0);
#endif
#ifdef linux
	mypad_linux_keyread();
	return mypad_linux_right;
#endif
	return 0;
}


int mypad_ishit_left(void)
{
#ifdef _WIN32
	return ((GetKeyState(VK_NUMPAD4)&0x8000)!=0)||((GetKeyState(VK_LEFT)&0x8000)!=0);
#endif
#ifdef linux
	mypad_linux_keyread();
	return mypad_linux_left;
#endif
	return 0;
}


int mypad_ishit_a(void)
{
#ifdef _WIN32
	return (GetKeyState('X')&0x8000)!=0;
#endif
#ifdef linux
	mypad_linux_keyread();
	return mypad_linux_a;
#endif
	return 0;
}


int mypad_ishit_b(void)
{
#ifdef _WIN32
	return (GetKeyState('Z')&0x8000)!=0;
#endif
#ifdef linux
	mypad_linux_keyread();
	return mypad_linux_b;
#endif
	return 0;
}

int mypad_ishit_reset(void)
{
#ifdef _WIN32
	return (GetKeyState('Q')&0x8000)!=0;
#endif
#ifdef linux
	mypad_linux_keyread();
	return mypad_linux_reset;
#endif
	return 0;
}


int mypad_ishit(int c)
{
	switch(c)
	{
	case MYPAD_START:
		return mypad_ishit_start();
	case MYPAD_SELECT:
		return mypad_ishit_select();
	case MYPAD_UP:
		return mypad_ishit_up();
	case MYPAD_DOWN:
		return mypad_ishit_down();
	case MYPAD_RIGHT:
		return mypad_ishit_right();
	case MYPAD_LEFT:
		return mypad_ishit_left();
	case MYPAD_A:
		return mypad_ishit_a();
	case MYPAD_B:
		return mypad_ishit_b();
	case MYPAD_RESET:
		return mypad_ishit_reset();
	}
	return 0;

}


void mypad_sleep(int ms)
{
#ifdef _WIN32
	Sleep(ms);
#endif
#ifdef linux
	sleep(ms / 1000);
	usleep((ms % 1000) * 1000);
#endif
}




#if 0

int main(int argc,char* argv[])
{
	int i;
	mypad_init();
	for(i=0;i<300;i++)
	{
		int c;
		c='-';
		if(mypad_ishit(MYPAD_START))c='S';
		if(mypad_ishit(MYPAD_SELECT))c='s';
		if(mypad_ishit(MYPAD_UP))c='u';
		if(mypad_ishit(MYPAD_DOWN))c='d';
		if(mypad_ishit(MYPAD_RIGHT))c='r';
		if(mypad_ishit(MYPAD_LEFT))c='l';
		if(mypad_ishit(MYPAD_A))c='a';
		if(mypad_ishit(MYPAD_B))c='b';

		printf("%c",c);
		fflush(stdout);
		mypad_sleep(50);
	}
	mypad_done();
}

#endif
