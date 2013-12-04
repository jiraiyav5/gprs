/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						  -��ͨ����ԭ������-          		          						  �~
�~ ���ߣ�Ī����																					  �~
�~   QQ: 1695627698 																			  �~
�~ ������һ��ͨ���ӿƼ�һ 																		  �~
�~ ���䣺mojianlin2011@126.com																	  �~
�~ �Ա�ר���꣺etmcu.taobao.com																	  �~
�~ ����:�˳�����Ϊ��ͨ���߼����������д�ģ�����������ҵĿ�ģ���Ҫ�����븽���ҵ���ϵ��ʽ��		  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
//��ģ�����û��Զ��幦�ܺ���

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

//��ʼ����������
void Key_Number_Init(void)
{
  INT16U temp,K[1],key16;
  INT32U Key32=888888;//���ó�ʼ����=888888
  EE_ReadVariable(2,K);//�洢�ڵ�ַ2��ֵ��Ϊ�Ƿ��ʼ��������ж�ֵ
  temp=K[0];
  if(temp!=8481)
    {
	   key16=Key32>>16;
 	   EE_WriteVariable(0,key16);   //д���ʼ����
	   key16=Key32&0x0000ffff;
	   EE_WriteVariable(1,key16);
 	   EE_WriteVariable(2,8481); //��ʼ����д���ж�ֵ���´θ�λ�Ͳ����ٳ�ʼ���ˡ�
    }
  else
    {
	 Key_Number_Renew();
	}
}

//�������������µ�����
void Key_Number_Store(void)
{
 INT16U temp;
   temp=My_Key_Number>>16;
   EE_WriteVariable(0,temp);
   temp=My_Key_Number&0x0000ffff;
   EE_WriteVariable(1,temp);
}

//��Ƭ����λ�����֮ǰ�����룬�����������My_Key_Number
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

//ͨ���ɼ���������ķ�ѹֵ���㻷���¶�
//ע�⣺ʵ���¶�=��������ֵ	- 40��
INT8S Get_Temperature(void)
{
    float ADx,x;
  	INT8U  i,FT,temp=0;
  	INT16U Rt=0;
  	INT16U ADCt=0;

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//�μ�BSP.C
	OSTimeDlyHMSM(0, 0, 0, 20);
  	ADCt=ADC_Result[ADC_TEMP];
  	
    ADx=(float)ADCt;  //Ŀ���ǰ�ת���ɸ�����
    x=10*ADx/(4095-ADx); //�������������ֵ�����ǻ����Ժ�Ĺ�ʽ
                         //[��������ADCģ���Դ=NTC��Դ]��
    x=x*100; //����100��Ҳ����ȡС��������λ
    Rt=(INT16U)x;   //����ֵ����100���󸳸�Rt��С�����ֽ�����ȥ��

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
		    if((TempTable[i-1]-Rt)<(Rt-TempTable[i])) i=i-1;  //��Rt�ӽ��ĸ�ֵ��ȡ�ĸ��ı��
	        temp=i;
	        FT=0;
	        }
    	   }
        }
return (temp-40-Temp_ADJ);	  
}

/****************************************************************************
* ��    �ƣ�unsigned char SPI_WriteByte(unsigned char data) 
* ��    �ܣ�SPI1 д����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
****************************************************************************/  
unsigned char SPI_WriteByte(unsigned char data) 
{ 
 unsigned char Data = 0; 

  //�ȴ����ͻ�������
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET); 
  // ����һ���ֽ�  
  SPI_I2S_SendData(SPI1,data); 

   //�ȴ��Ƿ���յ�һ���ֽ� 
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET); 
  // ��ø��ֽ�
  Data = SPI_I2S_ReceiveData(SPI1)&0XFF; 

  // �����յ����ֽ� 
  return Data; 
}  

