#include  "config.h"
#include "mlx90614.h"


#define Nack_counter 10

//************数据定义****************


//mlx90614 端口定义
sbit SCL=P0^7;//时钟线
sbit SDA=P0^6;//数据线

unsigned char bit_out=1;
unsigned char bit_in=1;

unsigned char DataH,DataL,Pecreg;

void start_bit(void)
{
	SDA=1;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	SCL=1;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	SDA=0;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	SCL=0;
	_nop_();_nop_();_nop_();_nop_();_nop_();
}

void stop_bit(void)
{
	SCL=0;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	SDA=0;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	SCL=1;
	_nop_();_nop_();_nop_();_nop_();_nop_();
	SDA=1;
}


//---------发送一个字节---------
void tx_byte(unsigned char dat_byte)
{
	char i,n,dat;
	n=Nack_counter;

	dat=dat_byte;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)
		bit_out=1;
		else
		bit_out=0;
		send_bit();
		dat=dat<<1;
	}
	receive_bit();
	if(bit_in==1)
	{
		stop_bit();
		if(n!=0)
		{n--;}
		else
			return;
	}
	else
		return;
	start_bit();
	tx_byte(dat_byte);		//函数自身回调
}

////---------发送一个字节---------
//void tx_byte(unsigned char dat_byte)
//{
//	char i,n,dat;
//	n=Nack_counter;
//	TX_again:
//	dat=dat_byte;
//	for(i=0;i<8;i++)
//	{
//		if(dat&0x80)
//		bit_out=1;
//		else
//		bit_out=0;
//		send_bit();
//		dat=dat<<1;
//	}
//	receive_bit();
//	if(bit_in==1)
//	{
//		stop_bit();
//		if(n!=0)
//		{n--;goto Repeat;}
//		else
//		goto exit;
//	}
//	else
//	goto exit;
//	Repeat:
//	start_bit();
//	goto TX_again;
//	exit: ;
//}
//-----------发送一个位---------
void send_bit(void)
{
	if(bit_out==0)
		SDA=0;
	else
		SDA=1;
	_nop_();
	SCL=1;
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	SCL=0;
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
}
//----------接收一个字节--------
unsigned char rx_byte(void)
{
	unsigned char i,dat;
	dat=0;
	for(i=0;i<8;i++)
	{
		dat=dat<<1;
		receive_bit();
		if(bit_in==1)
		dat=dat+1;
	}
	send_bit();
	return dat;
}
//----------接收一个位----------
void receive_bit(void)
{
	SDA=1;bit_in=1;
	SCL=1;
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
	bit_in=SDA;
	_nop_();
	SCL=0;
	_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();
}

//------------------------------
unsigned int ReadObjectTemp(void)
{
	start_bit();
	tx_byte(0x00); //Send SlaveAddress
	tx_byte(0x27); //Send Command
	start_bit();
	tx_byte(0x01);
	bit_out=0;
	DataL=rx_byte();
	bit_out=0;
	DataH=rx_byte();
	bit_out=1;
	Pecreg=rx_byte();
	stop_bit();
	return(DataH*256+DataL);
}

unsigned int ReadEnvironTemp(void)
{
	start_bit();
	tx_byte(0x00); //Send SlaveAddress
	tx_byte(0x26); //Send Command
	start_bit();
	tx_byte(0x01);
	bit_out=0;
	DataL=rx_byte();
	bit_out=0;
	DataH=rx_byte();
	bit_out=1;
	Pecreg=rx_byte();
	stop_bit();
	return(DataH*256+DataL);
}


//---------------------------------------
//Name: CALTEMP           
//Temperature data is T=(Data)*0.02-273.15

//Tem=ReadObjectTemp();
//objTemp=(float)(Tem)*0.02-273.15;
//sprintf(tab_up,"O:%5.2f %05d ",objTemp,Tem);//打印温度值
//LCD_Write_String(0,0,tab_up);//显示第一行
//
//Tem=ReadEnvironTemp();
//envTemp=(float)(Tem)*0.02-273.15;
//sprintf(tab_dw,"E:%5.2f %05d ",envTemp,Tem);//打印温度值
//LCD_Write_String(0,1,tab_dw);//显示第一行
//
//DelayMs(200);

/********************************************************************************

*Function Name  : PEC_calculation

*Description    : Calculates the PEC of received bytes

*Input          : pec[]300.* Output         : None

* Return         : pec[0]-this byte contains calculated crc value

*******************************************************************************/

unsigned char PEC_Calculation(unsigned char pec[])
{
   unsigned char         crc[6];
   unsigned char        BitPosition=47;
   unsigned char        shift;
   unsigned char        i;
   unsigned char        j;
   unsigned char        temp;
   do
    {
        /*Load pattern value 0x000000000107*/
        crc[5]=0;
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;
        /*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
        BitPosition=47;
        /*Set shift position at 0*/
        shift=0;
        /*Find first "1" in the transmited message beginning from the MSByte byte5*/
        i=5;
        j=0;
        while((pec[i]&(0x80>>j))==0 && i>0)
        {
            BitPosition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
        }/*End of while */
        /*Get shift value for pattern value*/
        shift=BitPosition-8;
        /*Shift pattern value */
        while(shift)
        {
            for(i=5; i<0xFF; i--)
            {
                if((crc[i-1]&0x80) && (i>0))
                {
                    temp=1;
                }
                else
                {
                    temp=0;
                }
                crc[i]<<=1;
                crc[i]+=temp;
            }/*End of for*/
            shift--;
        }/*End of while*/
        /*Exclusive OR between pec and crc*/
        for(i=0; i<=5; i++)
        {
            pec[i] ^=crc[i];
        }/*End of for*/
    }
    while(BitPosition>8); /*End of do-while*/
    return pec[0];
}

