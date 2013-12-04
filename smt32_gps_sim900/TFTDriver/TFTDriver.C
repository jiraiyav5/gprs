
#include "TFTDriver.h"
#include "ASCII_Lib.h"

//unsigned short int TFT_Y=0;
//unsigned char  TFT_X=0;



///////////////////////////////////////////////////////////
void DelayLCD(unsigned long nCount)
{ 
     while(nCount--);
}
/*������������������������������������������������������������������������������������������
�� ��    �ƣ�void TFT_CtrlLinesConfig(void)
�� ��    �ܣ�����FSMCģ������Ź���
������������������������������������������������������������������������������������������*/
void TFT_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 /*
	��λ����Ϊ���
	TFT_RST -->PB1    
	����Ϊ����
	TFT_CS --> PD7
	TFT_RD --> PD4
	TFT_WR --> PD5
	TFT_RS --> PD11
	������
    D0   -->   PD14
    D1   -->   PD15
	D2   -->   PD0
	D3   -->   PD1
	D4   -->   PE7
	D5   -->   PE8
	D6   -->   PE9
	D7   -->   PE10
	D8   -->   PE11
	D9   -->   PE12
	D10  -->   PE13
	D11  -->   PE14
	D12  -->   PE15
	D13  -->   PD8
	D14  -->   PD9
	D15  -->   PD10
*/
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|
								GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9| GPIO_Pin_10|
								GPIO_Pin_11|GPIO_Pin_14|GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |GPIO_Pin_8 |GPIO_Pin_9 |GPIO_Pin_10 | 
                                GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

}
/*������������������������������������������������������������������������������������������
�� ��    �ƣ�void TFT_FSMCConfig(void)
�� ��    �ܣ�����FSMCģ��
������������������������������������������������������������������������������������������*/
void TFT_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
  
  p.FSMC_AddressSetupTime = 0x02;
  p.FSMC_AddressHoldTime = 0x00;
  p.FSMC_DataSetupTime = 0x05;
  p.FSMC_BusTurnAroundDuration = 0x00;
  p.FSMC_CLKDivision = 0x00;
  p.FSMC_DataLatency = 0x00;
  p.FSMC_AccessMode = FSMC_AccessMode_B;

  FSMC_NORSRAMInitStructure.FSMC_Bank                 = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux       = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType           = FSMC_MemoryType_NOR;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth      = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode      = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity   = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode             = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive     = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation       = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal           = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode         = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst           = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct    = &p;	  

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE); 
  }
