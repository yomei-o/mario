/*
** Nofrendo (c) 1998-2000 Matthew Conte (matt@conte.com)
**
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of version 2 of the GNU Library General 
** Public License as published by the Free Software Foundation.
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
**
**
** types.h
**
** Data type definitions
** $Id: types.h,v 1.2 2007/07/26 09:26:33 yomei Exp $
*/

#ifndef _TYPESAA_H_
#define _TYPESAA_H_

/* Define this if running on little-endian (x86) systems */
#define  HOST_LITTLE_ENDIAN

/* These should be changed depending on the platform */
typedef  signed char     int8;
typedef  short    int16;
typedef  int      int32;

typedef  unsigned char  uint8;
typedef  unsigned short uint16;
typedef  unsigned int   uint32;

typedef  uint8    boolean;

#ifndef  TRUE
#define  TRUE     1
#endif
#ifndef  FALSE
#define  FALSE    0
#endif

#endif /* _TYPES_H_ */

/*
** $Log: types.h,v $
** Revision 1.2  2007/07/26 09:26:33  yomei
** no message
**
** Revision 1.1  2007/07/25 23:59:25  yomei
** no message
**
** Revision 1.6  2000/06/09 15:12:25  matt
** initial revision
**
*/