/****************************************************************************
* ��    �ƣ�void SpiDelay(unsigned int DelayCnt) 
* ��    �ܣ�SPI1 д��ʱ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷�����
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

	
	LCD_SHOW_CN15x16_String(0,240,"�������ٶȴ�����:");
	
  GX=Gsenser_ReadREG2(0x32);
	LCD_SHOW_CN15x16_String(0,260,"X��:");
  TFT_ShowHEX16(35,260,GX);

  GY=Gsenser_ReadREG2(0x34);
  LCD_SHOW_CN15x16_String(85,260,"Y��:");
  TFT_ShowHEX16(120,260,GY);

  GZ=Gsenser_ReadREG2(0x36);
  LCD_SHOW_CN15x16_String(160,260,"Z��:");
  TFT_ShowHEX16(195,260,GZ);
  
  Gsenser_ReadREG(0x2B);
	
	INT_STU=Gsenser_ReadREG(0x30);

	if(INT_STU&0x70)
	  {
     LCD_SHOW_CN15x16_String(140,240,"(��⵽��)");
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
	 //Gsenser_SHOW_REG();//��ʾADXL345�ڲ����мĴ�����ֵ
    // SHOW_SPI_REG();	 //��ʾSPIģ���������ؼĴ���ֵ�����Ƿ�������ȷ
	 Gsenser_SHOW_XYZ();//��ʾX,Y,Z����������ٶ�ת��ֵ
	//Gsenser_WriteREG(0x1E,dd++);//���ڼ��SPIͨѶ�Ƿ�����
}


void ADXL345_Init(void)
{
  Gsenser_WriteREG(0x1D,0x05);/*0x1D->THRESH_TAP�Ĵ���Ϊ8λ�Ĵ����������û��жϵ���
                                ֵ�����ݸ�ʽ�޷��ţ���ˣ��û��¼��ķ�����
                                 THRESH_TAP��ֵ���бȽϣ��Ա�ʵ�������û���⡣
                                ��������Ϊ62.5mg/LSB(��0xFF =16 g)�����ʹ�ܵ���/˫
                                ���жϣ�ֵΪ0���ܻᵼ�¹����쳣��*/
  SpiDelay(100);
  Gsenser_WriteREG(0x1E,0x00);/*X��У��ֵ*/
  SpiDelay(100);
  Gsenser_WriteREG(0x1F,0x00);/*Y��У��ֵ*/
  SpiDelay(100);
  Gsenser_WriteREG(0x20,0x00);/*Z��У��ֵ*/
  SpiDelay(100);
  Gsenser_WriteREG(0x21,0x0A);/*/0x21->DURΪ8λ�Ĵ���������δǩ��ʱ��ֵ����ʾ������
                                 THRESH_TAP��ֵ֮�ϵ��¼����ܳ�֮Ϊ�û��¼�����
                                 ��ʱ�䡣��������Ϊ625 ��s/LSB��ֵΪ0ʱ�����õ���/˫�����ܡ�*/
  SpiDelay(100);
  Gsenser_WriteREG(0x22,100); /*0x22->Latent�Ĵ���Ϊ8λ�Ĵ����������޷���ʱ��ֵ����ʾ��
                                 �û��¼���⵽ʱ�䴰�ڣ���Window�Ĵ������壩��ʼ
                                 �ĵȴ�ʱ�䣬�ڴ��ڼ䣬�ܼ������ܵĵڶ����û���
                                 ������������Ϊ1.25 ms/LSB��ֵΪ0ʱ������˫�����ܡ�*/
  SpiDelay(100);
  Gsenser_WriteREG(0x23,100);/*0x23->Window�Ĵ���Ϊ8λ�Ĵ���������δǩ����ʱ��ֵ����
                                ʾ�ӳ�ʱ��(��Latent�Ĵ���ȷ��)�������ʱ�������ڴ�
                                �ڼ䣬�ܿ�ʼ���еڶ�����Ч�û�����������Ϊ1.25 
                                 ms/LSB��ֵΪ0ʱ������˫�����ܡ�*/
  SpiDelay(100);
  Gsenser_WriteREG(0x24,16);/*0x24->THRESH_ACT�Ĵ���Ϊ8λ�Ĵ���������������
                                ֵ�����ݸ�ʽ�޷��ţ���ˣ���¼��ķ�����
                                THRESH_ACT�Ĵ�����ֵ���бȽϡ���������Ϊ62.5 
                                mg/LSB�����ʹ�ܻ�жϣ�ֵΪ0ʱ�����ܵ��¹����쳣��*/
  SpiDelay(100);
  Gsenser_WriteREG(0x25,0x05);/*0x25->THRESH_INACT�Ĵ���Ϊ8λ�Ĵ����������⾲ֹ��
								��ֵ�����ݸ�ʽ�޷��ţ���ˣ���ֹ�¼��ķ�����
								THRESH_ INACT�Ĵ�����ֵ���бȽϡ���������Ϊ62.5 
								mg/LSB�����ʹ�ܾ�ֹ�жϣ�ֵΪ0ʱ�����ܵ��¹����쳣��*/
  SpiDelay(100);
  Gsenser_WriteREG(0x26,1);/*0x26->TIME_INACT�Ĵ���Ϊ8λ�Ĵ���������δǩ��ʱ��ֵ��
								��ʾ���ٶȵ�ʱ��������С��THRESH_INACT�Ĵ�����
								ֵ����������ֹ����������Ϊ1 sec /LSB���б�������ʹ
								��δ�˲�����(����ֵ����)���жϹ��ܣ��þ�ֹ���ܲ�
								���˲�������ݡ�ΪҪ�����ľ�ֹ�жϣ�������������
								һ��������������TIME_INACT�Ĵ�������ֵС����
								���������ʵ�ʱ�䳣���������¹��ܷ�Ӧ�ٶۡ������
								����С��THRESH_INACT�Ĵ�����ֵ��ֵΪ0���¹����쳣��*/
  SpiDelay(100);
  Gsenser_WriteREG(0x27,0xFF);/*ACT����/ֱ��|INACT����/ֱ��|ACT_Xʹ��|INACT_Xʹ��|ACT_Yʹ��|INACT_Yʹ��|ACT_Zʹ��|INACT_Zʹ��*/
  SpiDelay(100);
  Gsenser_WriteREG(0x28,0x09);/*0x28->THRESH_FF�Ĵ���Ϊ8λ�Ĵ���������δǩ����ʽ����
								ֵ���������������⡣������ļ��ٶ���THRESH_FF
								��ֵ��Ƚϣ���ȷ���Ƿ������������¼���������������
								Ϊ62.5 mg/LSB����ע�⣬������������жϱ�ʹ�ܣ�ֵΪ
								0 mg���ܵ��¹����쳣���������300 mg��600 mg(0x05��0x09)֮���ֵ��*/
  SpiDelay(100);
  Gsenser_WriteREG(0x29,0xFF);/*0x29->TIME_FF�Ĵ���Ϊ8λ�Ĵ������洢δǩ��ֵ����ʾ����
								���ֵ����С��THRESH_FF����Сʱ�䣬������������
								���жϡ���������Ϊ5 ms/LSB�����ʹ�����������жϣ�
								ֵΪ0ʱ�����ܵ��¹����쳣���������100 ms��350 ms(0x14��0x46)֮���ֵ��*/
  SpiDelay(100);
  Gsenser_WriteREG(0x2A,0x07);/*0x2A->�û����ƿ��ƼĴ������μ������ֲ᡿*/
  SpiDelay(100);
