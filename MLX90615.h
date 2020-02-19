#ifndef __MLX90615_H
#define __MLX90615_H

#include "stc12c5a60s2.h"

sbit SDA = P0 ^ 6;
sbit SCL = P0 ^ 7;

#define mSDA_LOW()    SDA=0
#define mSDA_HIGH()   SDA=1
#define mSCL_LOW()    SCL=0
#define mSCL_HIGH()   SCL=1

#define mSDA_IN       do{} while(0)
#define mSDA_OUT      do{} while(0)
#define mSCL_IN       do{} while(0)
#define mSCL_OUT      do{} while(0)

#define ACK       0
#define NACK      1

//MLX90614 constants
#define SA                0x00  // Slave address
#define DEFAULT_SA      0x5B    // Default Slave address
#define RAM_Access      0x20    // RAM access command
#define EEPROM_Access   0x10    // EEPROM access command
#define RAM_Ta          0x06    // Ta address in the ram
#define RAM_To          0x07    // To address in the ram

//*PROTOTYPES**********************************************************************************
void START_bit ( void );
void STOP_bit ( void );
unsigned char TX_byte ( unsigned char Tx_buffer );
unsigned char RX_byte ( unsigned char ack_nack );
void send_bit ( unsigned char bit_out );
unsigned char Receive_bit ( void );
void MLX90615_init ( void );
unsigned int MemRead ( unsigned char SlaveAddress, unsigned char command );
void SendRequest ( void );
void DummyCommand ( unsigned char byte );
float CalcTemp ( unsigned int value );
unsigned char PEC_calculation ( unsigned char pec[] );

#endif