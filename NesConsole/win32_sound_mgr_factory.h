#ifndef _WIN32_SOUND_MGR_FACTORY_H_
#define _WIN32_SOUND_MGR_FACTORY_H_

#include "sound_mgr.h"

sound_mgr* create_win32_sound_mgr(void* window_handle, int sample_rate,
	int sample_size, int buffer_length_in_frames);

void delete_win32_sound_mgr(sound_mgr* mgr);

#endif
