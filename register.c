//-------------------------------------------------------------------
//�� �� �������ܿ���ϵͳ
//��    �ܣ�ϵͳ�Ĵ�������
//��    �ƣ�rtx51
//��    �ڣ�2009/09/27
//˵    ����http://shop36037744.taobao.com
//��    �ģ�
//          ��
//-------------------------------------------------------------------
//�� �� ��  ��ַ  ����(��)
//AI        0     4
//DI        4     1
//DQ        6     1
//����ָ��  10    20
//�ر�ָ��  30    20
//��ѯָ��  50    20
//���ֱ���1 70    20
//���ֱ���2 90    20
//���ֱ���3 110   20
//���ֱ���4 130   20
//���ֱ���5 150   20
//���ֱ���6 170   20
//���ֱ���7 190   20
//���ֱ���8 210   20
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
//�������ܣ�register_readbytes
//���������
//          pkt   , ����ָ��
//          addr  , ������ַ
//          len   , ��������
//�� �� ֵ��
//          0, �����ɹ�
//          1, ����ʧ��
//�޸ļ�¼����
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
    for ( i = 0; i < 24; i++ )                                      // �̵�����������
    {
        if ( ( addr >= ( 10 + 20 * i ) ) && ( ( addr + len ) <= ( 30 + 20 * i ) ) ) // ����������
        {
            eeprom_read_data ( 20 * i, pkt, 20 );
            return 0;
        }
    }
    for ( i = 0; i < 8; i++ )                                       // ���ֱ�����Ϣ
    {
        if ( ( addr >= ( 522 + 20 * i ) ) && ( ( addr + len ) <= ( 542 + 20 * i ) ) )
        {
            eeprom_read_data ( ( 512 + 20 * i ), pkt, 20 );
            return 0;
        }
    }
    for ( i = 0; i < 3; i++ )                                       // �ֻ�����
    {
        if ( ( addr >= ( 710 + 20 * i ) ) && ( ( addr + len ) <= ( 730 + 20 * i ) ) )
        {
            eeprom_read_data ( ( 710 + 20 * i ), pkt, 20 );
            return 0;
        }
    }
    for ( i = 0; i < 5; i++ )                                       // ģ����Ϣ��ѯ����
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
            printf ( "�̵���%d------------------------------\n", j / 3 + 1 );
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
//�������ܣ�register_writebytes
//���������
//          pkt   , ����ָ��
//          addr  , ������ַ
//          len   , ��������
//�� �� ֵ��
//          0, �����ɹ�
//          1, ����ʧ��
//�޸ļ�¼����
//---------------------------------------------------------
int8 register_writebytes ( uint8* pkt, uint16 addr, uint8 len )
{
    uint16 i, j;
    if ( ( addr == 5 ) && ( len ) == 1 )                // д�̵���
    {
        Q[0] = pkt[0] << 8 | pkt[1];
        return 0;
    }
    for ( i = 0; i < 24; i++ )                                      // �̵�����������
    {
        if ( ( addr >= ( 10 + 20 * i ) ) && ( ( addr + len ) <= ( 30 + 20 * i ) ) ) // д��������
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
    if ( ( addr >= 490 ) && ( ( addr + len ) <= 510 ) )             // ȫ��ָ��
    {
        memset ( cmd_sms_buf, 0, sizeof ( cmd_sms_buf ) );
        memcpy ( cmd_sms_buf, pkt, len * 2 );
        for ( i = 0; i < 20; i++ )
        {
            eeprom_write ( ( 480 + i ), cmd_sms_buf[i] );
        }
        return 0;
    }
    if ( ( addr >= 510 ) && ( ( addr + len ) < 522 ) )              // ȫ��ָ��
    {
        memset ( cmd_sms_buf, 0, sizeof ( cmd_sms_buf ) );
        memcpy ( cmd_sms_buf, pkt, len * 2 );
        for ( i = 0; i < 12; i++ )
        {
            eeprom_write ( ( 500 + i ), cmd_sms_buf[i] );
        }
        return 0;
    }
    for ( i = 0; i < 8; i++ )                                       // ������������
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
    for ( i = 0; i < 3; i++ )                                       // �����ֻ�����
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
    for ( i = 0; i < 5; i++ )                                       // ģ����ָ��
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
