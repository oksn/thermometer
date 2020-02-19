
#include <intrins.h>
#include "LCD1602.h"

/*********************************************************************
 * �������ƣ�wait()
 * ��    �ܣ��ȴ�1602��æ
 * ��ڲ�������
 * ���ڲ�������
 * �� �� ֵ����
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
 * �������ƣ�write_dat(uint8 dat)
 * ��    �ܣ���1602д����
 * ��ڲ���������ֵ
 * ���ڲ�������
 * �� �� ֵ����
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
 * �������ƣ�write_cmd(uint8 cmd)
 * ��    �ܣ���1602дָ��
 * ��ڲ�����ָ��ֵ
 * ���ڲ�������
 * �� �� ֵ����
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
 * �������ƣ�LCD1602_init()
 * ��    �ܣ�1602��ʼ��
 * ��ڲ�������
 * ���ڲ�������
 * �� �� ֵ����
 ********************************************************************/
void LCD1602_init()
{
    write_cmd ( 0x38 );           // 16*2��ʾ��5*7����8λ���ݽӿ�
    write_cmd ( 0x0C );           // ��ʾ��������꿪����겻������˸
    write_cmd ( 0x06 );           // ���ֲ���������Զ�����
    write_cmd ( 0x01 );           // ����
}

/*********************************************************************
 * �������ƣ�write_string(uint8 x,uint8 y,uint8 *p)
 * ��    �ܣ���1602д�ַ���
 * ��ڲ�����x    ������
 *           y    ������
 *           *p   ָ���ַ�����ָ��
 * ���ڲ�������
 * �� �� ֵ����
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
 * �������ƣ�clean_screen()
 * ��    �ܣ�����
 * ��ڲ�������
 * ���ڲ�������
 * �� �� ֵ����
 ********************************************************************/
void clean_screen()
{
    write_cmd ( 0x01 );
}