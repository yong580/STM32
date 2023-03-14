#ifndef __MPUIIC_H
#define	__MPUIIC_H
#include "sys.h"


//IO��������
#define MPU_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}//0100<<4==0100 0000
#define MPU_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}//0011<<4==0011 0000

//IO��������
#if !defined MPU_IIC_SCL  || !defined MPU_IIC_SDA || !defined MPU_READ_SDA
#define MPU_IIC_SCL    PBout(6) 		//SCL  6
#define MPU_IIC_SDA    PBout(7) 		//SDA  7	 
#define MPU_READ_SDA   PBin(7) 		    //����SDA 
#endif

//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  


#endif
