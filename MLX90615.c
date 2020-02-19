#include "MLX90615.h"
#include "intrins.h"
#include "delay.h"
#include "UART.h"

#define _NOP() _nop_()

// 5us
void delay_Tbuf()
{
    unsigned char a, b;
    for ( b = 1; b > 0; b-- )
        for ( a = 1; a > 0; a-- );
}


void delay_Thd()
{
    _nop_();
}

void MLX90615_init ( void )
{
    mSDA_OUT;    // Set SDA as Output
    mSCL_OUT;    // Set SCL as Output
    mSDA_HIGH(); // bus free
    mSCL_HIGH();
}

void START_bit ( void )
{
    mSDA_OUT;
    mSDA_HIGH();            // Set SDA line
    delay_Tbuf();         // Wait a few microseconds
    mSCL_HIGH();            // Set SCL line
    delay_Tbuf();         // Generate bus free time between Stop
    // and Start condition (Tbuf=4.7us min)
    mSDA_LOW();             // Clear SDA line
    delay_Tbuf();         // Hold time after (Repeated) Start
    // Condition. After this period, the first clock is generated.
    //(Thd:sta=4.0us min)
    mSCL_LOW();             // Clear SCL line
    delay_Tbuf();         // Wait a few microseconds
}

void STOP_bit ( void )
{
    mSDA_OUT;
    mSCL_LOW();             // Clear SCL line
    delay_Tbuf();         // Wait a few microseconds
    mSDA_LOW();             // Clear SDA line
    delay_Tbuf();         // Wait a few microseconds
    mSCL_HIGH();            // Set SCL line
    delay_Tbuf();         // Stop condition setup time(Tsu:sto=4.0us min)
    mSDA_HIGH();            // Set SDA line
}

unsigned char TX_byte ( unsigned char Tx_buffer )
{
    unsigned char   Bit_counter;
    unsigned char   Ack_bit;
    unsigned char   bit_out;

    for ( Bit_counter = 8; Bit_counter; Bit_counter-- )
    {
        if ( Tx_buffer & 0x80 )
            bit_out = 1; // If the current bit of Tx_buffer is 1 set bit_out
        else
            bit_out = 0; // else clear bit_out

        send_bit ( bit_out );         // Send the current bit on SDA
        Tx_buffer <<= 1;              // Get next bit for checking
    }

    Ack_bit = Receive_bit();          // Get acknowledgment bit

    return  Ack_bit;
}// End of TX_bite()

unsigned char RX_byte ( unsigned char ack_nack )
{
    unsigned char RX_buffer;
    unsigned char   Bit_Counter;

    for ( Bit_Counter = 8; Bit_Counter; Bit_Counter-- )
    {
        if ( Receive_bit() )                    // Get a bit from the SDA line
        {
            RX_buffer <<= 1;                    // If the bit is HIGH save 1  in RX_buffer
            RX_buffer |= 0x01;
        }
        else
        {
            RX_buffer <<= 1;                    // If the bit is LOW save 0 in RX_buffer
            RX_buffer &= 0xfe;
        }
    }

    send_bit ( ack_nack );                      // Sends acknowledgment bit

    return RX_buffer;
}

//---------------------------------------------------------------------------------------------
void send_bit ( unsigned char bit_out )
{
    mSDA_OUT;
    if ( bit_out )
        mSDA_HIGH();
    else
        mSDA_LOW();

    delay_Thd();                            // Tsu:dat = 250ns minimum

    mSCL_HIGH();                    // Set SCL line
    delay_Tbuf();           // High Level of Clock Pulse------------------
    mSCL_LOW();                     // Clear SCL line
    delay_Tbuf();           // Low Level of Clock Pulse----------------------
//  mSDA_HIGH();                    // Master release SDA line ,

    return;
}//End of send_bit()
//---------------------------------------------------------------------------------------------

unsigned char Receive_bit ( void )
{
    unsigned char Ack_bit;

    mSDA_IN;                          // SDA-input
    _NOP();
    _NOP();
    _NOP();
    mSCL_HIGH();                    // Set SCL line
    delay_Tbuf();                 // High Level of Clock Pulse
//  if(P2Input(BIT2))
    SDA = 1;
    if ( SDA )
        Ack_bit = 1;            // \ Read acknowledgment bit, save it in Ack_bit
    else
        Ack_bit = 0;            // /
    mSCL_LOW();                     // Clear SCL line
    delay_Tbuf();                 // Low Level of Clock Pulse

    return  Ack_bit;
}//End of Receive_bit

