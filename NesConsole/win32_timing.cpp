/*
** nester - NES emulator
** Copyright (C) 2000  Darren Ranalli
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/
#include "win32_timing.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef unix
#include <time.h>
#include <sys/time.h>
#endif

#include <stdio.h>

#ifdef unix
static unsigned int get_clock()
{
	struct timeval tv;
	struct timezone tz;
	unsigned int t=0;

	if(!gettimeofday(&tv, &tz)) {
		t  = (unsigned int)tv.tv_sec * 1000;
		t += (unsigned int)tv.tv_usec/1000;
	}

	return t;
}
#endif



static int get_mscount(void)
{
#ifdef _WIN32
	return GetTickCount();
#endif
#ifdef unix
	return get_clock();
#endif
}



// initialization function, call once at startup
static int ms;

void SYS_TimeInit()
{
	ms=get_mscount();
}

// actual time function, returns time in seconds
float SYS_TimeInSeconds()
{
	return (float)(get_mscount()-ms)/1000;
}

// actual time function, returns time in milliseconds
int SYS_TimeInMilliseconds()
{
	return get_mscount()-ms;
}
