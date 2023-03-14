#ifndef __MPUIIC_H
#define	__MPUIIC_H
#include "sys.h"


//IO方向设置
#define MPU_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}//0100<<4==0100 0000
#define MPU_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}//0011<<4==0011 0000

//IO操作函数
#if !defined MPU_IIC_SCL  || !defined MPU_IIC_SDA || !defined MPU_READ_SDA
#define MPU_IIC_SCL    PBout(6) 		//SCL  6
#define MPU_IIC_SDA    PBout(7) 		//SDA  7	 
#define MPU_READ_SDA   PBin(7) 		    //输入SDA 
#endif

//IIC所有操作函数
void MPU_IIC_Delay(void);				//MPU IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口				 
void MPU_IIC_Start(void);				//发送IIC开始信号
void MPU_IIC_Stop(void);	  			//发送IIC停止信号
void MPU_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void MPU_IIC_Ack(void);					//IIC发送ACK信号
void MPU_IIC_NAck(void);				//IIC不发送ACK信号

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  


#endif