/*������������������������������������������������������������������������������������������
�� ��    �ƣ�void TFT_Init(void)
�� ��    �ܣ�TFT��ʼ��
������������������������������������������������������������������������������������������*/
void TFT_Init(void)
{
    TFT_CtrlLinesConfig();
    TFT_FSMCConfig(); 
	
	TFT_RST0;
	DelayLCD(100000);
	TFT_RST1;
	DelayLCD(100000);
#if defined TFT_DRIVER_ILI9320
    TFT_Send_REG(0x00E5, 0x8000);     // Set the internal vcore voltage                                  
    TFT_Send_REG(0x0000, 0x0001);     // Start internal OSC. 
    TFT_Send_REG(0x0001, 0x0100);    // set SS and SM bit 
    TFT_Send_REG(0x0002, 0x0700);    // set 1 line inversion 
    TFT_Send_REG(0x0003, 0x1030);     // set GRAM write direction and BGR=1.  
    TFT_Send_REG(0x0004, 0x0000);    // Resize register 
    TFT_Send_REG(0x0008, 0x0202);     // set the back porch and front porch 
    TFT_Send_REG(0x0009, 0x0000);     // set non-display area refresh cycle ISC[3:0] 
    TFT_Send_REG(0x000A, 0x0000);    // FMARK function 
    TFT_Send_REG(0x000C, 0x0000);         // RGB interface setting 
    TFT_Send_REG(0x000D, 0x0000);    // Frame marker Position 
    TFT_Send_REG(0x000F, 0x0000);         // RGB interface polarity 
   
  //*************Power On sequence ****************// 
    TFT_Send_REG(0x0010, 0x0000);          // SAP, BT[3:0], AP, DSTB, SLP, STB 
    TFT_Send_REG(0x0011, 0x0007);         // DC1[2:0], DC0[2:0], VC[2:0] 
    TFT_Send_REG(0x0012, 0x0000);         // VREG1OUT voltage 
    TFT_Send_REG(0x0013, 0x0000);          // VDV[4:0] for VCOM amplitude 
      DelayLCD(200);                                // Dis-charge capacitor power voltage 
    TFT_Send_REG(0x0010, 0x17B0);          // SAP, BT[3:0], AP, DSTB, SLP, STB 
    TFT_Send_REG(0x0011, 0x0147);         // DC1[2:0], DC0[2:0], VC[2:0] 
      DelayLCD(50);              // Delay 50ms   
    TFT_Send_REG(0x0012, 0x013C);         // VREG1OUT voltage 
      DelayLCD(50);              // Delay 50ms 
    TFT_Send_REG(0x0013, 0x0E00);          // VDV[4:0] for VCOM amplitude 
    TFT_Send_REG(0x0029, 0x0009);         // VCM[4:0] for VCOMH 
      DelayLCD(50);     
    TFT_Send_REG(0x0020, 0x0000);          // GRAM horizontal Address 
    TFT_Send_REG(0x0021, 0x0000);          // GRAM Vertical Address 
   
  // ----------- Adjust the Gamma  Curve ----------// 
    TFT_Send_REG(0x0030, 0x0207);   
    TFT_Send_REG(0x0031, 0x0505);   
    TFT_Send_REG(0x0032, 0x0102); 
    TFT_Send_REG(0x0035, 0x0006);   
    TFT_Send_REG(0x0036, 0x0606);   
    TFT_Send_REG(0x0037, 0x0707);   
    TFT_Send_REG(0x0038, 0x0506);   
    TFT_Send_REG(0x0039, 0x0407); 
    TFT_Send_REG(0x003C, 0x0106);   
    TFT_Send_REG(0x003D, 0x0601);  
  //------------------ Set GRAM area ---------------// 
    TFT_Send_REG(0x0050, 0x0000);     // Horizontal GRAM Start Address 
    TFT_Send_REG(0x0051, 0x00EF);     // Horizontal GRAM End Address 
    TFT_Send_REG(0x0052, 0x0000);     // Vertical GRAM Start Address 
	  TFT_Send_REG(0x0053, 0x013F);     // Vertical GRAM Start Address 
       
    TFT_Send_REG(0x0060, 0x2700);     // Gate Scan Line 
    TFT_Send_REG(0x0061, 0x0001);     // NDL,VLE, REV 
    TFT_Send_REG(0x006A, 0x0000);     // set scrolling line 
   
  //-------------- Partial Display Control ---------// 
    TFT_Send_REG(0x0080, 0x0000); 
    TFT_Send_REG(0x0081, 0x0000); 
    TFT_Send_REG(0x0082, 0x0000); 
    TFT_Send_REG(0x0083, 0x0000); 
    TFT_Send_REG(0x0084, 0x0000); 
    TFT_Send_REG(0x0085, 0x0000); 
     
  //-------------- Panel Control -------------------// 
    TFT_Send_REG(0x0090, 0x0010); 
    TFT_Send_REG(0x0092, 0x0000); 
    TFT_Send_REG(0x0093, 0x0003); 
    TFT_Send_REG(0x0095, 0x0110); 
    TFT_Send_REG(0x0097, 0x0000); 
    TFT_Send_REG(0x0098, 0x0000); 

    TFT_Send_REG(0x0007, 0x0173);     // 262K color and display ON 

#elif defined TFT_DRIVER_ILI9328

 //************* Start Initial Sequence **********// 
    TFT_Send_REG(0x0001, 0x0100);      // set SS and SM bit 
    TFT_Send_REG(0x0002, 0x0700);      // set 1 line inversion 
    TFT_Send_REG(0x0003, 0x1030);      // set GRAM write direction and BGR=1.  
    TFT_Send_REG(0x0004, 0x0000);      // Resize register 
    TFT_Send_REG(0x0008, 0x0207);      // set the back porch and front porch 
    TFT_Send_REG(0x0009, 0x0000);      // set non-display area refresh cycle ISC[3:0] 
    TFT_Send_REG(0x000A, 0x0000);      // FMARK function 
    TFT_Send_REG(0x000C, 0x0000);      // RGB interface setting 
    TFT_Send_REG(0x000D, 0x0000);      // Frame marker Position 
    TFT_Send_REG(0x000F, 0x0000);      // RGB interface polarity 
  //*************Power On sequence ****************// 
    TFT_Send_REG(0x0010, 0x0000);      // SAP, BT[3:0], AP, DSTB, SLP, STB 
    TFT_Send_REG(0x0011, 0x0007);      // DC1[2:0], DC0[2:0], VC[2:0] 
    TFT_Send_REG(0x0012, 0x0000);      // VREG1OUT voltage 
    TFT_Send_REG(0x0013, 0x0000);      // VDV[4:0] for VCOM amplitude 
    TFT_Send_REG(0x0007, 0x0001); 
      DelayLCD(20000);                            // Dis-charge capacitor power voltage 
    TFT_Send_REG(0x0010, 0x1290);      // SAP, BT[3:0], AP, DSTB, SLP, STB 
    TFT_Send_REG(0x0011, 0x0227);      // DC1[2:0], DC0[2:0], VC[2:0] 
      DelayLCD(5000);                       // Delay 50ms   
    TFT_Send_REG(0x0012, 0x001A);      // Internal reference voltage= Vci; 
      DelayLCD(5000);                       // Delay 50ms 
    TFT_Send_REG(0x0013, 0x1800);      // Set VDV[4:0] for VCOM amplitude 
    TFT_Send_REG(0x0029, 0x0028);      // Set VCM[5:0] for VCOMH 
    TFT_Send_REG(0x002B, 0x000C);     // Set Frame Rate  
      DelayLCD(5000);                             // Delay 50ms 
    TFT_Send_REG(0x0020, 0x0000);      // GRAM horizontal Address 
    TFT_Send_REG(0x0021, 0x0000);      // GRAM Vertical Address  
  // ----------- Adjust the Gamma  Curve ----------// 
    TFT_Send_REG(0x0030, 0x0000); 
    TFT_Send_REG(0x0031, 0x0305); 
    TFT_Send_REG(0x0032, 0x0003); 
    TFT_Send_REG(0x0035, 0x0304); 
    TFT_Send_REG(0x0036, 0x000F); 
    TFT_Send_REG(0x0037, 0x0407); 
    TFT_Send_REG(0x0038, 0x0204); 
    TFT_Send_REG(0x0039, 0x0707); 
    TFT_Send_REG(0x003C, 0x0403); 
    TFT_Send_REG(0x003D, 0x1604); 
    //------------------ Set GRAM area ---------------// 
    TFT_Send_REG(0x0050, 0x0000);     // Horizontal GRAM Start Address 
    TFT_Send_REG(0x0051, 0x00EF);     // Horizontal GRAM End Address 
    TFT_Send_REG(0x0052, 0x0000);     // Vertical GRAM Start Address 
    TFT_Send_REG(0x0053, 0x013F);     // Vertical GRAM Start Address 
    TFT_Send_REG(0x0060, 0xA700);     // Gate Scan Line 
    TFT_Send_REG(0x0061, 0x0001);     // NDL,VLE, REV 
    TFT_Send_REG(0x006A, 0x0000);     // set scrolling line 
    //-------------- Partial Display Control ---------// 
    TFT_Send_REG(0x0080, 0x0000); 
    TFT_Send_REG(0x0081, 0x0000); 
    TFT_Send_REG(0x0082, 0x0000); 
    TFT_Send_REG(0x0083, 0x0000); 
    TFT_Send_REG(0x0084, 0x0000); 
    TFT_Send_REG(0x0085, 0x0000); 
    //-------------- Panel Control -------------------// 
    TFT_Send_REG(0x0090, 0x0010); 
    TFT_Send_REG(0x0092, 0x0600); 
 
    TFT_Send_REG(0x0007, 0x0133);     // 262K color and display ON 		
		
		DelayLCD(5000);  
#endif

	
//	DelayLCD(100000);
//   	Clear_TFT();

}
	
