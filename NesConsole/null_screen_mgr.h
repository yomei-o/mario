#ifndef _NULL_SCREEN_MGR_H_
#define _NULL_SCREEN_MGR_H_


#include "types.h"
#include "nes_screen_mgr.h"

class null_screen_mgr : public NES_screen_mgr
{
public:
  null_screen_mgr(void);
  virtual ~null_screen_mgr();

  boolean lock(pixmap& p);
  boolean unlock();

  void blt();
  void flip();
  

  void clear(PIXEL color);

  boolean set_palette(const uint8 pal[256][3]);
  boolean get_palette(uint8 pal[256][3]);
  boolean set_palette_section(uint8 start, uint8 len, const uint8 pal[][3]);
  boolean get_palette_section(uint8 start, uint8 len, uint8 pal[][3]);

  void assert_palette();

protected:
  PIXEL* buffer;

private:
};

#endif
