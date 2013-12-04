//--------------------------------------------------------------------------------
// 仅限GT21L16S2Y汉字库芯片使用，版权归上海集通数码科技有限责任公司所有。
// 作者：朱工 时间：2009-07-15 版本：VER1.0
//--------------------------------------------------------------------------------

#define  GT20_CS1    GPIO_SetBits(  GPIOC, GPIO_Pin_13)  	   //字库芯片片选脚
#define  GT20_CS0    GPIO_ResetBits(GPIOC, GPIO_Pin_13)
 

#define  ZFAdd      36224 	  //ZFAdd、HZAdd：表示字符和汉字的起始地址。
#define  HZAdd      93452  	  //MSB、LSB：表示 GB2312编码的高字节和低字节
#define  GB2312tab  87436 	  //GB2312tab：表示 GB2312转码表起始地址。


unsigned long  GB2312_Add   (unsigned char *pGB);
unsigned char  GT_Readbyte  (unsigned long addr);  
void           GT_Read_nByte(unsigned char *BUFF,unsigned long addr,unsigned int n);

//------------------------------------------------
// 程序使用例子
//-------------------------------------------------
void LCD_Show_CN_Char(unsigned short x0,unsigned short y0,unsigned char *pCN);
void LCD_SHOW_CN15x16_String(unsigned short x0,unsigned short y0,unsigned char *str);