/****************************************************************************
* ��    �ƣ�void TFT_SetCursor(u16 x,u16 y)
* ��    �ܣ�������Ļ����
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������
****************************************************************************/
//__inline void TFT_SetCursor(u16 x,u16 y)
//{
//	TFT_Send_REG(0x0020,x); // ��
// 	TFT_Send_REG(0x0021,y); // ��
//    TFT_SHOW_START();		
//}
/****************************************************************************
* ��    �ƣ�u16 TFT_GetPoint(u16 x,u16 y)
* ��    �ܣ���ȡָ���������ɫֵ
* ��ڲ�����x      ������
*           y      ������
* ���ڲ�������ǰ������ɫֵ
****************************************************************************/
void TFT_SET_Window(u16 x0,u16 y0,u16 x1,u16 y1)
{
  TFT_Send_REG(X_Sart_REG,x0);
  TFT_Send_REG(Y_Sart_REG,y0);
  TFT_Send_REG(X_END_REG ,x1);
  TFT_Send_REG(Y_END_REG ,y1);
}
/*��������������������������������������������������������������������
�� ��    �ƣ�u16 TFT_GetPoint(u16 x,u16 y)
�� ��    �ܣ���ȡָ���������ɫֵ
�� ��ڲ�����x      ������
��           y      ������
�� ���ڲ�������ǰ������ɫֵ
��������������������������������������������������������������������*/
u16 TFT_GetPoint(u16 x,u16 y)
{
  u16 a=0;
  TFT_SetCursor(x,y);
  a=*(volatile u16 *)(Bank1_TFT_D); 
  return(a);
}
/*��������������������������������������������������������������������
�� ��    �ƣ�void Clear_TFT(void)
�� ��    �ܣ��ñ�����ɫ����
��������������������������������������������������������������������*/
void Clear_TFT(void)
{
 unsigned long n,L;
 TFT_SetCursor(0,0);
 L=TFT_MAX_X*TFT_MAX_Y; 
 for(n=0;n<L;n++){ TFT_WriteRAM(Back_color); }
}
/*��������������������������������������������������������������������
�� ��    �ƣ�void Set_Font_Type(unsigned short int type)
�� ��    �ܣ�����ASCII�ַ���С
�� ��ڲ�����812 =8*12�������С��
             816 =8*16�������С��
��           1624=16*24�������С
��������������������������������������������������������������������*/
void Set_Font_Type(unsigned short int type)
{ 
  FontType=type;
  if(type==816)           {TypeX=8;   TypeY=16; }
  else if(type==812)      {TypeX=8;   TypeY=12; }
       else if(type==1624){TypeX=16;  TypeY=24; }
            else          {TypeX=8;   TypeY=16; }
}
/*����������������������������������������������������������������������������������������
�� ��    �ƣ�void TFT_ShowChar(unsigned char x,unsigned short int y,unsigned char num)
�� ��    �ܣ���ʾһ��ASCII�ַ� 
�� ��ڲ�����
��           
�� ���ڲ�����
����������������������������������������������������������������������������������������*/
void TFT_ShowChar(unsigned char x,unsigned short int y,unsigned char num)
{       
    unsigned char temp;
    unsigned char pos,t; 
if(FontType==812)//��ʾ8x12�����ASCII���ַ� 
{   
    if(x+7>TFT_MAX_X||y+11>TFT_MAX_Y) return;//�ж��Ƿ񳬳�TFT�߽�    
	TFT_SET_Window(x,y,x+7,y+11);
	TFT_SetCursor( x,y);
	num=num-' ';//��Ϊ���������Ǵӿո�ʼ�ģ�����Ҫ�����ո����õ�����ڿո��ƫ�������Ӷ��õ���Ӧ�ַ��ĵ������ݡ�
	if(num>94) num=95;
	for(pos=0;pos<12;pos++)
	{
	    temp=ascii0612[num][pos];
	    for(t=0;t<8;t++)
	    {                 
	        if(temp&0x80) TFT_WriteRAM(FONT_color);//������ɫ
	        else          TFT_WriteRAM(Back_color);//����ɫ    
	        temp<<=1; 
	    }
	}

}
if(FontType==816)//��ʾ8x16�����ASCII���ַ� 
{   
    if(x+7>TFT_MAX_X||y+15>TFT_MAX_Y) return; //�ж��Ƿ񳬳�TFT�߽�    
    TFT_SET_Window(x,y,x+7,y+15);
	TFT_SetCursor( x,y);

	if(num>=' '&&num<='~')num=num- ' ';//�õ�ƫ�ƺ��ֵ
    else{
	     if(num=='\r')num=97;
		 else if(num=='\n')num=98;
		      else num=96;
	    }
	 
	for(pos=0;pos<16;pos++)
	{
	    temp=ascii0816[num][pos];
	    for(t=0;t<8;t++)
	    {                 
	        if(temp&0x80) TFT_WriteRAM(FONT_color);//������ɫ
	        else          TFT_WriteRAM(Back_color);//����ɫ    
	        temp<<=1; 
	    }
	}
}	
if(FontType==1624) //��ʾ16x24�����ASCII���ַ�
 {  
    if(x+15>TFT_MAX_X||y+23>TFT_MAX_Y) return; //�ж��Ƿ񳬳�TFT�߽�      
	TFT_SET_Window(x,y,x+15,y+23);
	TFT_SetCursor(x,y);
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	if(num>94) num=95;
	for(pos=0;pos<48;pos++)
	{
	    temp=ascii1624[num][pos];
	    for(t=0;t<8;t++)
	    {                 
	        if(temp&0x80) TFT_WriteRAM(FONT_color);//������ɫ
	        else          TFT_WriteRAM(Back_color);//����ɫ    
	        temp<<=1; 
	    }
	}
 }
}
/*������������������������������������������������������������������������������������
�� ��    �ƣ�void TFT_ShowNum(unsigned char  x,unsigned short int  y,long  num)
�� ��    �ܣ���ָ��������ʾ����
�� ��ڲ�����x      ������
��           y      ������
��           num    ����
������������������������������������������������������������������������������������*/
void TFT_ShowNum(unsigned char  x,unsigned short int  y,long  num)
{      
	long res;   	   
	unsigned char t=0,t1=0,t2=0;   
	res=num;
	if(!num)TFT_ShowChar(x,y,'0');//��ʾ0
	while(res)  //�õ����ֳ���
	{
		res/=10;
		t++;
	}
	t1=t;
	while(t)	//��ʾ����
	{	
		res=1;
		t2=t;
		while(t2-1)
		{
			res*=10;
			t2--;
		}
	    TFT_ShowChar(x+(t1-t)*TypeX,y,(num/res)%10+'0');
		t--;
	}				     
}
/*����������������������������������������������������������������������������������������
�� ��    �ƣ�char TFT_Show3Num(unsigned char x,unsigned short  int y,unsigned char num)
�� ��    �ܣ���ʾһ��3λ��ʮ��������0~255��
�� ��ڲ�����x      ������
��           y      ������
��           num    ����
�� ���ڲ������Ƿ������
����������������������������������������������������������������������������������������*/
char TFT_Show3Num(unsigned char x,unsigned short  int y,unsigned char num)
{         
   	unsigned char g,s,b;
   	if(num>255) return 0;
	
	b=num/100;
	s=(num/10)%10;
	g=num%10;
   
    if(b)TFT_ShowChar(x,y,b+'0');
	else TFT_ShowChar(x,y,' '); 
    
	if(s||b)TFT_ShowChar(x+TypeX,y,s+'0');
	else    TFT_ShowChar(x+TypeX,y,' ');

	TFT_ShowChar(x+TypeX*2,y,g+'0');   

 return 1;
}
/*����������������������������������������������������������������������������������������������
�� ��    �ƣ�void TFT_Show5Num(unsigned char x,unsigned short int y,unsigned short int num)
�� ��    �ܣ���ʾһ��5λ��ʮ��������0~65535��
�� ��ڲ�����x      ������
��           y      ������
��           num    ����
����������������������������������������������������������������������������������������������*/
void TFT_Show5Num(unsigned char x,unsigned short int y,unsigned short int num)
{         
    TFT_ShowChar(x,   y,num/10000+'0');
    TFT_ShowChar(x+TypeX, y,(num/1000)%10+'0'); 
    TFT_ShowChar(x+TypeX*2, y,(num/100)%10+'0'); 
    TFT_ShowChar(x+TypeX*3, y,(num/10)%10+'0'); 
    TFT_ShowChar(x+TypeX*4,y,num%10+'0');
}
/*��������������������������������������������������������������������������������������
�� ��    �ƣ�void TFT_ShowHEX(unsigned char x,unsigned short int y,unsigned char num)
�� ��    �ܣ���ʾһ��2λ��ʮ����������0~255��
�� ��ڲ�����x      ������
��           y      ������
��           num    ����
��������������������������������������������������������������������������������������*/
void TFT_ShowHEX(unsigned char x,unsigned short int y,unsigned char num)
  {   
    unsigned char H,L;	
	H=num>>4;
	L=num&0X0F;
	if(H<10) TFT_ShowChar(x,y,H+'0');
      else   TFT_ShowChar(x,y,H+55);
    if(L<10) TFT_ShowChar(x+TypeX,y,L+'0');
      else   TFT_ShowChar(x+TypeX,y,L+55); 
  }

