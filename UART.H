/*
02. * 模 块 名：UART.H
03. * 芯    片：STC12C5A60S2
04. * 晶    振：12MHz
05. * 创 建 者：冷月
06. * 创建日期：2010.8.16
07. * 修 改 者：
08. * 修改日期：
09. * 功能描述：STC12C5A系列单片机串口模块，包括串口1和串口2,方式1,使用独立波特率发生器
10. *           可设置串口波特率
11. *           功能：1.发送一个字符；2.发送一个字符串；3.接收一个字符。
12. */
#ifndef _UART_H_
#define _UART_H_

#define uchar   unsigned char
#define uint    unsigned int

//定义串口1口开关，关闭则不能接收数据
#define OpenUART1()     ES=1
#define CloseUART1()    ES=0
#define OpenUART2()     IE2|=0x01
#define CloseUART2()    IE2&=0xFE

//缓存串口1和串口2接收到的字符
extern uchar UART1_Recv_Val;
extern uchar UART2_Recv_Val;
extern uchar CO2_data[9];



/*
 * 函 数 名：UART1_Init
 * 调    用：UART1_Init(0xD9, 0, 0);
 *           BRT=OxD9，波特率不加倍,独立波特率发生器12T模式，Fosc = 12MHz, Baud0 = 9600
 *           UART1_Init(0xB2, 1, 1);
 *           BRT=0xB2，波特率加倍,独立波特率发生器1T模式,Fosc = 12MHz, Baud0 = 9600
 * 功能描述：串口1初始化
 */
void UART1_Init ( uchar RELOAD, bit doubleBaud, bit timeMod );


/*
 * 函 数 名：UART2_Init
 * 调    用：UART2_Init(0xD9, 0, 0);
 *           BRT=OxD9，波特率不加倍,独立波特率发生器12T模式，Fosc = 12MHz, Baud0 = 9600
 *           UART2_Init(0xB2, 1, 1);
*           BRT=0xB2，波特率加倍,独立波特率发生器1T模式,Fosc = 12MHz, Baud0 = 9600
 * 功能描述：串口2初始化
 */
void UART2_Init ( uchar RELOAD, bit doubleBaud, bit timeMod );

/*
 * 函 数 名：UART1_SendOneChar
 * 调    用：UART1_SendOneChar('A');
 * 功能描述：串口1发送一个字符
 */
void UART1_SendOneChar ( uchar val );

/*
62. * 函 数 名：UART2_SendOneChar
63. * 调    用：UART2_SendOneChar('A');
64. * 功能描述：串口2发送一个字符
 */
void UART2_SendOneChar ( uchar val );
/*
70. * 函 数 名：UART1_SendStr
71. * 调    用：UART1_SendStr("MCU");
72. * 功能描述：串口1发送字符串
 */
void UART1_SendStr ( uchar *str );

/*
 * 函 数 名：UART2_SendStr
79. * 调    用：UART2_SendStr("MCU");
80. * 功能描述：串口2发送字符串
 */
void UART2_SendStr ( uchar *str );
#endif