unsigned int MemRead ( unsigned char SlaveAddress, unsigned char command )
{
    unsigned int  tdata;                // Data storage (DataH:DataL)
    unsigned char Pec;                  // PEC byte storage
    unsigned char DataL;                // Low data byte storage
    unsigned char DataH;                // High data byte storage
    unsigned char arr[6];               // Buffer for the sent bytes
    unsigned char PecReg;               // Calculated PEC byte storage
    unsigned char ErrorCounter;     // Defines the number of the attempts for communication with MLX90614

    ErrorCounter = 0x00;                // Initialising of ErrorCounter

    do
    {
    repeat:
        STOP_bit();                         //If slave send NACK stop comunication
        --ErrorCounter;                   //Pre-decrement ErrorCounter
        if ( !ErrorCounter )            //ErrorCounter=0?
        {
            break;                            //Yes,go out from do-while{}
        }
        START_bit();                        //Start condition

        if ( TX_byte ( SlaveAddress ) ) //Send SlaveAddress
        {
            goto    repeat;                 //Repeat comunication again
        }

        if ( TX_byte ( command ) )      //Send command
        {
            goto    repeat;                 //Repeat comunication again
        }
        START_bit();                        //Repeated Start condition

        if ( TX_byte ( SlaveAddress ) ) //Send SlaveAddress-------------------???
        {
            goto    repeat;             //Repeat comunication again
        }

        DataL = RX_byte ( ACK );        //Read low data,master must send ACK
        DataH = RX_byte ( ACK );        //Read high data,master must send ACK
        Pec = RX_byte ( NACK );         //Read PEC byte, master must send NACK
        STOP_bit();                         //Stop condition


        arr[5] = SlaveAddress;          //
        arr[4] = command;                 //
        arr[3] = SlaveAddress;          //Load array arr
        arr[2] = DataL;                     //
        arr[1] = DataH;                     //
        arr[0] = 0;                           //
        PecReg = PEC_calculation ( arr ); //Calculate CRC
        UART1_SendOneChar ( PecReg );
        UART1_SendOneChar ( Pec );
    }
    while ( PecReg != Pec );        //If received and calculated CRC are equal go out from do-while{}

    * ( ( unsigned char * ) ( &tdata ) ) = DataH; //
    * ( ( unsigned char * ) ( &tdata ) + 1 ) = DataL; //data=DataH:DataL

    return tdata;
}

unsigned char PEC_calculation ( unsigned char pec[] )
{
    unsigned char crc[6];
    unsigned char   BitPosition = 47;
    unsigned char   shift;
    unsigned char   i;
    unsigned char   j;
    unsigned char   temp;

    do
    {
        crc[5] = 0;             /* Load CRC value 0x000000000107 */
        crc[4] = 0;
        crc[3] = 0;
        crc[2] = 0;
        crc[1] = 0x01;
        crc[0] = 0x07;
        BitPosition = 47;       /* Set maximum bit position at 47 */
        shift = 0;

        //Find first 1 in the transmited message
        i = 5;                  /* Set highest index */
        j = 0;
        while ( ( pec[i]& ( 0x80 >> j ) ) == 0 && i > 0 )
        {
            BitPosition--;
            if ( j < 7 )
            {
                j++;
            }
            else
            {
                j = 0x00;
                i--;
            }
        }/*End of while */

        shift = BitPosition - 8; /*Get shift value for crc value*/


        //Shift crc value
        while ( shift )
        {
            for ( i = 5; i < 0xFF; i-- )
            {
                if ( ( crc[i-1] & 0x80 ) && ( i > 0 ) )
                {
                    temp = 1;
                }
                else
                {
                    temp = 0;
                }
                crc[i] <<= 1;
                crc[i] += temp;
            }/*End of for*/
            shift--;
        }/*End of while*/


        //Exclusive OR between pec and crc
        for ( i = 0; i <= 5; i++ )
        {
            pec[i] ^= crc[i];
        }/*End of for*/
    }
    while ( BitPosition > 8 ); /*End of do-while*/

    return pec[0];
}/*End of PEC_calculation*/