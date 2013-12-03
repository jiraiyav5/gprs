
#include "TFTDriver.h"
#include "ASCII_Lib.h"

//unsigned short int TFT_Y=0;
//unsigned char  TFT_X=0;



///////////////////////////////////////////////////////////
void DelayLCD(unsigned long nCount)
{ 
     while(nCount--);
}
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：void TFT_CtrlLinesConfig(void)
※ 功    能：配置FSMC模块的引脚功能
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
void TFT_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 /*
	复位线设为输出
	TFT_RST -->PB1    
	以下为复用
	TFT_CS --> PD7
	TFT_RD --> PD4
	TFT_WR --> PD5
	TFT_RS --> PD11
	数据线
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
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：void TFT_FSMCConfig(void)
※ 功    能：配置FSMC模块
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
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
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：void TFT_Init(void)
※ 功    能：TFT初始化
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
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
* 名    称：void TFT_SetCursor(u16 x,u16 y)
* 功    能：设置屏幕座标
* 入口参数：x      行座标
*           y      列座标
* 出口参数：无
****************************************************************************/
//__inline void TFT_SetCursor(u16 x,u16 y)
//{
//	TFT_Send_REG(0x0020,x); // 行
// 	TFT_Send_REG(0x0021,y); // 列
//    TFT_SHOW_START();		
//}
/****************************************************************************
* 名    称：u16 TFT_GetPoint(u16 x,u16 y)
* 功    能：获取指定座标的颜色值
* 入口参数：x      行座标
*           y      列座标
* 出口参数：当前座标颜色值
****************************************************************************/
void TFT_SET_Window(u16 x0,u16 y0,u16 x1,u16 y1)
{
  TFT_Send_REG(X_Sart_REG,x0);
  TFT_Send_REG(Y_Sart_REG,y0);
  TFT_Send_REG(X_END_REG ,x1);
  TFT_Send_REG(Y_END_REG ,y1);
}
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：u16 TFT_GetPoint(u16 x,u16 y)
※ 功    能：获取指定座标的颜色值
※ 入口参数：x      行座标
※           y      列座标
※ 出口参数：当前座标颜色值
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
u16 TFT_GetPoint(u16 x,u16 y)
{
  u16 a=0;
  TFT_SetCursor(x,y);
  a=*(volatile u16 *)(Bank1_TFT_D); 
  return(a);
}
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：void Clear_TFT(void)
※ 功    能：用背景颜色清屏
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
void Clear_TFT(void)
{
 unsigned long n,L;
 TFT_SetCursor(0,0);
 L=TFT_MAX_X*TFT_MAX_Y; 
 for(n=0;n<L;n++){ TFT_WriteRAM(Back_color); }
}
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：void Set_Font_Type(unsigned short int type)
※ 功    能：设置ASCII字符大小
※ 入口参数：812 =8*12的字体大小；
             816 =8*16的字体大小；
