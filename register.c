//-------------------------------------------------------------------
//工 程 名：智能控制系统
//功    能：系统寄存器定义
//设    计：rtx51
//日    期：2009/09/27
//说    明：http://shop36037744.taobao.com
//修    改：
//          无
//-------------------------------------------------------------------
//寄 存 器  地址  长度(字)
//AI        0     4
//DI        4     1
//DQ        6     1
//开启指令  10    20
//关闭指令  30    20
//查询指令  50    20
//数字报警1 70    20
//数字报警2 90    20
//数字报警3 110   20
//数字报警4 130   20
//数字报警5 150   20
//数字报警6 170   20
//数字报警7 190   20
//数字报警8 210   20
//-------------------------------------------------------------------
#include "register.h"
#include "eeprom.h"
idata uint16 I[1] = {0};
idata uint16 Q[1] = {0};
idata uint16 AI[4];
idata char cmd_sms_buf[25] = {0};
void register_init ( void )
{
    register_from_eeprom();
}
//---------------------------------------------------------
//函数功能：register_readbytes
//输入参数：
//          pkt   , 数据指针
//          addr  , 变量地址
//          len   , 变量个数
//返 回 值：
//          0, 操作成功
//          1, 操作失败
//修改记录：无
//---------------------------------------------------------
int8 register_readbytes ( uint8* pkt, uint16 addr, uint8 len )
{
    uint8 i;
    if ( ( addr == 0 ) && ( len == 7 ) )        // AI DI DQ
    {
        for ( i = 0; i < 4; i++ )
        {
            pkt[i*2 + 0] = AI[i] >> 8;
            pkt[i*2 + 1] = AI[i];
        }
        pkt[8] = I[0] >> 8;
        pkt[9] = I[0];
        pkt[10] = Q[0] >> 8;
        pkt[11] = Q[0];
        pkt[12] = 0;
        pkt[13] = VERTION;
        return ( 0 );
    }
    for ( i = 0; i < 24; i++ )                                      // 继电器相关命令处理
    {
        if ( ( addr >= ( 10 + 20 * i ) ) && ( ( addr + len ) <= ( 30 + 20 * i ) ) ) // 读启动命令
        {
            eeprom_read_data ( 20 * i, pkt, 20 );
            return 0;
        }
    }
    for ( i = 0; i < 8; i++ )                                       // 数字报警信息
    {
        if ( ( addr >= ( 522 + 20 * i ) ) && ( ( addr + len ) <= ( 542 + 20 * i ) ) )
        {
            eeprom_read_data ( ( 512 + 20 * i ), pkt, 20 );
            return 0;
        }
    }
    for ( i = 0; i < 3; i++ )                                       // 手机号码
    {
        if ( ( addr >= ( 710 + 20 * i ) ) && ( ( addr + len ) <= ( 730 + 20 * i ) ) )
        {
            eeprom_read_data ( ( 710 + 20 * i ), pkt, 20 );
            return 0;
        }
    }
    for ( i = 0; i < 5; i++ )                                       // 模拟信息查询命令
    {
        if ( ( addr >= ( 770 + 20 * i ) ) && ( ( addr + len ) <= ( 790 + 20 * i ) ) )
        {
            eeprom_read_data ( ( 770 + 20 * i ), pkt, 20 );
            return 0;
        }
    }
    if ( ( addr == 900 ) && ( len == 1 ) )
    {
        eeprom_read_data ( 900, pkt, 1 );
        return 0;
    }
    return 0;
}
void register_to_eeprom ( void )
{
    uint8 i;
    eeprom_erase ( 0 );
    for ( i = 0; i < 30; i++ )
    {
    }
}
void register_from_eeprom ( void )
{
    int i, j;
    printf ( "the mcu eeprom cmd is:\n" );
    for ( j = 0; j < 24; j++ )
    {
        for ( i = 0; i < 20; i++ )
        {
            cmd_sms_buf[i] = eeprom_read ( 20 * j + i );
        }
        if ( ( j % 3 ) == 0 )
        {
            printf ( "继电器%d------------------------------\n", j / 3 + 1 );
            printf ( "open cmd   :[%s]\n", cmd_sms_buf );
        }
        else if ( ( j % 3 ) == 1 )
        {
            printf ( "close cmd  :[%s]\n", cmd_sms_buf );
        }
        else
        {
            printf ( "res   cmd  :[%s]\n", cmd_sms_buf );
        }
    }
    for ( i = 0; i < 20; i++ )
    {
        cmd_sms_buf[i] = eeprom_read ( 480 + i );
    }
    printf ( "all open cmd:[%s]\n", cmd_sms_buf );
    memset ( cmd_sms_buf, 0, sizeof ( cmd_sms_buf ) );
    for ( i = 0; i < 12; i++ )
    {
        cmd_sms_buf[i] = eeprom_read ( 500 + i );
    }
    printf ( "all close cmd:[%s]\n", cmd_sms_buf );
    for ( i = 0; i < 8; i++ )
    {
        for ( j = 0; j < 20; j++ )
        {
            cmd_sms_buf[j] = eeprom_read ( 512 + 20 * i + j );
        }
        printf ( "alram msg  :[%s]\n", cmd_sms_buf );
    }
    for ( i = 0; i < 5; i++ )
    {
        eeprom_read_data ( 770 + 20 * i, cmd_sms_buf, 20 );
        printf ( "adc check cmd:[%s]\n", cmd_sms_buf );
    }
    for ( i = 0; i < 3; i++ )
    {
        eeprom_read_data ( 710 + 20 * i, cmd_sms_buf, 20 );
        printf ( "phone number :[%s]\n", cmd_sms_buf );
    }
    i = eeprom_read ( 900 );
    printf ( "device addr is %d\n", i );
}
//---------------------------------------------------------
//函数功能：register_writebytes
//输入参数：
//          pkt   , 数据指针
//          addr  , 变量地址
//          len   , 变量个数
//返 回 值：
//          0, 操作成功
//          1, 操作失败
//修改记录：无
//---------------------------------------------------------
int8 register_writebytes ( uint8* pkt, uint16 addr, uint8 len )
{
    uint16 i, j;
    if ( ( addr == 5 ) && ( len ) == 1 )                // 写继电器
    {
        Q[0] = pkt[0] << 8 | pkt[1];
        return 0;
    }
    for ( i = 0; i < 24; i++ )                                      // 继电器相关命令处理
    {
        if ( ( addr >= ( 10 + 20 * i ) ) && ( ( addr + len ) <= ( 30 + 20 * i ) ) ) // 写启动命令
        {
            if ( i == 0 )
            {
                eeprom_erase ( 0 );
            }
            memset ( cmd_sms_buf, 0, sizeof ( cmd_sms_buf ) );
            memcpy ( cmd_sms_buf, pkt, len * 2 );
            for ( j = 0; j < 20; j++ )
            {
                eeprom_write ( ( 20 * i + j ), cmd_sms_buf[j] );
            }
            return 0;
        }
    }
    if ( ( addr >= 490 ) && ( ( addr + len ) <= 510 ) )             // 全开指令
    {
        memset ( cmd_sms_buf, 0, sizeof ( cmd_sms_buf ) );
        memcpy ( cmd_sms_buf, pkt, len * 2 );
        for ( i = 0; i < 20; i++ )
        {
            eeprom_write ( ( 480 + i ), cmd_sms_buf[i] );
        }
        return 0;
    }
    if ( ( addr >= 510 ) && ( ( addr + len ) < 522 ) )              // 全关指令
    {
        memset ( cmd_sms_buf, 0, sizeof ( cmd_sms_buf ) );
        memcpy ( cmd_sms_buf, pkt, len * 2 );
        for ( i = 0; i < 12; i++ )
        {
            eeprom_write ( ( 500 + i ), cmd_sms_buf[i] );
        }
        return 0;
    }
    for ( i = 0; i < 8; i++ )                                       // 报警相关命令处理
    {
        if ( ( addr >= ( 522 + 20 * i ) ) && ( ( addr + len ) <= ( 542 + 20 * i ) ) )
        {
            if ( i == 0 )
            {
                eeprom_erase ( 512 );
            }
            memset ( cmd_sms_buf, 0, sizeof ( cmd_sms_buf ) );
            memcpy ( cmd_sms_buf, pkt, len * 2 );
            for ( j = 0; j < 20; j++ )
            {
                eeprom_write ( ( 512 + 20 * i + j ), cmd_sms_buf[j] );
            }
            return 0;
        }
    }
    for ( i = 0; i < 3; i++ )                                       // 报警手机号码
    {
        if ( ( addr >= ( 710 + 20 * i ) ) && ( ( addr + len ) <= ( 730 + 20 * i ) ) )
        {
            memset ( cmd_sms_buf, 0, sizeof ( cmd_sms_buf ) );
            memcpy ( cmd_sms_buf, pkt, len * 2 );
            for ( j = 0; j < 20; j++ )
            {
                eeprom_write ( ( 710 + 20 * i + j ), cmd_sms_buf[j] );
            }
            return 0;
        }
    }
    for ( i = 0; i < 5; i++ )                                       // 模拟量指令
    {
        if ( ( addr >= ( 770 + 20 * i ) ) && ( ( addr + len ) <= ( 790 + 20 * i ) ) )
        {
            memset ( cmd_sms_buf, 0, sizeof ( cmd_sms_buf ) );
            memcpy ( cmd_sms_buf, pkt, len * 2 );
            for ( j = 0; j < 20; j++ )
            {
                eeprom_write ( ( 770 + 20 * i + j ), cmd_sms_buf[j] );
            }
            return 0;
        }
    }
    if ( ( addr == 900 ) && ( len == 1 ) )
    {
        eeprom_write ( 900, pkt[1] );
        return 0;
    }
    return 1;
}
int8 register_chk_adr ( uint8 addr )
{
    uint8 soft_addr;
    soft_addr = eeprom_read ( 900 );
    if ( ( addr == 1 ) || ( addr == soft_addr ) )
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
