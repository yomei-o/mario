#include <stdlib.h>

#include "null_screen_mgr.h"
#include "debug.h"

null_screen_mgr::null_screen_mgr(void)
{
	buffer=NULL;
	buffer=(unsigned char*)malloc(1024*1024);
}

null_screen_mgr::~null_screen_mgr()
{
	if(buffer)free(buffer);
}

boolean null_screen_mgr::lock(pixmap& p)
{
  p.height = get_height();
  p.width  = get_width();
  p.pitch  = get_width()*2;
  p.data   = buffer;
  return TRUE;
}

boolean 
null_screen_mgr::unlock()
{
  return TRUE;
}

void null_screen_mgr::blt()
{
}

void null_screen_mgr::clear(PIXEL color)
{
}

boolean null_screen_mgr::set_palette(const uint8 pal[256][3])
{
  return TRUE;
}

boolean null_screen_mgr::get_palette(uint8 pal[256][3])
{
  return FALSE;
}

boolean null_screen_mgr::set_palette_section(uint8 start, uint8 len, const uint8 pal[][3])
{
  return TRUE;
}

boolean null_screen_mgr::get_palette_section(uint8 start, uint8 len, uint8 pal[][3])
{
  return FALSE;
}

void null_screen_mgr::assert_palette()
{
}

void null_screen_mgr::flip()
{
}