※           1624=16*24的字体大小
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
void Set_Font_Type(unsigned short int type)
{ 
  FontType=type;
  if(type==816)           {TypeX=8;   TypeY=16; }
  else if(type==812)      {TypeX=8;   TypeY=12; }
       else if(type==1624){TypeX=16;  TypeY=24; }
            else          {TypeX=8;   TypeY=16; }
}
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：void TFT_ShowChar(unsigned char x,unsigned short int y,unsigned char num)
※ 功    能：显示一个ASCII字符 
※ 入口参数：
※           
※ 出口参数：
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
void TFT_ShowChar(unsigned char x,unsigned short int y,unsigned char num)
{       
    unsigned char temp;
    unsigned char pos,t; 
if(FontType==812)//显示8x12点阵的ASCII码字符 
{   
    if(x+7>TFT_MAX_X||y+11>TFT_MAX_Y) return;//判断是否超出TFT边界    
	TFT_SET_Window(x,y,x+7,y+11);
	TFT_SetCursor( x,y);
	num=num-' ';//因为点阵数据是从空格开始的，所以要减掉空格来得到相对于空格的偏移量，从而得到相应字符的点阵数据。
	if(num>94) num=95;
	for(pos=0;pos<12;pos++)
	{
	    temp=ascii0612[num][pos];
	    for(t=0;t<8;t++)
	    {                 
	        if(temp&0x80) TFT_WriteRAM(FONT_color);//字体颜色
	        else          TFT_WriteRAM(Back_color);//背景色    
	        temp<<=1; 
	    }
	}

}
if(FontType==816)//显示8x16点阵的ASCII码字符 
{   
    if(x+7>TFT_MAX_X||y+15>TFT_MAX_Y) return; //判断是否超出TFT边界    
    TFT_SET_Window(x,y,x+7,y+15);
	TFT_SetCursor( x,y);

	if(num>=' '&&num<='~')num=num- ' ';//得到偏移后的值
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
	        if(temp&0x80) TFT_WriteRAM(FONT_color);//字体颜色
	        else          TFT_WriteRAM(Back_color);//背景色    
	        temp<<=1; 
	    }
	}
}	
if(FontType==1624) //显示16x24点阵的ASCII码字符
 {  
    if(x+15>TFT_MAX_X||y+23>TFT_MAX_Y) return; //判断是否超出TFT边界      
	TFT_SET_Window(x,y,x+15,y+23);
	TFT_SetCursor(x,y);
	num=num-' ';//得到偏移后的值
	if(num>94) num=95;
	for(pos=0;pos<48;pos++)
	{
	    temp=ascii1624[num][pos];
	    for(t=0;t<8;t++)
	    {                 
	        if(temp&0x80) TFT_WriteRAM(FONT_color);//字体颜色
	        else          TFT_WriteRAM(Back_color);//背景色    
	        temp<<=1; 
	    }
	}
 }
}
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：void TFT_ShowNum(unsigned char  x,unsigned short int  y,long  num)
※ 功    能：在指定座标显示数字
※ 入口参数：x      行座标
※           y      列座标
※           num    数字
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
void TFT_ShowNum(unsigned char  x,unsigned short int  y,long  num)
{      
	long res;   	   
	unsigned char t=0,t1=0,t2=0;   
	res=num;
	if(!num)TFT_ShowChar(x,y,'0');//显示0
	while(res)  //得到数字长度
	{
		res/=10;
		t++;
	}
	t1=t;
	while(t)	//显示数字
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
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：char TFT_Show3Num(unsigned char x,unsigned short  int y,unsigned char num)
※ 功    能：显示一个3位的十进制数（0~255）
※ 入口参数：x      行座标
※           y      列座标
※           num    数字
※ 出口参数：是否有溢出
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
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
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：void TFT_Show5Num(unsigned char x,unsigned short int y,unsigned short int num)
※ 功    能：显示一个5位的十进制数（0~65535）
※ 入口参数：x      行座标
※           y      列座标
※           num    数字
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
void TFT_Show5Num(unsigned char x,unsigned short int y,unsigned short int num)
{         
    TFT_ShowChar(x,   y,num/10000+'0');
    TFT_ShowChar(x+TypeX, y,(num/1000)%10+'0'); 
    TFT_ShowChar(x+TypeX*2, y,(num/100)%10+'0'); 
    TFT_ShowChar(x+TypeX*3, y,(num/10)%10+'0'); 
    TFT_ShowChar(x+TypeX*4,y,num%10+'0');
}
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：void TFT_ShowHEX(unsigned char x,unsigned short int y,unsigned char num)
※ 功    能：显示一个2位的十六进制数（0~255）
※ 入口参数：x      行座标
※           y      列座标
※           num    数字
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
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

/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：void TFT_ShowHEX16(unsigned char x,unsigned short int y,unsigned char num)
※ 功    能：显示一个4位的十六进制数（0~255）
※ 入口参数：x      行座标
※           y      列座标
※           num    数字
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
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


/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：void TFT_ShowString(unsigned char x,unsigned  short  int y,const unsigned char *p)
※ 功    能：显示一个字符串
※ 入口参数：x      行座标
※           y      列座标
※           num    数字
※           *p     字符串首地址,指向ROM空间的字符型指针
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
void TFT_ShowString(unsigned char x,unsigned  short  int y,const unsigned char *p)
{         
    while(*p!='\0')
    {       
        if(x+TypeX>240)//自动换行
              {x=0;
	           y+=TypeY;
               }
        if(y>TFT_MAX_Y){y=x=0;Clear_TFT();}
        TFT_ShowChar(x,y,*p);
        x+=TypeX;
        p++;
    }  
}
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称：void TFT_RAMString(unsigned char x,unsigned  short  int y,unsigned char *p)
※ 功    能：显示一串在RAM中的字符串
※ 入口参数：x      行座标
※           y      列座标
※           num    数字
※           *p     字符串首地址,指向RAM空间的字符型指针
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
void TFT_RAMString(unsigned char x,unsigned  short  int y,unsigned char *p)
{         
    while(*p!='\0')
    {       
        if(x+TypeX>240)//自动换行
              {x=0;
	           y+=TypeY;
               }
        if(y>TFT_MAX_Y){y=x=0;Clear_TFT();}
        TFT_ShowChar(x,y,*p);
        x+=TypeX;
        p++;
    }      
}
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称： TFT_ShowPicture()
※ 功    能： 显示一幅图片
※ 入口参数： x       图片的左上角行座标
※            y       图片的左上角列座标
※            w       图片的宽度 
※            h       图片的高度  
※            *image  图片数据的首字节地址
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
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
/*※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※ 名    称： TFT_ShowPicture2()
※ 功    能： 显示一幅图片，对于大端模式和小端模式的数据，调整起来会方便一点，但是执行速度会稍慢一点。
※ 入口参数： x       图片的左上角行座标
※            y       图片的左上角列座标
※            w       图片的宽度 
※            h       图片的高度  
※            *image  图片数据的首字节地址
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
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



















