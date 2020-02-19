#include "stc12c5a60s2.h"
#include <intrins.h>
#include <stdio.h>
#include  "config.h"
#include "delay.h"
#include "uart.h"
#include "oled.h"
#include "bmp.h"
#include "wifi.h"
#include "mlx90614.h"
xdata unsigned char UART2_Rec[150];
xdata unsigned char UART2_len = 0;
extern bit doubleBaud;
extern bit timeMod;
extern uchar RELOAD;
//xdata unsigned char  LCD_linne2[]="                ";
unsigned char  LCD_linne1[] = "                ";
unsigned char  LCD_linne2[] = "                ";
unsigned char  LCD_linne3[] = "                ";
unsigned char  LCD_linne4[] = "                ";
unsigned  int idata js = 0, thisLength;
uchar flag2, tcp;
idata float adc, adcout;
sbit WIFI_Ready = P3 ^ 2;
sbit WIFI_Link = P3 ^ 3;
unsigned int  Tem;	   //�¶�
float objTemp,envTemp;	 //�����¶Ⱥͻ����¶�


void main ( void )
{


    UART1_Init ( RELOAD, doubleBaud, timeMod );
    UART2_Init ( RELOAD, doubleBaud, timeMod );
    OLED_Init();            //��ʼ��OLED
    OLED_Clear();
    OLED_DrawBMP ( 0, 0, 127, 7, BMP1 );
    delay400Ms ();
    OLED_Clear();
    // B_LUX_Init();
 //    WDT_CONTR = 0x3f;
	OLED_Init();            //��ʼ��OLED
    OLED_Clear();
   
    while ( 1 )
    {

            delay5Ms();
            delay5Ms();
            delay400Ms;


//			sprintf(dis0,"E: %4.1f'C   ",envTemp);//��ӡ�¶�ֵ
//			LCD_Write_String(0,0,dis0);//��ʾ
//			sprintf(dis1,"O:%5.1f'C   %3d  ",objTemp,setNum);//��ӡ�¶�����ֵ
//			LCD_Write_String(0,1,dis1);//��ʾ

              
             
            OLED_ShowCHinese ( 0, 0, 0 ); //��
            OLED_ShowCHinese ( 16, 0, 1 ); //��
            OLED_ShowCHinese ( 32, 0, 2 ); //ʪ
            OLED_ShowCHinese ( 48, 0, 3 ); //��
            OLED_ShowCHinese ( 64, 0, 4 ); //��
            OLED_ShowCHinese ( 80, 0, 5 ); //��
            OLED_ShowCHinese ( 96, 0, 6 ); //��
            OLED_ShowCHinese ( 112, 0, 7 ); //��

            //    OLED_ShowString ( 0, 2, LCD_linne1 );
			
	
			Tem=ReadEnvironTemp();	 //��ȡ�����¶�
    		envTemp=(float)(Tem)*0.02-273.15;
           Tem=ReadObjectTemp();	 //��ȡʵ���¶�
			//Tem=16000;
			objTemp=(float)(Tem)*0.02-273.15;
               sprintf ( LCD_linne3, "WD1: %5.1f C", envTemp);
			//	  adc = CO2_data[2] * 256 + CO2_data[3];
                //  adc=986;
                sprintf ( LCD_linne4, "WD2: %5.1f C", objTemp);
        
              	OLED_ShowString ( 0, 4, LCD_linne3 );
                OLED_ShowString ( 0, 6, LCD_linne4 );

       // WDT_CONTR = 0x3f;
    }
}
