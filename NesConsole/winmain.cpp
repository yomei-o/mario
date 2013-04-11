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
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.66  
zzxzxxx*xx*
** Any permitted reproduction of these routines, in whole or in part,
** must bear txxxxxxhis legend.
*/

#include "win32_emu.h"
#include "nes_ppu.h"

#if 1
//#include <windows.h>
#include "win32_window.h"
#endif



static void* create_something_window(void)
{
#if 1
	return create_win32_window();
#else
	return (void*)fb0_create();
#endif
	return NULL;
}
static void delete_something_window(void* vp)
{
#if 1
	delete_win32_window(vp);
	return;
#else
	fb0_close((FB0*)vp);
	return;
#endif
	return;
}

int realmain(char* fname)
{
	static void* main_window_handle = NULL; // save the window handle
	static win32_emu* emu=NULL;
	main_window_handle=create_something_window();
	if(main_window_handle)
	{
		emu = new win32_emu((void*)main_window_handle,fname);
		if(emu){
			win32_emu::main_thread(emu);
			delete emu;
			emu=NULL;
		}
	}
	delete_something_window(main_window_handle);
	main_window_handle=NULL;
	return 1;
}






#ifdef _WINDOWS
int WINAPI WinMain(HINSTANCE hinstance,
                   HINSTANCE hprevinstance,
                   LPSTR lpcmdline,
                   int ncmdshow)
{
	char *fname;

	if(lpcmdline[0]==0)fname="smb3.nes";
	else fname=(char*)lpcmdline;
	realmain(fname);
	return 0;
}
#endif

#ifndef _WINDOWS
int main(int argc,char *argv[])
{
	char *fname;

	if(argc<2)fname="mario.nes";
	else fname=argv[1];
	realmain(fname);

	return 1;
}
#endif

