#include "mpuiic.h"
#include "timer_delay.h"
#include  "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������V3
//MPU6050 IIC���� ����
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/17
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

  //MPU IIC ��ʱ����
void MPU_IIC_Delay(void)
{
	delay_us(5);
}

//��ʼ��IIC
void MPU_IIC_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//��ʹ������IO PORTBʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;	 // �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIO

  GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11);						 //PB10,PB11 �����

}
//����IIC��ʼ�ź�
void MPU_IIC_Start(void)
{
	MPU_SDA_OUT();     //sda�����
	MPU_IIC_SDA=1;
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
 	MPU_IIC_SDA=0;//START:when CLK is high,DATA change form high to low
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void MPU_IIC_Stop(void)
{
	MPU_SDA_OUT();//sda�����
	MPU_IIC_SCL=0;
	MPU_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	MPU_IIC_Delay();
	MPU_IIC_SCL=1;
	MPU_IIC_SDA=1;//����I2C���߽����ź�
	MPU_IIC_Delay();
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MPU_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MPU_SDA_IN();      //SDA����Ϊ����
	MPU_IIC_SDA=1;MPU_IIC_Delay();
	MPU_IIC_SCL=1;MPU_IIC_Delay();
	while(MPU_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL=0;//ʱ�����0
	return 0;
}
//����ACKӦ��
void MPU_IIC_Ack(void)
{
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=0;
	MPU_IIC_Delay();
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;
}
//������ACKӦ��
void MPU_IIC_NAck(void)
{
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void MPU_IIC_Send_Byte(u8 txd)
{
    u8 t;
	MPU_SDA_OUT();
    MPU_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
        MPU_IIC_SDA=(txd&0x80)>>7;//��ȡ�������ݵĵ�һλ��Ȼ����λ�����λ�����ͳ�ȥ
        txd<<=1; //��������Ҫ���͵����ݵĵڶ�λ�Ƶ���һλ��׼����һ��ѭ��
		    MPU_IIC_SCL=1;
		    MPU_IIC_Delay();
		    MPU_IIC_SCL=0;
		    MPU_IIC_Delay();
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	MPU_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        MPU_IIC_SCL=0;
        MPU_IIC_Delay();
		MPU_IIC_SCL=1;
        receive<<=1;
        if(MPU_READ_SDA)receive++;
		MPU_IIC_Delay();
    }
    if (!ack)
        MPU_IIC_NAck();//����nACK
    else
        MPU_IIC_Ack(); //����ACK
    return receive;
}

