/*��������������������������������������������������������������������������������������
�� ��    �ƣ�void TFT_ShowHEX16(unsigned char x,unsigned short int y,unsigned char num)
�� ��    �ܣ���ʾһ��4λ��ʮ����������0~255��
�� ��ڲ�����x      ������
��           y      ������
��           num    ����
��������������������������������������������������������������������������������������*/
void TFT_ShowHEX16(unsigned char x,unsigned short int y,unsigned short int num)
  {   
    unsigned char temp,H,L;
	temp=(unsigned char)(num>>8);	
	H=temp>>4;
	L=temp&0X0F;
	if(H<10) TFT_ShowChar(x,y,H+'0');
      else   TFT_ShowChar(x,y,H+55);
    if(L<10) TFT_ShowChar(x+TypeX,y,L+'0');
      else   TFT_ShowChar(x+TypeX,y,L+55); 

	temp=(unsigned char)(num&0x00ff);	
	H=temp>>4;
	L=temp&0X0F;
	if(H<10) TFT_ShowChar(x+2*TypeX,y,H+'0');
      else   TFT_ShowChar(x+2*TypeX,y,H+55);
    if(L<10) TFT_ShowChar(x+3*TypeX,y,L+'0');
      else   TFT_ShowChar(x+3*TypeX,y,L+55);
  }


