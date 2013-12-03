/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  -易通电子原创代码-          		          						  ▇
▇ 作者：莫建林																					  ▇
▇   QQ: 1695627698 																			  ▇
▇ 旺旺：一易通电子科技一 																		  ▇
▇ 邮箱：mojianlin2011@126.com																	  ▇
▇ 淘宝专卖店：etmcu.taobao.com																	  ▇
▇ 声明:此程序是为易通无线技术开发板而写的，请勿用于商业目的，若要引用请附上我的联系方式！		  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
//此模块是用户自定义功能函数

#include "includes.h"
#include "GSM.h"
#include "GSM_TypeDefine.h"
#include "TempTable.h"
#include "GSM_USER_API.h"
#include <stm32f10x_lib.h>
INT32U My_Key_Number=0;

#define  Gsenser_CS1    GPIO_SetBits(  GPIOE, GPIO_Pin_4)  
#define  Gsenser_CS0    GPIO_ResetBits(GPIOE, GPIO_Pin_4)

#define  Temp_ADJ   3

//初始化操作密码
void Key_Number_Init(void)
{
  INT16U temp,K[1],key16;
  INT32U Key32=888888;//设置初始密码=888888
  EE_ReadVariable(2,K);//存储在地址2的值作为是否初始化密码的判定值
  temp=K[0];
  if(temp!=8481)
    {
	   key16=Key32>>16;
 	   EE_WriteVariable(0,key16);   //写入初始密码
	   key16=Key32&0x0000ffff;
	   EE_WriteVariable(1,key16);
 	   EE_WriteVariable(2,8481); //初始化后写入判定值，下次复位就不会再初始化了。
    }
  else
    {
	 Key_Number_Renew();
	}
}

//密码重设后存入新的密码
void Key_Number_Store(void)
{
 INT16U temp;
   temp=My_Key_Number>>16;
   EE_WriteVariable(0,temp);
   temp=My_Key_Number&0x0000ffff;
   EE_WriteVariable(1,temp);
}

//单片机复位后读出之前的密码，更新密码变量My_Key_Number
void Key_Number_Renew(void)
{
 INT16U temp[1];

   EE_ReadVariable(0,temp);
   My_Key_Number=0;
   My_Key_Number+=temp[0];
   My_Key_Number=My_Key_Number<<16;

   EE_ReadVariable(1,temp);
   My_Key_Number+=temp[0];
}

//通过采集热敏电阻的分压值计算环境温度
//注意：实际温度=函数返回值	- 40；
INT8S Get_Temperature(void)
{
    float ADx,x;
  	INT8U  i,FT,temp=0;
  	INT16U Rt=0;
  	INT16U ADCt=0;

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//参见BSP.C
	OSTimeDlyHMSM(0, 0, 0, 20);
  	ADCt=ADC_Result[ADC_TEMP];
  	
    ADx=(float)ADCt;  //目的是把转换成浮点数
    x=10*ADx/(4095-ADx); //求热敏电阻的阻值，这是化简以后的公式
                         //[仅适用于ADC模块电源=NTC电源]；
    x=x*100; //扩大100倍也就是取小数点后的两位
    Rt=(INT16U)x;   //将阻值扩大100倍后赋给Rt，小数部分将被舍去；

//	TFT_ShowNum(100,240,Rt);
    if(Rt>TempTable[0])
	{
	 return 0;
	}
	else if(Rt<TempTable[140])
		 {
		  return 100;
		 }
		 else
		 { 
	      FT=1; 
          for(i=140;FT;i--)
           {
	        if(Rt>=TempTable[i]&&Rt<=TempTable[i-1]) 
	        {
		    if((TempTable[i-1]-Rt)<(Rt-TempTable[i])) i=i-1;  //看Rt接近哪个值就取哪个的编号
	        temp=i;
	        FT=0;
	        }
    	   }
        }
return (temp-40-Temp_ADJ);	  
}

