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

#include <windows.h>
#include <process.h>
#include "win32_emu.h"
#include "nes_ppu.h"
#include "win32_window.h"

#define WINCLASS_NAME "WinClass_nester"
#define PROG_NAME "Supermario"

#define SCREEN_WIDTH_WINDOWED   (2*NES_PPU::NES_SCREEN_WIDTH_VIEWABLE)
#define SCREEN_HEIGHT_WINDOWED  (2*NES_PPU::getViewableHeight())

#define APPROX_WINDOW_WIDTH  (SCREEN_WIDTH_WINDOWED + 2*GetSystemMetrics(SM_CXFIXEDFRAME))
#define APPROX_WINDOW_HEIGHT (SCREEN_HEIGHT_WINDOWED + GetSystemMetrics(SM_CYMENU) \
                         + GetSystemMetrics(SM_CYSIZE) \
                         + 2*GetSystemMetrics(SM_CYFIXEDFRAME) \
                         + 1)

#define STYLE_WINDOWED (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | \
      WS_MINIMIZEBOX /*| *//*WS_MAXIMIZEBOX | *//*WS_POPUP*//* | WS_VISIBLE*/)



static HWND my_hwnd=NULL;

void assertWindowSize()
{
  RECT rct;

  // set rc to client size
  SetRect(&rct, 0, 0, SCREEN_WIDTH_WINDOWED, SCREEN_HEIGHT_WINDOWED);

  // adjust rc to full window size
  AdjustWindowRectEx(&rct,
                     GetWindowLong(my_hwnd,GWL_STYLE),
                     GetMenu(my_hwnd) != NULL,
                     GetWindowLong(my_hwnd,GWL_STYLE));

  SetWindowPos(my_hwnd, HWND_TOP, 0, 0,
    rct.right-rct.left, rct.bottom-rct.top, SWP_NOCOPYBITS | SWP_NOMOVE | SWP_NOZORDER);
}

void CenterWindow()
{
  RECT rct;

  // set rc to client size
  SetRect(&rct, 0, 0, SCREEN_WIDTH_WINDOWED, SCREEN_HEIGHT_WINDOWED);

  // adjust rc to full window size
  AdjustWindowRectEx(&rct,
                     GetWindowLong(my_hwnd,GWL_STYLE),
                     GetMenu(my_hwnd) != NULL,
                     GetWindowLong(my_hwnd,GWL_STYLE));

  SetWindowPos(my_hwnd, HWND_TOP,
               GetSystemMetrics(SM_CXFULLSCREEN)/2 - (rct.right-rct.left)/2,
               GetSystemMetrics(SM_CYFULLSCREEN)/2 - (rct.bottom-rct.top)/2,
               0, 0, SWP_NOCOPYBITS | SWP_NOSIZE | SWP_NOZORDER);
}



LRESULT CALLBACK WindowProc(HWND hwnd, 
                            UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
  PAINTSTRUCT  ps;      // used in WM_PAINT
  HDC          hdc;     // handle to a device context


  switch(msg)
  {

    case WM_CREATE:
      return(0);
    case WM_DESTROY:
      PostQuitMessage(0);
      return(0);
//    case WM_MOVE:
//      if(emu)emu->blt();
//      break;
    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      EndPaint(hwnd, &ps);
//    if(emu)emu->blt();
      return(0);
  } // end switch

  // process any messages that we didn't take care of 
  return(DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc



// WINMAIN ////////////////////////////////////////////////

static void msg_thread(void* vp)
{
  WNDCLASSEX  winclass; // window class we create
  HWND     hwnd;      // window handle
  MSG      msg;       // message



  winclass.cbSize     = sizeof(winclass);
  winclass.style      = CS_OWNDC | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
  winclass.lpfnWndProc  = WindowProc;
  winclass.cbClsExtra   = 0;
  winclass.cbWndExtra   = 0;
  winclass.hInstance    = GetModuleHandle(NULL);
  winclass.hIcon        = NULL;
  winclass.hCursor      = LoadCursor(NULL, IDC_ARROW);
  winclass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
  winclass.lpszMenuName   = NULL;
  winclass.lpszClassName  = WINCLASS_NAME;
  winclass.hIconSm      = NULL;

  // register the window class
  if(!RegisterClassEx(&winclass))
    return;


  // create the window
  if(!(hwnd = CreateWindowEx(0,
                WINCLASS_NAME, // class
                PROG_NAME,   // title
                STYLE_WINDOWED,
                CW_USEDEFAULT, // x
                CW_USEDEFAULT, // y
                0,  // width
                0, // height
                NULL,     // handle to parent
                NULL,     // handle to menu
                GetModuleHandle(NULL),// instance
                NULL)))  // creation parms
    return;
  my_hwnd=hwnd;



  assertWindowSize();
  CenterWindow();

  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);
  while(GetMessage(&msg, NULL, 0, 0))
  {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
  }
  win32_emu::stop_all_thread();
}


void delete_win32_window(void* vp)
{
	return;
}



void* create_win32_window(void)
{

 
  _beginthread(msg_thread,0,NULL);
  while(my_hwnd==NULL)Sleep(100);
  return (void*)my_hwnd;

#if 0  
  emu = new win32_emu(main_window_handle,"smb3.nes");
  _beginthread(win32_emu::main_thread,0,emu);

  while(GetMessage(&msg, NULL, 0, 0))
  {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
  }
  emu->wait_thread();
  if(emu)
  {
	  delete emu;
  }
  return(msg.wParam);
#endif
}



