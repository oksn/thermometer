#include <uart.h>
#include <register.h>
#define FOSC      11059200L
#define BAUD    9600

//extern

//独立波特率发生器初值,1T模式
//Fosc = 晶振频率, Baud0 = 标准波特率
//RELOAD = 256 - INT(Fosc/Baud0/32 + 0.5)
//Baud = Fosc/(256 - RELOAD)/32
//error = (Baud - Baud0)/Baud0 * 100%

uchar RELOAD = 0xdc;                    //Fosc = 12MHz, Baud0 = 9600
uchar UART1_Recv_Val = 0;
//uchar UART2_Recv_Val = 0;

extern unsigned  int idata js;
uchar flag1 = 0;
extern uchar flag2;
//extern unsigned  int idata js;
extern unsigned char UART2_len;
extern unsigned char UART2_Rec[150];
//波特率是否加倍，0不倍，1加倍
bit doubleBaud = 0;

//独立波特率发生器,0为12T模式，1为1T模式
bit timeMod = 1;



/*
 * 函 数 名：UART1_Init
27. * 功能描述：串口1初始化
28. * 输入参数：RELOAD:BRT初值；
29. *           doubleBaud:0波特率不加倍，1波特率加倍
30. *           timeMod:0独立波特率发生器12T模式，1为1T模式
31. * 返 回 值：无
 */
void UART1_Init ( uchar RELOAD, bit doubleBaud, bit timeMod )
{
    SCON |= 0x50;       //串口1方式1,接收充许

    BRT = RELOAD;       //波特率2400

    if ( timeMod == 1 )     //1T
    {
        //T0x12   T1x12   UM0x6   BRTR    S2SMOD  BRTx12  EXTRAM  S1BRS
        AUXR |= 0x15;       //串口1使用独立波特率发生器，独立波特率发生器1T
    }
    else                    //12T
    {
        AUXR |= 0x11;
    }

    if ( doubleBaud == 1 )
    {
        PCON |= 0x80;     //波特率加倍
    }
    else
    {
        PCON &= 0x7F;     //波特率不加倍
    }

    EA = 1;
    ES = 1;             //充许串口1中断
}


/*
 * 函 数 名：UART2_Init
 * 功能描述：串口2初始化
 * 输入参数：RELOAD:BRT初值；
 *           doubleBaud:0波特率不加倍，1波特率加倍
 *           timeMod:0独立波特率发生器12T模式，1为1T模式
 * 返 回 值：无
 */
void UART2_Init ( uchar RELOAD, bit doubleBaud, bit timeMod )
{
    //S2SM0  S2SM1   S2SM2   S2REN   S2TB8   S2RB8   S2TI     S2RI
    S2CON |= 0x50;      //串口2,方式1，接收充许

    BRT = RELOAD;

    if ( timeMod == 1 )     //1T
    {
        //T0x12   T1x12   UM0x6   BRTR    S2SMOD  BRTx12  EXTRAM  S1BRS
        AUXR |= 0x14;       //串口1使用独立波特率发生器，独立波特率发生器1T
    }
    else                    //12T
    {
        AUXR = ( AUXR | 0x10 ) & 0xFB;
    }

    if ( doubleBaud == 1 )
    {
        AUXR |= 0x08;       //波特率加倍
    }
    else
    {
        AUXR &= 0xF7;       //波特率不加倍
    }
    AUXR1 |= 0x10;   // 将串口2切换到P4口
    EA = 1;
    //-       -       -       -       -       -       ESPI    ES2
    IE2 |= 0x01;            //充许串口2中断
}


/*
 * 函 数 名：UART1_SendOneChar
 * 功能描述：串口1发送一个字符
 * 输入参数：val:要发送的字符
 * 返 回 值：无
 */
void UART1_SendOneChar ( uchar val )
{
    ES = 0;                   //关闭串口1中断

    SBUF = val;
    while ( TI == 0 );
    TI = 0;

    ES = 1;                  //恢复串口1中断
}


/*
 * 函 数 名：UART2_SendOneChar
 * 功能描述：串口2发送一个字符
 * 输入参数：val:要发送的字符
 * 返 回 值：无
 */
void UART2_SendOneChar ( uchar val )
{
    IE2 = 0x00;                 //关闭串口2中断

    S2BUF = val;
    while ( ( S2CON & 0x02 ) == 0 );
    S2CON &= 0xFD;

    IE2 = 0x01;                //恢复串口2中断
}


/*
 * 函 数 名：UART1_SendStr
 * 功能描述：串口1发送字符串
 * 输入参数：str:指向要发送的字符串的指针
 * 返 回 值：无
 */
void UART1_SendStr ( uchar *str )
{
    while ( ( *str ) != '\0' )
    {
        UART1_SendOneChar ( *str );
        str++;
    }
}


/*
 * 函 数 名：UART2_SendStr
 * 功能描述：串口2发送字符串
 * 输入参数：str:指向要发送的字符串的指针
 * 返 回 值：无
 */
void UART2_SendStr ( uchar *str )
{
    while ( ( *str ) != '\0' )
    {
        UART2_SendOneChar ( *str );
        str++;
    }
}

/*
172. * 函 数 名：UART1_Int
173. * 功能描述：串口1中断服务程序，接收串口1字符
174. * 输入参数：无
175. * 返 回 值：无
 */
void UART1_Int ( void ) interrupt 4
{

    if ( RI == 1 )
    {
        RI = 0;
        UART1_Recv_Val = SBUF;
        CO2_data[js] = UART1_Recv_Val;
        flag1 = 1;
        if ( js >= 1 )
        {
            if  ( ( CO2_data[js-1] == 0xff ) && ( CO2_data[js] == 0x86 ) )
            {
                js = 1;
            }
        }
        js++;
        if  ( js >= 9 )
        {
            js = 0;
        }
        //          UART2_SendOneChar( UART1_Recv_Val);
    }
}

/*
 * 函 数 名：UART2_Int
 * 功能描述：串口2中断服务程序，接收串口2字符
 * 输入参数：无
 * 返 回 值：无
 */
void UART2_Int ( void ) interrupt 8
{


//  idata unsigned  char indata[20];
    if ( ( S2CON & 0x01 ) == 1 )
    {
        S2CON &= 0xFE;
        //   flag2 = 0;
        UART2_Rec[UART2_len] = S2BUF;
       // UART1_SendOneChar ( S2BUF );
        if ( UART2_len >= 150 )
        {
            UART2_len = 0;
        }
        else
        {

            if  ( UART2_len > 1 ) //判断接收长度不为0的情况
            {
                if  ( ( UART2_Rec[UART2_len-1] == 0x0d ) && ( UART2_Rec[UART2_len] == 0x0a ) )
                {
                    flag2++;   //计算接收回车换行的次数
                }
            }

            UART2_len++;
//           flag2=3;
        }
  //     UART1_SendOneChar ( S2BUF );
    }




}