//  Gsenser_WriteREG(0x2B,0x00);/*0x2B->[״̬�Ĵ���][ֻ��]|0|ACT_X��Դ|ACT_Y��Դ|ACT_Z��Դ|����|TAP_X��Դ|TAP_Y��Դ|TAP_Z��Դ*/
//  SpiDelay(100);
  Gsenser_WriteREG(0x2C,0x0A);/*0x2C->���ĺ�ת���������ã�Ĭ��ֵΪ0x0A��ת��Ϊ100Hz������������ʡ�Ӧѡ
								���ʺ���ѡͨ��Э���Ƶ�ʵ�����������ʡ�ѡ��̫��
								����������ʺ͵�ͨ���ٶȻᵼ�²���*/
  SpiDelay(100);
  Gsenser_WriteREG(0x2D,0x28);/*0x2D->���Ŀ��ƼĴ�����|0|0|AUTO_SLEEP|����|����|����|����[1:0]|*/
  SpiDelay(100);
  Gsenser_WriteREG(0x2E,0xFF);/*�ж�ʹ�ܼĴ������μ������ֲ᡿*/
  SpiDelay(100);
  Gsenser_WriteREG(0x2F,0x00);/*�ж�����������üĴ������μ������ֲ᡿*/
  SpiDelay(100);
