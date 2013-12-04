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


//IIC模块配置
void I2C_Configuration(void)
{
  	I2C_InitTypeDef  I2C_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

	/*配置I2C1引脚: SCL 、SDA */
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*中断引脚*/
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);


	I2C_DeInit(I2C1);
  	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; // 设置I2C为I2C模式 ；
  	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; // I2C快速模式Tlow / Thigh = 2 ；
  	I2C_InitStructure.I2C_OwnAddress1 = 0X00;//单片机作为从器件时的自身地址
  	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;	
  	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 应答7位地址 ；
  	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed; // 该参数用来设置时钟频率
  	I2C_Init(I2C1, &I2C_InitStructure);	// 初始化外设I2C1寄存器 ；
  	I2C_Cmd(I2C1, ENABLE); // 使能I2C1外设 ；
  	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}

/*********************************************************************/
//写一个字节
/*********************************************************************/
void I2C_WriteByte(INT8U DriveADD,INT8U WriteAddr,INT8U DATA)
{ INT32U TimeOut=0x3ffff;

  I2C_GenerateSTART(I2C1, ENABLE);//发送IIC启动时序
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)&&TimeOut) TimeOut--;  

  /*发送芯片地址*/
   I2C_Send7bitAddress(I2C1, DriveADD, I2C_Direction_Transmitter);
   TimeOut=0x3ffff;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)&&TimeOut) TimeOut--;  
  
  /*发送芯片内存单元地址*/    
  I2C_SendData(I2C1, WriteAddr);
   TimeOut=0x3ffff;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&&TimeOut) TimeOut--;
				
  //发送一个字节数据
  I2C_SendData(I2C1, DATA);
   TimeOut=0x3ffff;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&&TimeOut) TimeOut--;
  
  /*发送IIC停止时序*/
  I2C_GenerateSTOP(I2C1, ENABLE);

}
/*********************************************************************/
//读一个字节
/*********************************************************************/
INT8U I2C_ReadByte(INT8U DriveADD,INT8U ReadAddr)
{
  INT8U	 DATA=0;
  INT32U TimeOut=0x3ffff;
  I2C_AcknowledgeConfig(I2C1, ENABLE);

  I2C_GenerateSTART(I2C1, ENABLE);//发送IIC启动时序
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)&&TimeOut) TimeOut--;//等待完成


  I2C_Send7bitAddress(I2C1, DriveADD, I2C_Direction_Transmitter);//发送器件地址
   TimeOut=0x3ffff;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)&&TimeOut) TimeOut--;//等待完成
  

  I2C_SendData(I2C1, ReadAddr);//发送器件内存单元地址
  TimeOut=0x3ffff;  
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&&TimeOut) TimeOut--;//等待完成

  I2C_GenerateSTART(I2C1, ENABLE);//发送重启时序
  TimeOut=0x3ffff;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)&&TimeOut) TimeOut--;//等待完成
  
  I2C_Send7bitAddress(I2C1,DriveADD,I2C_Direction_Receiver);//发送器件地址
  TimeOut=0x3ffff;
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)&&TimeOut) TimeOut--;//等待完成

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

