#include "win32_sound_mgr_factory.h"
#include "null_sound_mgr.h"

#ifdef _WINDOWS
#include "win32_directsound_sound_mgr.h"
#endif

sound_mgr* create_win32_sound_mgr(void* window_handle, int sample_rate,
	int sample_size, int buffer_length_in_frames)
{
#ifdef _WINDOWS
	return new win32_directsound_sound_mgr((HWND)window_handle,sample_rate,
		sample_size,buffer_length_in_frames); 
#endif
#ifndef _WINDOWS
	return new null_sound_mgr();
#endif
}

void delete_win32_sound_mgr(sound_mgr* mgr)
{
	if(mgr)delete mgr;
}



