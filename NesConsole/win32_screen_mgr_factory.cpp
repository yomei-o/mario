#include "win32_screen_mgr_factory.h"
#include "null_screen_mgr.h"

#if 1
#include "win32_windowed_nes_screen_mgr_org.h"
#else
#include "win32_windowed_nes_screen_mgr.h"
#endif

NES_screen_mgr* create_win32_screen_mgr(void* hwnd)
{
	return new win32_windowed_NES_screen_mgr((void*) hwnd);
#if 0
	return new null_screen_mgr();
#endif
}
void delete_win32_screen_mgr(NES_screen_mgr* mgr)
{
	if(mgr)delete mgr;
}