/****************************************************************************
* 名    称：unsigned char SPI_WriteByte(unsigned char data) 
* 功    能：SPI1 写函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/  
unsigned char SPI_WriteByte(unsigned char data) 
{ 
 unsigned char Data = 0; 

  //等待发送缓冲区空
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET); 
  // 发送一个字节  
  SPI_I2S_SendData(SPI1,data); 

   //等待是否接收到一个字节 
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET); 
  // 获得该字节
  Data = SPI_I2S_ReceiveData(SPI1)&0XFF; 

  // 返回收到的字节 
  return Data; 
}  

/****************************************************************************
* 名    称：void SpiDelay(unsigned int DelayCnt) 
* 功    能：SPI1 写延时函数
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：
****************************************************************************/  
void SpiDelay(unsigned int DelayCnt)
{
 unsigned int i;
 for(i=0;i<DelayCnt;i++);
}
unsigned  char SHAKE_SHOW_COUNT=0;
void  Gsenser_SHOW_XYZ(void)
{
  unsigned  short int GX=0,GY=0,GZ=0;
  unsigned  char INT_STU=0;

	
	LCD_SHOW_CN15x16_String(0,240,"重力加速度传感器:");
	
  GX=Gsenser_ReadREG2(0x32);
	LCD_SHOW_CN15x16_String(0,260,"X轴:");
  TFT_ShowHEX16(35,260,GX);

  GY=Gsenser_ReadREG2(0x34);
  LCD_SHOW_CN15x16_String(85,260,"Y轴:");
  TFT_ShowHEX16(120,260,GY);

  GZ=Gsenser_ReadREG2(0x36);
  LCD_SHOW_CN15x16_String(160,260,"Z轴:");
  TFT_ShowHEX16(195,260,GZ);
  
  Gsenser_ReadREG(0x2B);
	
	INT_STU=Gsenser_ReadREG(0x30);

	if(INT_STU&0x70)
	  {
     LCD_SHOW_CN15x16_String(140,240,"(检测到震动)");
		 SHAKE_SHOW_COUNT=5;
    }
	else 
	  {
			if(SHAKE_SHOW_COUNT) SHAKE_SHOW_COUNT--;
			if(SHAKE_SHOW_COUNT==0) TFT_ShowString(140,240,"            ");
    }
	
	
	
	
}

void  Gsenser_SHOW_REG(void)
{
  unsigned  short int X,Y;
  unsigned	char add,data=0;
  X=0;
  Y=80;
  for(add=0x1D;add<0x3A;add++)
   {
   	TFT_ShowHEX(X,Y,add);
	TFT_ShowString(X+20,Y,"=>");
	data=Gsenser_ReadREG(add);
	TFT_ShowHEX(X+40,Y,data);
   	Y+=16;
   	if(Y>240) {Y=80; X+=70;}
   }

}
void ADXL345_Test(void)
{
 //	INT8U  dd=0;
	 //Gsenser_SHOW_REG();//显示ADXL345内部所有寄存器的值
    // SHOW_SPI_REG();	 //显示SPI模块的所有相关寄存器值，看是否配置正确
	 Gsenser_SHOW_XYZ();//显示X,Y,Z轴的重力加速度转换值
	//Gsenser_WriteREG(0x1E,dd++);//用于检测SPI通讯是否正常
}


