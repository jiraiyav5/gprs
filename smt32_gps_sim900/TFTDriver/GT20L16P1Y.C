//--------------------------------------------------------------------------------
// GT20L16S1Y汉字库芯片	驱动程序
//--------------------------------------------------------------------------------
#include "includes.h"



unsigned long  GB2312_Add(unsigned char *pGB)
{
 unsigned char  MSB,LSB,buffer[2]; 
 unsigned long 	Address=0;

 MSB = pGB[0];
 LSB = pGB[1];

	if( MSB >= 0xA1 && MSB <= 0xA5    )//字符区 
	{ 
	  if(MSB == 0xA1 && LSB >= 0xA1 && LSB <= 0xBF ) 
	     { 
		  Address = 32*(LSB-0xA1)+ ZFAdd; 
		 }
	  else if(MSB == 0xA3 && LSB >= 0xA1 && LSB <= 0xFE )
	     {
	  	  Address = 32*(31+LSB-0xA1)+ ZFAdd; 
	     } 
	  else if(MSB == 0xA4 && LSB >= 0xA1 && LSB <= 0xF3)
	     {
		  Address = 32*(31+94+LSB-0xA1)+ ZFAdd;
	     }  
	  else if(MSB == 0xA5 && LSB >= 0xA1 && LSB <= 0xF6)
	     {
		  Address = 32*(31+94+83+LSB-0xA1)+ ZFAdd;
		 } 
	  else
	     {
		  Address = ZFAdd; 
		 } 
	} 
	else if( (MSB >= 0xB0 && MSB <= 0xD7)&&(LSB>=0xA1 && LSB <= 0xFE) )//汉字 5270 
	{
	  Address = 32*( (MSB - 0xB0) * 94 + (LSB - 0xA1)+1)+ HZAdd; 
	} 
	else if( (MSB >= 0xD8 && MSB <= 0xF7)&&(LSB>=0xA1 && LSB <= 0xFE))//汉字 5270~6763 
	{ 
	  Address = ((MSB - 0xD8) * 94 + (LSB - 0xA1) ) *2 + GB2312tab; 
	  GT_Read_nByte(buffer, Address,2 ); //从字库中读取二个字节 
	  Address = 32* (256*buffer[0]+buffer[1]) + HZAdd; 
	}
 return Address;
}

//从芯片addr地址连续读取n个BYTE的数据到指定的数组
void GT_Read_nByte(unsigned char *BUFF,unsigned long addr,unsigned int n)
{
  unsigned int i;
  union {unsigned char DB8[4]; unsigned long DB32;} ADD;

	 ADD.DB32=addr;
	 GT20_CS0;
	 SPI_WriteByte(0X03);
	 SPI_WriteByte(ADD.DB8[2]);
	 SPI_WriteByte(ADD.DB8[1]);
	 SPI_WriteByte(ADD.DB8[0]);
    for(i=0;i<n;i++)
	 {
	  BUFF[i] = SPI_WriteByte(0XFF);
	 }
    GT20_CS1;
}

//从芯片中addr地址读一个BYTE的数据
unsigned char GT_Readbyte(unsigned long addr)  
	{
	 unsigned char DATA=0;
	 union {unsigned char DB8[4]; unsigned long DB32;} ADD;
	 ADD.DB32=addr;
	 GT20_CS0;
	 SPI_WriteByte(0X03);
	 SPI_WriteByte(ADD.DB8[2]);
	 SPI_WriteByte(ADD.DB8[1]);
	 SPI_WriteByte(ADD.DB8[0]);
	 DATA=SPI_WriteByte(0XFF);
	 GT20_CS1;
	
	 return DATA;
	}

//------------------------------------------------
// 程序使用例子
//-------------------------------------------------
void LCD_Show_CN_Char(unsigned short x0,unsigned short y0,unsigned char *pCN)
{
   unsigned char  temp,t;
   unsigned char  DotData[32];//存储汉字的点阵数据。
   unsigned char  count;

	GT_Read_nByte(DotData,GB2312_Add(pCN),32);

    //显示上半区
    TFT_SetWindow(x0,y0,x0+15,y0+7);
	TFT_SetCursor(x0,y0);
	temp=0x01;
	for(t=0;t<8;t++)
	{ 	
		for(count=0;count<16;count++)
		{      
		   if(DotData[count]&temp) TFT_WriteRAM(FONT_color);//字体颜色
		   else  TFT_WriteRAM(Back_color);//背景色       
		}
		temp<<=1;
	 }
    //显示下半区 	
	TFT_SetWindow(x0,y0+8,x0+15,y0+15);
	TFT_SetCursor(x0,y0+8);
	temp=0x01;
	for(t=0;t<8;t++)
	{ 	
		for(count=16;count<32;count++)
		{      
		   if(DotData[count]&temp) TFT_WriteRAM(FONT_color);//字体颜色
		   else  TFT_WriteRAM(Back_color);//背景色       
		}
		temp<<=1;
	 }
}


void LCD_SHOW_CN15x16_String(unsigned short x0,unsigned short y0,unsigned char *str)
{
   unsigned short X,Y;
   
   if(x0+16>TFT_MAX_X-1||y0+16>TFT_MAX_Y-1) return;//判断是否超出TFT边界  
   X=x0; 
   Y=y0;
   while( *str!='\0')
   {
   	 if(*str>=0xA1)
			 {
		     LCD_Show_CN_Char(X,Y,str);
   	     X+=16;
	       if(X>TFT_MAX_X-1) 
	         {
	           X=0; 
		         Y+=16; 
		         if(Y>TFT_MAX_Y-1) { Y=0; }
	         }
   	    str+=2;
				}
     else
		    {
					TFT_ShowChar(X,Y,*str);
					X+=8;
	        if(X>TFT_MAX_X-1) 
	         {
	          X=0; 
		        Y+=16; 
		        if(Y>TFT_MAX_Y-1) { Y=0; }
	         }
   	       str+=1;
        }


   }
}















