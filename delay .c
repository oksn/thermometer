#include <delay.h>
#include <intrins.h>
#include "stc12c5a60s2.h"
void delay ( unsigned int m ) //��ʱ����΢�
{
    while ( m-- )
    {
        _nop_();
        _nop_();
        _nop_();
    }
}
void delay1us ( void ) //��� -0.005353009259us
{
    _nop_();
    _nop_();
    _nop_();
}
void delay10us ( void ) //��� -0.005353009259us
{
    unsigned char i;
    _nop_();
    i = 25;
    while ( --i )
    {
        ;
    }
}
void delay1Ms ( void ) //��� -0.005353009259us
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
/***********����ʱ********************************************************/
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
/***********����ʱ********************************************************/
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
