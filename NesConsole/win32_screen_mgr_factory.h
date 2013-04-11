#ifndef _WIN32_SCREEN_MGR_FACTORY_H_
#define _WIN32_SCREEN_MGR_FACTORY_H_

#include "nes_screen_mgr.h"

NES_screen_mgr* create_win32_screen_mgr(void* hwnd);
void delete_win32_screen_mgr(NES_screen_mgr* mgr);

#endif
