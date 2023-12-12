/*
 * md5.c requires two macros to stuff and extract 32-bit values from
 * a byte array in LSB order. Calling on fair use, I excerpt them
 * here. They're pretty trivial. --jonh
 *
 * original comment below:
 */
/*

getput.h

Author: Tatu Ylonen <ylo@cs.hut.fi>

Copyright (c) 1995 Tatu Ylonen <ylo@cs.hut.fi>, Espoo, Finland
                   All rights reserved

Created: Wed Jun 28 22:36:30 1995 ylo

Macros for storing and retrieving data in msb first and lsb first order.

*/

#ifndef GETPUT_H
#define GETPUT_H

#define GET_32BIT_LSB_FIRST(cp) \
  (((unsigned long)(unsigned char)(cp)[0]) | \
  ((unsigned long)(unsigned char)(cp)[1] << 8) | \
  ((unsigned long)(unsigned char)(cp)[2] << 16) | \
  ((unsigned long)(unsigned char)(cp)[3] << 24))

#define PUT_32BIT_LSB_FIRST(cp, value) do { \
  (cp)[0] = (value); \
  (cp)[1] = (value) >> 8; \
  (cp)[2] = (value) >> 16; \
  (cp)[3] = (value) >> 24; } while (0)

#endif /* GETPUT_H */

