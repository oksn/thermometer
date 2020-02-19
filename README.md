#基于STC12C5A60S2的红外传感器MLX90165读取程序
sbit SCL=P0^7;//时钟线
sbit SDA=P0^6;//数据线
// 20200219 修改支持CRC校验 
