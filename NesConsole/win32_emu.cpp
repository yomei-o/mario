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

#include "win32_emu.h"
#include "win32_timing.h"
#include "nes_settings.h"
#include "win32_screen_mgr_factory.h"
#include "win32_sound_mgr_factory.h"

#include "mypad.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef unix
#include <unistd.h>
#include <fcntl.h>
#endif

int win32_emu::m_all_stop=0;

win32_emu::win32_emu(void* parent_window_handle, const char* ROM_name)
{
  m_stop=0;
  parent_wnd_handle = parent_window_handle;
  scr_mgr = NULL;
  snd_mgr = &local_null_snd_mgr;
  emu = NULL;

  mypad_init();
  SYS_TimeInit();

  try {
    try {
      scr_mgr = create_win32_screen_mgr(parent_wnd_handle);
    } catch(...) {
      throw;// "error creating screen manager";
    }


    // get a null sound mgr
		snd_mgr = &local_null_snd_mgr;

    try {
      emu = new NES(ROM_name,scr_mgr,snd_mgr);
    } catch(...) {
      throw;// "error creating emulated NES";
    }

    scr_mgr->setParentNES((NES*)emu);


    // start the timer off right
    reset_last_frame_time();

    // try to init dsound if appropriate
    enable_sound(NESTER_settings.nes.sound.enabled);

    // set up control pads
    emu->set_pad1(&pad1);
    emu->set_pad2(&pad2);

  } catch(...) {
    // careful of the order here
    if(emu) delete emu;
    if(scr_mgr) delete_win32_screen_mgr(scr_mgr);
  	if(snd_mgr != &local_null_snd_mgr) delete_win32_sound_mgr(snd_mgr);
    throw;
  }

}

win32_emu::~win32_emu()
{
  if(emu) delete emu;
  if(scr_mgr) delete_win32_screen_mgr(scr_mgr);
	if(snd_mgr != &local_null_snd_mgr) delete_win32_sound_mgr(snd_mgr);
	 mypad_done();

}

void win32_emu::OnKeyDown(int keycode,bool pushed)
{
	pad1.set_button_state(keycode,pushed);
}



boolean win32_emu::emulate_frame(boolean draw)
{
  return emu->emulate_frame(draw);
}


void win32_emu::reset_last_frame_time()
{
  last_frame_time = SYS_TimeInMilliseconds();


}

const char* win32_emu::getROMname()
{
  return emu->getROMname();
}

const char* win32_emu::getROMnameExt()
{
  return emu->getROMnameExt();
}

const char* win32_emu::getROMpath()
{
  return emu->getROMpath();
}

NES_ROM* win32_emu::get_NES_ROM()
{
  return emu->get_NES_ROM();
}

boolean win32_emu::loadState(const char* fn)
{
  boolean result;

  freeze();
  result = emu->loadState(fn);
  thaw();

  return result;
}

boolean win32_emu::saveState(const char* fn)
{
  boolean result;

  freeze();
  result = emu->saveState(fn);
  thaw();

  return result;
}

void win32_emu::reset()
{
  freeze();
  emu->reset();
  thaw();
}

void win32_emu::blt()
{
  scr_mgr->blt();
}

void win32_emu::flip()
{
  scr_mgr->flip();
}

void win32_emu::assert_palette()
{
  scr_mgr->assert_palette();
}


void win32_emu::enable_sound(boolean enable)
{
  freeze();

	if(snd_mgr != &local_null_snd_mgr)
	{
		delete_win32_sound_mgr(snd_mgr);
		snd_mgr = &local_null_snd_mgr;
	}

	if(enable)
	{
		// try to init dsound
		try {
      try {
#ifdef I_USE_SOUND
				snd_mgr = create_win32_sound_mgr(parent_wnd_handle,
					NESTER_settings.nes.sound.sample_rate, NESTER_settings.nes.sound.sample_size,
          NESTER_settings.nes.sound.buffer_len);
#endif
      } catch(const char* IFDEBUG(s)) {
        LOG(s << endl);
        throw;
      }
		} catch(...) {
			LOG("Directsound initialization failed" << endl);
    	snd_mgr = &local_null_snd_mgr;
		}
	}

	((NES*)emu)->new_snd_mgr(snd_mgr);

  thaw();
}

boolean win32_emu::sound_enabled()
{
  return !snd_mgr->IsNull();
}