void ADXL345_Init(void)
{
  Gsenser_WriteREG(0x1D,0x05);/*0x1D->THRESH_TAP寄存器为8位寄存器，保存敲击中断的阈
                                值。数据格式无符号，因此，敲击事件的幅度与
                                 THRESH_TAP的值进行比较，以便实现正常敲击检测。
                                比例因子为62.5mg/LSB(即0xFF =16 g)。如果使能单击/双
                                击中断，值为0可能会导致工作异常。*/
  SpiDelay(100);
  Gsenser_WriteREG(0x1E,0x00);/*X轴校正值*/
  SpiDelay(100);
  Gsenser_WriteREG(0x1F,0x00);/*Y轴校正值*/
  SpiDelay(100);
  Gsenser_WriteREG(0x20,0x00);/*Z轴校正值*/
  SpiDelay(100);
  Gsenser_WriteREG(0x21,0x0A);/*/0x21->DUR为8位寄存器，包含未签名时间值，表示必须是
                                 THRESH_TAP阈值之上的事件才能称之为敲击事件的最
                                 大时间。比例因子为625 μs/LSB。值为0时，禁用单击/双击功能。*/
  SpiDelay(100);
  Gsenser_WriteREG(0x22,100); /*0x22->Latent寄存器为8位寄存器，包含无符号时间值，表示从
                                 敲击事件检测到时间窗口（由Window寄存器定义）开始
                                 的等待时间，在此期间，能检测出可能的第二次敲击事
                                 件。比例因子为1.25 ms/LSB。值为0时，禁用双击功能。*/
  SpiDelay(100);
  Gsenser_WriteREG(0x23,100);/*0x23->Window寄存器为8位寄存器，包含未签名的时间值，表
                                示延迟时间(由Latent寄存器确定)期满后的时间量，在此
                                期间，能开始进行第二次有效敲击。比例因子为1.25 
                                 ms/LSB。值为0时，禁用双击功能。*/
  SpiDelay(100);
  Gsenser_WriteREG(0x24,16);/*0x24->THRESH_ACT寄存器为8位寄存器，保存检测活动的阈
                                值。数据格式无符号，因此，活动事件的幅度与
                                THRESH_ACT寄存器的值进行比较。比例因子为62.5 
                                mg/LSB。如果使能活动中断，值为0时，可能导致工作异常。*/
  SpiDelay(100);
  Gsenser_WriteREG(0x25,0x05);/*0x25->THRESH_INACT寄存器为8位寄存器，保存检测静止的
								阈值。数据格式无符号，因此，静止事件的幅度与
								THRESH_ INACT寄存器的值进行比较。比例因子为62.5 
								mg/LSB。如果使能静止中断，值为0时，可能导致工作异常。*/
  SpiDelay(100);
  Gsenser_WriteREG(0x26,1);/*0x26->TIME_INACT寄存器为8位寄存器，包含未签名时间值，
								表示加速度的时间量必须小于THRESH_INACT寄存器的
								值，以宣布静止。比例因子为1 sec /LSB。有别于其他使
								用未滤波数据(见阈值部分)的中断功能，该静止功能采
								用滤波输出数据。为要触发的静止中断，必须生成至少
								一个输出样本。如果TIME_INACT寄存器设置值小于输
								出数据速率的时间常数，将导致功能反应迟钝。当输出
								数据小于THRESH_INACT寄存器的值，值为0导致工作异常。*/
  SpiDelay(100);
  Gsenser_WriteREG(0x27,0xFF);/*ACT交流/直流|INACT交流/直流|ACT_X使能|INACT_X使能|ACT_Y使能|INACT_Y使能|ACT_Z使能|INACT_Z使能*/
  SpiDelay(100);
  Gsenser_WriteREG(0x28,0x09);/*0x28->THRESH_FF寄存器为8位寄存器，保存未签名格式的阈
								值，用于自由落体检测。所有轴的加速度与THRESH_FF
								的值相比较，以确定是否有自由落体事件发生。比例因子
								为62.5 mg/LSB。请注意，如果自由落体中断被使能，值为
								0 mg可能导致工作异常。建议采用300 mg与600 mg(0x05至0x09)之间的值。*/
  SpiDelay(100);
  Gsenser_WriteREG(0x29,0xFF);/*0x29->TIME_FF寄存器为8位寄存器，存储未签名值，表示所有
								轴的值必须小于THRESH_FF的最小时间，以生成自由落
								体中断。比例因子为5 ms/LSB。如果使能自由落体中断，
								值为0时，可能导致工作异常。建议采用100 ms与350 ms(0x14至0x46)之间的值。*/
  SpiDelay(100);
  Gsenser_WriteREG(0x2A,0x07);/*0x2A->敲击抑制控制寄存器【参见数据手册】*/
  SpiDelay(100);
//  Gsenser_WriteREG(0x2B,0x00);/*0x2B->[状态寄存器][只读]|0|ACT_X来源|ACT_Y来源|ACT_Z来源|休眠|TAP_X来源|TAP_Y来源|TAP_Z来源*/
//  SpiDelay(100);
  Gsenser_WriteREG(0x2C,0x0A);/*0x2C->功耗和转换速率配置：默认值为0x0A，转换为100Hz的输出数据速率。应选
								择适合所选通信协议和频率的输出数据速率。选择太高
								输出数据速率和低通信速度会导致采样*/
  SpiDelay(100);
  Gsenser_WriteREG(0x2D,0x28);/*0x2D->功耗控制寄存器：|0|0|AUTO_SLEEP|链接|测量|休眠|唤醒[1:0]|*/
  SpiDelay(100);
  Gsenser_WriteREG(0x2E,0xFF);/*中断使能寄存器【参见数据手册】*/
  SpiDelay(100);
  Gsenser_WriteREG(0x2F,0x00);/*中断输出引脚配置寄存器【参见数据手册】*/
  SpiDelay(100);
//  Gsenser_WriteREG(0x30,0x00);/*寄存器0x30—INT_SOURCE(只读)*/
//  SpiDelay(100);
  Gsenser_WriteREG(0x31,0x2B);/*寄存器0x31—DATA_FORMAT(读/写):\SELF_TEST\SPI\INT_INVERT\0\FULL_RES\Justify\Range\*/
  SpiDelay(100);
//  Gsenser_WriteREG(0x32,0x00);/*X轴Data0(只读)*/
//  SpiDelay(100);
//  Gsenser_WriteREG(0x33,0x00);/*X轴Data1(只读)*/
//  SpiDelay(100);
//  Gsenser_WriteREG(0x34,0x00);/*Y轴Data0(只读)*/
//  SpiDelay(100);
//  Gsenser_WriteREG(0x35,0x00);/*Y轴Data1(只读)*/
//  SpiDelay(100);
//  Gsenser_WriteREG(0x36,0x00);/*Z轴Data0(只读)*/
//  SpiDelay(100);
//  Gsenser_WriteREG(0x37,0x00);/*Z轴Data1(只读)*/
//  SpiDelay(100);
  Gsenser_WriteREG(0x38,0x9F);/**/
  SpiDelay(100);
//  Gsenser_WriteREG(0x39,0x00);/*FIFO状态寄存器(只读)*/
//  SpiDelay(100);
//  TFT_ShowHEX(180,264,Gsenser_ReadREG(0x00));//读ADXL345的ID 正确值=0B11100101=0xE5=345(八进制的)
}

