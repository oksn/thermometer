#ifndef _mlx90614_H_
#define _mlx90614_H_



void start_bit(); //MLX90614 ����ʼλ�ӳ���
void stop_bit(); //MLX90614 ������λ�ӳ���
unsigned char rx_byte(void); //MLX90614 �����ֽ��ӳ���
void send_bit(void); //MLX90614 ����λ�ӳ���
void tx_byte(unsigned char dat_byte); //MLX90614 �����ֽ��ӳ���
void receive_bit(void); //MLX90614 ����λ�ӳ���
unsigned int ReadObjectTemp(void); //���¶�����
unsigned int ReadEnvironTemp(void);unsigned char PEC_Calculation(unsigned char pec[]);
