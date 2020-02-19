#include <uart.h>
#include <register.h>
#define FOSC      11059200L
#define BAUD    9600

//extern

//���������ʷ�������ֵ,1Tģʽ
//Fosc = ����Ƶ��, Baud0 = ��׼������
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
//�������Ƿ�ӱ���0������1�ӱ�
bit doubleBaud = 0;

//���������ʷ�����,0Ϊ12Tģʽ��1Ϊ1Tģʽ
bit timeMod = 1;



/*
 * �� �� ����UART1_Init
27. * ��������������1��ʼ��
28. * ���������RELOAD:BRT��ֵ��
29. *           doubleBaud:0�����ʲ��ӱ���1�����ʼӱ�
30. *           timeMod:0���������ʷ�����12Tģʽ��1Ϊ1Tģʽ
31. * �� �� ֵ����
 */
void UART1_Init ( uchar RELOAD, bit doubleBaud, bit timeMod )
{
    SCON |= 0x50;       //����1��ʽ1,���ճ���

    BRT = RELOAD;       //������2400

    if ( timeMod == 1 )     //1T
    {
        //T0x12   T1x12   UM0x6   BRTR    S2SMOD  BRTx12  EXTRAM  S1BRS
        AUXR |= 0x15;       //����1ʹ�ö��������ʷ����������������ʷ�����1T
    }
    else                    //12T
    {
        AUXR |= 0x11;
    }

    if ( doubleBaud == 1 )
    {
        PCON |= 0x80;     //�����ʼӱ�
    }
    else
    {
        PCON &= 0x7F;     //�����ʲ��ӱ�
    }

    EA = 1;
    ES = 1;             //������1�ж�
}


/*
 * �� �� ����UART2_Init
 * ��������������2��ʼ��
 * ���������RELOAD:BRT��ֵ��
 *           doubleBaud:0�����ʲ��ӱ���1�����ʼӱ�
 *           timeMod:0���������ʷ�����12Tģʽ��1Ϊ1Tģʽ
 * �� �� ֵ����
 */
void UART2_Init ( uchar RELOAD, bit doubleBaud, bit timeMod )
{
    //S2SM0  S2SM1   S2SM2   S2REN   S2TB8   S2RB8   S2TI     S2RI
    S2CON |= 0x50;      //����2,��ʽ1�����ճ���

    BRT = RELOAD;

    if ( timeMod == 1 )     //1T
    {
        //T0x12   T1x12   UM0x6   BRTR    S2SMOD  BRTx12  EXTRAM  S1BRS
        AUXR |= 0x14;       //����1ʹ�ö��������ʷ����������������ʷ�����1T
    }
    else                    //12T
    {
        AUXR = ( AUXR | 0x10 ) & 0xFB;
    }

    if ( doubleBaud == 1 )
    {
        AUXR |= 0x08;       //�����ʼӱ�
    }
    else
    {
        AUXR &= 0xF7;       //�����ʲ��ӱ�
    }
    AUXR1 |= 0x10;   // ������2�л���P4��
    EA = 1;
    //-       -       -       -       -       -       ESPI    ES2
    IE2 |= 0x01;            //������2�ж�
}


/*
 * �� �� ����UART1_SendOneChar
 * ��������������1����һ���ַ�
 * ���������val:Ҫ���͵��ַ�
 * �� �� ֵ����
 */
void UART1_SendOneChar ( uchar val )
{
    ES = 0;                   //�رմ���1�ж�

    SBUF = val;
    while ( TI == 0 );
    TI = 0;

    ES = 1;                  //�ָ�����1�ж�
}


/*
 * �� �� ����UART2_SendOneChar
 * ��������������2����һ���ַ�
 * ���������val:Ҫ���͵��ַ�
 * �� �� ֵ����
 */
void UART2_SendOneChar ( uchar val )
{
    IE2 = 0x00;                 //�رմ���2�ж�

    S2BUF = val;
    while ( ( S2CON & 0x02 ) == 0 );
    S2CON &= 0xFD;

    IE2 = 0x01;                //�ָ�����2�ж�
}


/*
 * �� �� ����UART1_SendStr
 * ��������������1�����ַ���
 * ���������str:ָ��Ҫ���͵��ַ�����ָ��
 * �� �� ֵ����
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
 * �� �� ����UART2_SendStr
 * ��������������2�����ַ���
 * ���������str:ָ��Ҫ���͵��ַ�����ָ��
 * �� �� ֵ����
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
172. * �� �� ����UART1_Int
173. * ��������������1�жϷ�����򣬽��մ���1�ַ�
174. * �����������
175. * �� �� ֵ����
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
 * �� �� ����UART2_Int
 * ��������������2�жϷ�����򣬽��մ���2�ַ�
 * �����������
 * �� �� ֵ����
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

            if  ( UART2_len > 1 ) //�жϽ��ճ��Ȳ�Ϊ0�����
            {
                if  ( ( UART2_Rec[UART2_len-1] == 0x0d ) && ( UART2_Rec[UART2_len] == 0x0a ) )
                {
                    flag2++;   //������ջس����еĴ���
                }
            }

            UART2_len++;
//           flag2=3;
        }
  //     UART1_SendOneChar ( S2BUF );
    }




}
