#ifndef     __CONFIG_H
#define     __CONFIG_H
#include <stdio.h>
#include <string.h>
#include "stc12c5a60s2.h"
#include    <intrins.h>
#ifndef _SYS_GLOBAL_
#define SYS_GLOBAL  extern
#else
#define SYS_GLOBAL
#endif
#define         FSOC        11059200
#define         uint8       unsigned char
#define         int8        char
#define         uint16      unsigned int
#define         int16       int
#define         VERTION     0x13
#define LongToBin(n) \
(\
((n >> 21) & 0x80) | \
((n >> 18) & 0x40) | \
((n >> 15) & 0x20) | \
((n >> 12) & 0x10) | \
((n >> 9) & 0x08) | \
((n >> 6) & 0x04) | \
((n >> 3) & 0x02) | \
((n ) & 0x01) \
)
#define Bin(n) LongToBin(0x##n##l)
#endif
