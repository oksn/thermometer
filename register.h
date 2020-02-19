#ifndef     __REGISTER_H
#define     __REGISTER_H
#include "config.h"
#define     Q_LENTH     4
#define     I_LENTH     4
#define     CMD_RELAY_ADDR      0
#define     CMD_ALRAM_MSG       522
#define     CMD_ALRAM_PHONE     710
typedef struct _RELAY_CTRL
{
    uint16 year;
    uint16 month;
    uint16 day;
    uint16 hour;
    uint16 minute;
    uint16 second;
    uint16 relay;
    uint16 stat;
    uint16 cycle;
} RELAY_CTRL;
extern idata uint16 I[1];
extern idata uint16 Q[1];
extern idata uint16 AI[4];
extern idata char cmd_sms_buf[25];
int8 register_chk_adr ( uint8 addr );
void register_init ( void );
int8 register_writebytes ( uint8* pkt, uint16 addr, uint8 len );
int8 register_readbytes ( uint8* pkt, uint16 addr, uint8 len );
void register_from_eeprom ( void );
void register_to_eeprom ( void );
#endif
