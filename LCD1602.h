#ifndef __LCD1602_H__
#define __LCD1602_H__

#include <reg52.h>

#define uint8 unsigned char
#define uint16 unsigned int

#define LCD_DATA P0 //Êý¾Ý¿Ú

sbit RS = P2 ^ 7;
sbit RW = P2 ^ 6;
sbit EN = P2 ^ 5;
sbit BUSY = LCD_DATA ^ 7;

void LCD1602_init();
void clean_screen();
void write_string ( uint8 x, uint8 y, uint8 *p );

#endif