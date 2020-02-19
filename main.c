#include "stc12c5a60s2.h"
#include "MLX90615.h"
#include "delay.h"
#include "stdio.h"
#include "oled.h"
#include "oled.h"
#include "bmp.h"
#include "uart.h"

unsigned char s[20];  // 待显示的字符串
float temp;           // 温度值
unsigned long int Tem;     //温度
float objTemp, envTemp;  //物体温度和环境温度

extern bit doubleBaud;
extern bit timeMod;
extern unsigned char RELOAD;
xdata unsigned char UART2_Rec[150];
xdata unsigned char UART2_len = 0;

void main()
{
    unsigned char RunStatus = 0;        // 是否测量标志
    unsigned char SlaveAddress;               // Contains device address
    unsigned char   command;                    // Contains the access command
//    unsigned int  tdata;                      // Contains data value
    unsigned char  LCD_linne1[] = "                ";
    unsigned char  LCD_linne2[] = "                ";
    unsigned char  LCD_linne3[] = "                ";
    unsigned char  LCD_linne4[] = "                ";
    UART1_Init ( RELOAD, doubleBaud, timeMod );
    UART2_Init ( RELOAD, doubleBaud, timeMod );
    SlaveAddress = SA << 1;                       // Set device address
    command = RAM_Access | RAM_To;      // Form RAM access command + RAM address

    //LEDG = 0;
    MLX90615_init();
    //LCD1602_init();
    OLED_Init();            //初始化OLED
    OLED_Clear();
    OLED_DrawBMP ( 0, 0, 127, 7, BMP1 );
    delay400Ms ();
    OLED_Clear();
    // B_LUX_Init();
//    WDT_CONTR = 0x3f;
    OLED_Init();            //初始化OLED
    OLED_Clear();


    OLED_ShowCHinese ( 0, 0, 0 ); //温
    OLED_ShowCHinese ( 16, 0, 1 ); //度
    OLED_ShowCHinese ( 32, 0, 2 ); //湿
    OLED_ShowCHinese ( 48, 0, 3 ); //度
    OLED_ShowCHinese ( 64, 0, 4 ); //氨
    OLED_ShowCHinese ( 80, 0, 5 ); //气
    OLED_ShowCHinese ( 96, 0, 6 ); //采
    OLED_ShowCHinese ( 112, 0, 7 ); //集
    while ( 1 )
    {

        Tem = MemRead ( 0, 0x26 ); //读取环境温度
        envTemp = ( float ) ( Tem ) * 0.02 - 273.15;
        Tem = MemRead ( 0, 0x27 ); //读取实物温度
        //Tem=16000;
        objTemp = ( float ) ( Tem ) * 0.02 - 273.15;
        sprintf ( LCD_linne3, "WD1: %5.1f C", envTemp );
        //    adc = CO2_data[2] * 256 + CO2_data[3];
        //  adc=986;
        sprintf ( LCD_linne4, "WD2: %5.1f C", objTemp );

        OLED_ShowString ( 0, 4, LCD_linne3 );
        OLED_ShowString ( 0, 6, LCD_linne4 );
        delay400Ms ( );
        delay400Ms ( );
    }
}