//  Gsenser_WriteREG(0x30,0x00);/*�Ĵ���0x30��INT_SOURCE(ֻ��)*/
//  SpiDelay(100);
  Gsenser_WriteREG(0x31,0x2B);/*�Ĵ���0x31��DATA_FORMAT(��/д):\SELF_TEST\SPI\INT_INVERT\0\FULL_RES\Justify\Range\*/
  SpiDelay(100);
//  Gsenser_WriteREG(0x32,0x00);/*X��Data0(ֻ��)*/
//  SpiDelay(100);
//  Gsenser_WriteREG(0x33,0x00);/*X��Data1(ֻ��)*/
//  SpiDelay(100);
//  Gsenser_WriteREG(0x34,0x00);/*Y��Data0(ֻ��)*/
//  SpiDelay(100);
//  Gsenser_WriteREG(0x35,0x00);/*Y��Data1(ֻ��)*/
//  SpiDelay(100);
//  Gsenser_WriteREG(0x36,0x00);/*Z��Data0(ֻ��)*/
//  SpiDelay(100);
//  Gsenser_WriteREG(0x37,0x00);/*Z��Data1(ֻ��)*/
//  SpiDelay(100);
  Gsenser_WriteREG(0x38,0x9F);/**/
  SpiDelay(100);
//  Gsenser_WriteREG(0x39,0x00);/*FIFO״̬�Ĵ���(ֻ��)*/
//  SpiDelay(100);
//  TFT_ShowHEX(180,264,Gsenser_ReadREG(0x00));//��ADXL345��ID ��ȷֵ=0B11100101=0xE5=345(�˽��Ƶ�)
}

void Gsenser_WriteREG(INT8U REG,INT8U DATA)
{
  Gsenser_CS0;
  SpiDelay(100);
  SPI_WriteByte(REG&0X7F);//д���������λΪ��0��
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
  SPI_WriteByte(REG|0X80);	//����ָ�����λΪ��1��
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
  SPI_WriteByte(REG|0X80|0X40);	//����ָ�����λΪ��1��
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
 TFT_ShowHEX16(140,200,SPI1->CR1);//SPI1�Ŀ��ƼĴ���1
 TFT_ShowHEX16(140,216,SPI1->CR2);//SPI1�Ŀ��ƼĴ���2
 TFT_ShowHEX16(140,232,SPI1->SR); //SPI1��״̬�Ĵ���
 TFT_ShowHEX16(140,248,SPI1->DR); //SPI1�����ݼĴ���2
 TFT_ShowHEX16(140,264,SPI1->CRCPR);

// TFT_ShowHEX16(180,200,SPI1->RXCRCR);
// TFT_ShowHEX16(180,216,SPI1->TXCRCR);
// TFT_ShowHEX16(180,232,SPI1->I2SCFGR);
// TFT_ShowHEX16(180,248,SPI1->I2SPR);
}







