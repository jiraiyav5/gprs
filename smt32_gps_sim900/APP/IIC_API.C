/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : i2c_ee.c
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : This file provides a set of functions needed to manage the
*                      communication between I2C peripheral and I2C M24C08 EEPROM.
*******************************************************************************/
#include <includes.h>
#include "IIC_API.h"

#define I2C_Speed              100000


//IICģ������
void I2C_Configuration(void)
{
  	I2C_InitTypeDef  I2C_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

	/*����I2C1����: SCL ��SDA */
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*�ж�����*/
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);


	I2C_DeInit(I2C1);
  	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; // ����I2CΪI2Cģʽ ��
  	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; // I2C����ģʽTlow / Thigh = 2 ��
  	I2C_InitStructure.I2C_OwnAddress1 = 0X00;//��Ƭ����Ϊ������ʱ��������ַ
  	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;	
  	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // Ӧ��7λ��ַ ��
  	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed; // �ò�����������ʱ��Ƶ��
  	I2C_Init(I2C1, &I2C_InitStructure);	// ��ʼ������I2C1�Ĵ��� ��
  	I2C_Cmd(I2C1, ENABLE); // ʹ��I2C1���� ��
  	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}

/*********************************************************************/
//дһ���ֽ�
/*********************************************************************/
void I2C_WriteByte(INT8U DriveADD,INT8U WriteAddr,INT8U DATA)
{ INT32U TimeOut=0x3ffff;

  I2C_GenerateSTART(I2C1, ENABLE);//����IIC����ʱ��
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)&&TimeOut) TimeOut--;  

  /*����оƬ��ַ*/
   I2C_Send7bitAddress(I2C1, DriveADD, I2C_Direction_Transmitter);
   TimeOut=0x3ffff;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)&&TimeOut) TimeOut--;  
  
  /*����оƬ�ڴ浥Ԫ��ַ*/    
  I2C_SendData(I2C1, WriteAddr);
   TimeOut=0x3ffff;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&&TimeOut) TimeOut--;
				
  //����һ���ֽ�����
  I2C_SendData(I2C1, DATA);
   TimeOut=0x3ffff;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&&TimeOut) TimeOut--;
  
  /*����IICֹͣʱ��*/
  I2C_GenerateSTOP(I2C1, ENABLE);

}
/*********************************************************************/
//��һ���ֽ�
/*********************************************************************/
INT8U I2C_ReadByte(INT8U DriveADD,INT8U ReadAddr)
{
  INT8U	 DATA=0;
  INT32U TimeOut=0x3ffff;
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  I2C_GenerateSTART(I2C1, ENABLE);//����IIC����ʱ��
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)&&TimeOut) TimeOut--;//�ȴ����


  I2C_Send7bitAddress(I2C1, DriveADD, I2C_Direction_Transmitter);//����������ַ
   TimeOut=0x3ffff;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)&&TimeOut) TimeOut--;//�ȴ����
  

  I2C_SendData(I2C1, ReadAddr);//���������ڴ浥Ԫ��ַ
  TimeOut=0x3ffff;  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&&TimeOut) TimeOut--;//�ȴ����

  I2C_GenerateSTART(I2C1, ENABLE);//��������ʱ��
  TimeOut=0x3ffff;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)&&TimeOut) TimeOut--;//�ȴ����
  
  I2C_Send7bitAddress(I2C1,DriveADD,I2C_Direction_Receiver);//����������ַ
  TimeOut=0x3ffff;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)&&TimeOut) TimeOut--;//�ȴ����

  TimeOut=0x3ffff;
  while( (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))&&TimeOut) TimeOut--;

  DATA = I2C_ReceiveData(I2C1);

  I2C_AcknowledgeConfig(I2C1, DISABLE);
  I2C_GenerateSTOP(I2C1, ENABLE);
  TimeOut=0x3ffff;
  while( (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))&&TimeOut) TimeOut--;

  I2C_AcknowledgeConfig(I2C1, ENABLE);
  return DATA;
}
