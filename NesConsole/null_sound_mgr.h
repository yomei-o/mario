#ifndef _NULL_SOUND_MGR_H_
#define _NULL_SOUND_MGR_H_

#include "sound_mgr.h"

class null_sound_mgr : public sound_mgr
{
public:
  null_sound_mgr() : sound_mgr(0,0,0) {}
  ~null_sound_mgr() {}

  void reset() {}

  // lock down for a period of inactivity
  void freeze() {}
  void thaw() {}

  void clear_buffer() {}

  boolean lock(sound_buf_pos which, void** buf, uint32* buf_len) { return FALSE; }
  void unlock() {}

  int get_buffer_len()  { return 0; }

  // returns SOUND_BUF_LOW or SOUND_BUF_HIGH
  sound_buf_pos get_currently_playing_half() { return SOUND_BUF_LOW; }

  boolean IsNull() { return TRUE; }
};

#endif
