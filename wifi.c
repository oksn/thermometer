#include <stdio.h>
#include <reg52.h>
#include <intrins.h>
#include <wifi.h>
#include <delay.h>
#include <string.h>
#include <uart.h>
extern uchar flag2;
extern unsigned char UART2_len;
extern unsigned char UART2_Rec[150];
extern xdata unsigned char ip_adr[16];
unsigned char seek_cr, reseek;
unsigned int timeover;
unsigned char seekdot;
sbit  LED_ready = P3 ^ 2;
sbit  LED_link = P3 ^ 3;
sbit  LED_serial = P1 ^ 0;

uchar  tcp_link ( void )
{
    bit tcp_link_stats;
    flag2 = 0;
    UART2_len = 0;
    UART2_SendOneChar ( 0x2b );
    UART2_SendOneChar ( 0x2b );
    UART2_SendOneChar ( 0x2b );

    flag2 = 0;
    UART2_len = 0;
    timeover = 0;
    delay400Ms();
    UART2_SendOneChar ( 'a' );

    do
    {
        delay5Ms();
        timeover++;
        if ( timeover == 350 )
        {
            tcp_link_stats = 2;
            break;
        }
    }
    while ( ! ( flag2 == 2 ) );

    flag2 = 0;
    UART2_len = 0;
    timeover = 0;
    //       if (UART2_Rec[0]=='a') UART2_len=0;

    UART2_SendStr ( "AT+TCPLK\r\n" );


    do
    {
        delay5Ms();
        timeover++;
        if ( timeover == 350 ) break;
    }
    while ( ! ( flag2 == 3 ) );


    for ( seek_cr = 1; seek_cr < UART2_len - 2; seek_cr++ )
    {
        if ( ( UART2_Rec[seek_cr-1] == 0x0d ) && ( UART2_Rec[seek_cr] == 0x0a ) ) // 将返回的字符串变为一行,去除前面的换行
        {
            UART2_Rec[seek_cr-1] = 0x2d;
            UART2_Rec[seek_cr] = 0x2d;
            UART2_Rec[seek_cr+1] = 0x2d;
            break;
        }

    }
    if ( ( strstr ( UART2_Rec, "+ok=on" ) > 0 ) )
    {
        //  DisplayListChar ( 0, 4, "TCP link is on  " );
        tcp_link_stats = 1;
    }
    else
    {
        tcp_link_stats = 0;
    }

    flag2 = 0;
    UART2_len = 0;
    timeover = 0;
    UART2_SendStr ( "AT+ENTM\r\n" );
    do
    {
        delay5Ms();
        timeover++;
        if ( timeover == 350 )
        {
            tcp_link_stats = 3;
            break;
        }
    }
    while ( ! ( flag2 == 3 ) );
    return tcp_link_stats;
}

void ip_add ( void )
{
    flag2 = 0;
    UART2_len = 0;
    UART2_SendOneChar ( 0x2b );
    UART2_SendOneChar ( 0x2b );
    UART2_SendOneChar ( 0x2b );

    flag2 = 0;
    UART2_len = 0;
    timeover = 0;
    delay400Ms();
    UART2_SendOneChar ( 'a' );

    do
    {
        delay5Ms();
        timeover++;
        if ( timeover == 350 )
        {
            break;
        }
    }
    while ( ! ( flag2 == 2 ) );

    flag2 = 0;
    UART2_len = 0;
    timeover = 0;

    UART2_SendStr ( "AT+WANN\r\n" );


    do
    {
        delay5Ms();
        timeover++;
        if ( timeover == 350 ) break;
    }
    while ( ! ( flag2 == 3 ) );

    strncpy ( ip_adr, UART2_Rec + 19, 16 );
    seekdot = 0;
    for ( seek_cr = 1; seek_cr < 16; seek_cr++ ) //在字符串中查找指定字符的位置
    {
        if ( ip_adr[seek_cr] == 0x2c ) break;
        seekdot++;
    }
    for ( seek_cr = seekdot + 1; seek_cr < 16; seek_cr++ ) //去除以后的字符
    {
        ip_adr[seek_cr] = 0x20;
    }


    flag2 = 0;
    UART2_len = 0;
    timeover = 0;
    UART2_SendStr ( "AT+ENTM\r\n" );
    do
    {
        delay5Ms();
        timeover++;
        if ( timeover == 350 )
        {
            break;
        }
    }
    while ( ! ( flag2 == 3 ) );

}

void wifi_reboot ( void )
{
    flag2 = 0;
    UART2_len = 0;
    UART2_SendOneChar ( 0x2b );
    UART2_SendOneChar ( 0x2b );
    UART2_SendOneChar ( 0x2b );

    flag2 = 0;
    UART2_len = 0;
    timeover = 0;
    delay400Ms();
    UART2_SendOneChar ( 'a' );

    do
    {
        delay5Ms();
        timeover++;
        if ( timeover == 350 )
        {
            break;
        }
    }
    while ( ! ( flag2 == 2 ) );

    flag2 = 0;
    UART2_len = 0;
    timeover = 0;

    UART2_SendStr ( "AT+Z\r\n" );

}


void WSLQ ( void )
{
    flag2 = 0;
    UART2_len = 0;
    UART2_SendOneChar ( 0x2b );
    UART2_SendOneChar ( 0x2b );
    UART2_SendOneChar ( 0x2b );

    flag2 = 0;
    UART2_len = 0;
    timeover = 0;
    delay400Ms();
    UART2_SendOneChar ( 'a' );

    do
    {
        delay5Ms();
        timeover++;
        if ( timeover == 350 )
        {
            break;
        }
    }
    while ( ! ( flag2 == 2 ) );
    for ( seek_cr = 0; seek_cr < UART2_len; seek_cr++ )  //清除接收区
    {
        UART2_Rec[seek_cr] = 0x2c;
    }
    flag2 = 0;
    UART2_len = 0;
    timeover = 0;

    UART2_SendStr ( "AT+WSLQ\r\n" );


    do
    {
        delay5Ms();
        timeover++;
        if ( timeover == 350 ) break;
    }
    while ( ! ( flag2 == 3 ) );

    for ( seek_cr = 1; seek_cr < UART2_len - 2; seek_cr++ )
    {
        if ( ( UART2_Rec[seek_cr-1] == 0x0d ) && ( UART2_Rec[seek_cr] == 0x0a ) ) // 将返回的字符串变为一行,去除前面的换行
        {
            UART2_Rec[seek_cr-1] = 0x2d;
            UART2_Rec[seek_cr] = 0x2d;
            UART2_Rec[seek_cr+1] = 0x2d;
            break;
        }

    }
    if ( ( strstr ( UART2_Rec, "+ok=" ) > 0 ) )
    {
        //  DisplayListChar ( 0, 4, "TCP link is on  " );
   strncpy ( ip_adr,  "                ", 16 );
  
    strncpy ( ip_adr, UART2_Rec + 10,7);
   



    }
    else
    {
        strncpy ( ip_adr, "Error!", 16 );
    }



    flag2 = 0;
    UART2_len = 0;
    timeover = 0;
    UART2_SendStr ( "AT+ENTM\r\n" );
    do
    {
        delay5Ms();
        timeover++;
        if ( timeover == 350 )
        {
            break;
        }
    }
    while ( ! ( flag2 == 3 ) );

}
