#ifndef _mlx90614_H_
#define _mlx90614_H_



void start_bit(); //MLX90614 发起始位子程序
void stop_bit(); //MLX90614 发结束位子程序
unsigned char rx_byte(void); //MLX90614 接收字节子程序
void send_bit(void); //MLX90614 发送位子程序
void tx_byte(unsigned char dat_byte); //MLX90614 接收字节子程序
void receive_bit(void); //MLX90614 接收位子程序
unsigned int ReadObjectTemp(void); //读温度数据
unsigned int ReadEnvironTemp(void);unsigned char PEC_Calculation(unsigned char pec[]);
