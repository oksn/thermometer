
#include "delay.h"
#include "intrins.h"

void delay_ms ( unsigned int n )
{
    unsigned char a, b;
    while ( n-- )
    {
        for ( b = 133; b > 0; b-- )
            for ( a = 6; a > 0; a-- );
    }
}

void delay ( unsigned int N ) //—” ±≥Ã–Ú£¨Œ¢√Óº∂
{
    unsigned int i;
    for ( i = 0; i < N; i++ )
        _nop_();
}
void delay1us ( void ) //ŒÛ≤Ó -0.005353009259us
{
    _nop_();
    _nop_();
    _nop_();
}
void delay10us ( void ) //ŒÛ≤Ó -0.005353009259us
{
    unsigned char i;
    _nop_();
    i = 25;
    while ( --i )
    {
        ;
    }
}
void delay1Ms ( void ) //ŒÛ≤Ó -0.005353009259us
{
    unsigned char i, j;
    _nop_();
    _nop_();
    _nop_();
    i = 11;
    j = 190;
    do
    {
        while ( --j )
        {
            ;
        }
    }
    while ( --i );
}
/***********∂Ã—” ±********************************************************/
void delay5Ms ( void )
{
    unsigned char i, j;
    i = 54;
    j = 199;
    WDT_CONTR = 0x3f;
    do
    {
        while ( --j )
        {
            ;
        }
    }
    while ( --i );
}
/***********≥§—” ±********************************************************/
void Delay100ms()       //@11.0592MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 5;
    j = 52;
    k = 195;
    do
    {
        do
        {
            while ( --k );
        }
        while ( --j );
    }
    while ( --i );
}



void delay400Ms ( void )
{
    unsigned char i, j, k;
    _nop_();
    _nop_();
    i = 17;
    j = 208;
    k = 27;
    do
    {
        do
        {
            while ( --k )
            {
                ;
            }
        }
        while ( --j );
    }
    while ( --i );
}