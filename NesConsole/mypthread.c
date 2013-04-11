
/* スレッド作成ライブラリ
（Linux p_threadベース）*/
#include <stdio.h>

#ifdef unix
#include <unistd.h>
#include <pthread.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#endif

#include "mypthread.h"

#ifndef PTHREAD_TEST

void my_pthread_create(void (*func)(void*), void *arg)
{
#ifdef _WIN32
	_beginthread(func, 0, arg);
#endif
#ifdef unix
	pthread_t ph;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); 
	pthread_create(&ph, &attr, (void*(*)(void*))func, arg);
	pthread_attr_destroy(&attr);
#endif
}
#endif /* PTHREAD_TEST */



#ifdef PTHREAD_TEST

void mysleep(int s)
{
#ifdef unix
	sleep(s);
#endif
#ifdef _WIN32
	Sleep(s*1000);
#endif
}

/* サブスレッド */
void subthread(void* vp)
{
	int i;

	printf("sub thread start\n");

	for(i=0; i<5; i++){
		printf("-\n");
		mysleep(1);
	}
	
	printf("sub thread end\n");	
}

int main()
{
	my_pthread_create(subthread, NULL);

	mysleep(2);
	printf("main thread end\n");
}

#endif /* PTHREAD_TEST */

