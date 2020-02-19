
#include <intrins.h>
#include "LCD1602.h"

/*********************************************************************
 * 函数名称：wait()
 * 功    能：等待1602繁忙
 * 入口参数：无
 * 出口参数：无
 * 返 回 值：无
 ********************************************************************/
void wait()
{
    do
    {
        LCD_DATA = 0xFF;
        RS = 0;
        RW = 1;
        EN = 0;
        EN = 1;
    }
    while ( BUSY );
    EN = 0;
}

/*********************************************************************
 * 函数名称：write_dat(uint8 dat)
 * 功    能：向1602写数据
 * 入口参数：数据值
 * 出口参数：无
 * 返 回 值：无
 ********************************************************************/
void write_dat ( uint8 dat )
{
    wait();
    EN = 0;
    RS = 1;
    RW = 0;
    LCD_DATA = dat;
    EN = 1;
    EN = 0;
}

/*********************************************************************
 * 函数名称：write_cmd(uint8 cmd)
 * 功    能：向1602写指令
 * 入口参数：指令值
 * 出口参数：无
 * 返 回 值：无
 ********************************************************************/
void write_cmd ( uint8 cmd )
{
    wait();
    EN = 0;
    RS = 0;
    RW = 0;
    LCD_DATA = cmd;
    EN = 1;
    EN = 0;
}

/*********************************************************************
 * 函数名称：LCD1602_init()
 * 功    能：1602初始化
 * 入口参数：无
 * 出口参数：无
 * 返 回 值：无
 ********************************************************************/
void LCD1602_init()
{
    write_cmd ( 0x38 );           // 16*2显示，5*7点阵，8位数据接口
    write_cmd ( 0x0C );           // 显示器开、光标开、光标不允许闪烁
    write_cmd ( 0x06 );           // 文字不动，光标自动右移
    write_cmd ( 0x01 );           // 清屏
}

/*********************************************************************
 * 函数名称：write_string(uint8 x,uint8 y,uint8 *p)
 * 功    能：向1602写字符串
 * 入口参数：x    行坐标
 *           y    列坐标
 *           *p   指向字符串的指针
 * 出口参数：无
 * 返 回 值：无
 ********************************************************************/
void write_string ( uint8 x, uint8 y, uint8 *p )
{
    uint8 add_start;
    if ( x == 0 )
    {
        add_start = 0x80;
    }
    else if ( x == 1 )
    {
        add_start = 0xC0;
    }
    else
    {
        return;
    }
    write_cmd ( add_start + y );
    while ( *p != '\0' )
    {
        write_dat ( *p );
        p++;
    }
}

/*********************************************************************
 * 函数名称：clean_screen()
 * 功    能：清屏
 * 入口参数：无
 * 出口参数：无
 * 返 回 值：无
 ********************************************************************/
void clean_screen()
{
    write_cmd ( 0x01 );
}