/*������������������������������������������������������������������������������������������������������������
�� ��    �ƣ�void TFT_ShowString(unsigned char x,unsigned  short  int y,const unsigned char *p)
�� ��    �ܣ���ʾһ���ַ���
�� ��ڲ�����x      ������
��           y      ������
��           num    ����
��           *p     �ַ����׵�ַ,ָ��ROM�ռ���ַ���ָ��
������������������������������������������������������������������������������������������������������������*/
void TFT_ShowString(unsigned char x,unsigned  short  int y,const unsigned char *p)
{         
    while(*p!='\0')
    {       
        if(x+TypeX>240)//�Զ�����
              {x=0;
	           y+=TypeY;
               }
        if(y>TFT_MAX_Y){y=x=0;Clear_TFT();}
        TFT_ShowChar(x,y,*p);
        x+=TypeX;
        p++;
    }  
}
/*������������������������������������������������������������������������������������������
�� ��    �ƣ�void TFT_RAMString(unsigned char x,unsigned  short  int y,unsigned char *p)
�� ��    �ܣ���ʾһ����RAM�е��ַ���
�� ��ڲ�����x      ������
��           y      ������
��           num    ����
��           *p     �ַ����׵�ַ,ָ��RAM�ռ���ַ���ָ��
������������������������������������������������������������������������������������������*/
void TFT_RAMString(unsigned char x,unsigned  short  int y,unsigned char *p)
{         
    while(*p!='\0')
    {       
        if(x+TypeX>240)//�Զ�����
              {x=0;
	           y+=TypeY;
               }
        if(y>TFT_MAX_Y){y=x=0;Clear_TFT();}
        TFT_ShowChar(x,y,*p);
        x+=TypeX;
        p++;
    }      
}
/*������������������������������������������������������������������������������������������
�� ��    �ƣ� TFT_ShowPicture()
�� ��    �ܣ� ��ʾһ��ͼƬ
�� ��ڲ����� x       ͼƬ�����Ͻ�������
��            y       ͼƬ�����Ͻ�������
��            w       ͼƬ�Ŀ��� 
��            h       ͼƬ�ĸ߶�  
��            *image  ͼƬ���ݵ����ֽڵ�ַ
������������������������������������������������������������������������������������������*/
void TFT_ShowPicture(unsigned char x,unsigned short  int y,unsigned char w,unsigned short int h,const unsigned char  *image)
{ unsigned int i,LONG;
  unsigned  short  int  *p;
  p= (unsigned  short  int*)image;

	TFT_SET_Window(x,y,x+w-1,y+h-1);
	TFT_SetCursor( x,y);

    LONG=w*h*2-1;
  for(i=0;i<LONG;i+=2)
    {
	 TFT_WriteRAM(*p++);
    }
}
/*������������������������������������������������������������������������������������������
�� ��    �ƣ� TFT_ShowPicture2()
�� ��    �ܣ� ��ʾһ��ͼƬ�����ڴ��ģʽ��С��ģʽ�����ݣ����������᷽��һ�㣬����ִ���ٶȻ�����һ�㡣
�� ��ڲ����� x       ͼƬ�����Ͻ�������
��            y       ͼƬ�����Ͻ�������
��            w       ͼƬ�Ŀ��� 
��            h       ͼƬ�ĸ߶�  
��            *image  ͼƬ���ݵ����ֽڵ�ַ
������������������������������������������������������������������������������������������*/
void TFT_ShowPicture2(unsigned char x,unsigned short int y,unsigned char w,unsigned short int h,const unsigned char  *image)
{ unsigned int i,LONG;
  union _Data{unsigned short int B16; unsigned char B8[2];}Data;

	TFT_SET_Window(x,y,x+w-1,y+h-1);
	TFT_SetCursor( x,y);

    LONG=w*h*2-1;
  for(i=0;i<LONG;i+=2)
    {
 	 Data.B8[1]=*image++;
	 Data.B8[0]=*image++;	 
	 
	 TFT_WriteRAM(Data.B16);
    }
}


