boolean win32_emu::set_sample_rate(int sample_rate)
{
  if(!sound_enabled()) return FALSE;
  if(get_sample_rate() == sample_rate) return TRUE;
  return TRUE;
}

int win32_emu::get_sample_rate()
{
  return snd_mgr->get_sample_rate();
}

static void win32_sleep(int ms)
{
#ifdef _WIN32
	::Sleep(ms);
#endif
#ifdef linux
	sleep(ms / 1000);
	usleep((ms % 1000) * 1000);
#endif
}

// STATIC FUNCTIONS
static inline void SleepUntil(long time)
{
  long timeleft;

  while(1)
  {
    timeleft = time - long(SYS_TimeInMilliseconds());
    if(timeleft <= 0) break;

    if(timeleft > 2)
    {
      win32_sleep((timeleft) - 1);
    }
  }
}

/*
When the NTSC standard was designed, certain frequencies involved
in the color subcarrier were interfering with the 60 Hz power lines.  So
the NTSC engineers set the framerate to 60000/1001 Hz.  See also
"drop frame timecode" on any search engine for the full story.
*/
#define NTSC_FRAMERATE (60000.0/1001.0)
#define FRAME_PERIOD   (1000.0/NTSC_FRAMERATE)

#define THROTTLE_SPEED  (NESTER_settings.nes.preferences.speed_throttling)
#define SKIP_FRAMES     (NESTER_settings.nes.preferences.auto_frameskip && THROTTLE_SPEED)


void win32_emu::do_frame()
{
  uint32 frames_since_last;

  if(frozen()) return;

  // at this point, last_frame_time is set to the time when the last frame was drawn.

  // get the current time
  cur_time = SYS_TimeInMilliseconds();

  // make up for missed frames
  if(SKIP_FRAMES)
  {
    frames_since_last = (uint32)((cur_time - last_frame_time) / FRAME_PERIOD);

    // are there extra frames?
    if(frames_since_last > 1)
    {
      for(uint32 i = 1; i < frames_since_last; i++)
      {
        if(i == 4) break;
        last_frame_time += FRAME_PERIOD;
        emulate_frame(FALSE);
      }
    }
  }

  // emulate current frame
  if(emulate_frame(TRUE))
  {
    // display frame
    blt();
    flip();
  }

  // sleep until this frame's target time
  if(THROTTLE_SPEED)
  {
    SleepUntil(long(last_frame_time + FRAME_PERIOD));
  }


  // get ready for next frame
  if(THROTTLE_SPEED)
  {
    last_frame_time += FRAME_PERIOD;
  }
  else
  {
    last_frame_time = cur_time;
  }
}

void win32_emu::main_thread(void* vp)
{
	win32_emu* emu;
	emu=(win32_emu*)vp;

	while(1)
	{
	    if(emu && !emu->frozen())
		{
	      emu->do_frame();
		}
		if(emu->m_stop)break;


		if(mypad_ishit_start())emu->OnKeyDown(NES_START,true);
		else emu->OnKeyDown(NES_START,false);
		if(mypad_ishit_select())emu->OnKeyDown(NES_SELECT,true);
		else emu->OnKeyDown(NES_SELECT,false);

		if(mypad_ishit_up())emu->OnKeyDown(NES_UP,true);
		else emu->OnKeyDown(NES_UP,false);
		if(mypad_ishit_down())emu->OnKeyDown(NES_DOWN,true);
		else emu->OnKeyDown(NES_DOWN,false);
		if(mypad_ishit_right())emu->OnKeyDown(NES_RIGHT,true);
		else emu->OnKeyDown(NES_RIGHT,false);
		if(mypad_ishit_left())emu->OnKeyDown(NES_LEFT,true);
		else emu->OnKeyDown(NES_LEFT,false);

		if(mypad_ishit_a())emu->OnKeyDown(NES_A,true);
		else emu->OnKeyDown(NES_A,false);
		if(mypad_ishit_b())emu->OnKeyDown(NES_B,true);
		else emu->OnKeyDown(NES_B,false);

		if(mypad_ishit_reset())emu->m_stop=1;
		if(m_all_stop)emu->m_stop=1;
		win32_sleep(10);

	}
	emu->m_stop=2;
}

void win32_emu::wait_thread(void)
{
	m_stop=1;
	while(m_stop==1)
	{
		win32_sleep(100);
	}
}

void win32_emu::stop_all_thread(void)
{
	m_all_stop=1;
}
