#ifndef __DELAY_H
#define __DELAY_H

#include "stc12c5a60s2.h"

void delay_ms ( unsigned int n );
void delay ( unsigned int m ); //—” ±≥Ã–Ú£¨Œ¢√Óº∂
void delay1us ( void ); //ŒÛ≤Ó -0.005353009259us
void delay10us ( void );
void delay1Ms ( void );
void delay5Ms ( void );
void Delay100ms ( void );
void delay400Ms ( void );

#endif