void Gsenser_WriteREG(INT8U REG,INT8U DATA)
{
  Gsenser_CS0;
  SpiDelay(100);
  SPI_WriteByte(REG&0X7F);//写入命令最高位为‘0’
  SpiDelay(100);
  SPI_WriteByte(DATA);
  SpiDelay(100);
  Gsenser_CS1;
  SPI_I2S_ReceiveData(SPI1)&0xff;
  SpiDelay(100);
}

INT8U Gsenser_ReadREG(INT8U REG)
{
  INT8U DATA;

  Gsenser_CS0;
  SpiDelay(1000);
  SPI_WriteByte(REG|0X80);	//读出指令最高位为‘1’
  DATA=SPI_WriteByte(0);
  Gsenser_CS1;
  SpiDelay(10);
  return DATA;
}

INT16U Gsenser_ReadREG2(INT8U REG)
{
  INT16U DATA;
  INT8U H=0,L=0;
  Gsenser_CS0;
  SpiDelay(1000);
  SPI_WriteByte(REG|0X80|0X40);	//读出指令最高位为‘1’
  L=SPI_WriteByte(0);
  H=SPI_WriteByte(0);
  Gsenser_CS1;
  SpiDelay(10);

  DATA=H;
  DATA<<=8;
  DATA+=L;
  return DATA;
}


void SHOW_SPI_REG(void)
{
 TFT_ShowHEX16(140,200,SPI1->CR1);//SPI1的控制寄存器1
 TFT_ShowHEX16(140,216,SPI1->CR2);//SPI1的控制寄存器2
 TFT_ShowHEX16(140,232,SPI1->SR); //SPI1的状态寄存器
 TFT_ShowHEX16(140,248,SPI1->DR); //SPI1的数据寄存器2
 TFT_ShowHEX16(140,264,SPI1->CRCPR);

// TFT_ShowHEX16(180,200,SPI1->RXCRCR);
// TFT_ShowHEX16(180,216,SPI1->TXCRCR);
// TFT_ShowHEX16(180,232,SPI1->I2SCFGR);
// TFT_ShowHEX16(180,248,SPI1->I2SPR);
}








