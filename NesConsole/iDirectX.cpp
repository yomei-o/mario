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

#define DIRECTINPUT_VERSION 0x0700

#include "iDirectX.h"

LPDIRECTSOUND iDirectX::lpDS = NULL;
LPDIRECTSOUND iDirectX::getDirectSound()
{
  if(!lpDS)
  {
    DirectSoundCreate(NULL, &lpDS, NULL);
  }
  return lpDS;
}

void iDirectX::releaseDirectSound()
{
  if(lpDS)
  {
    lpDS->Release();
    lpDS = NULL;
  }
}




