//--------------------------------------------------------------------------------
// ����GT21L16S2Y���ֿ�оƬʹ�ã���Ȩ���Ϻ���ͨ����Ƽ��������ι�˾���С�
// ���ߣ��칤 ʱ�䣺2009-07-15 �汾��VER1.0
//--------------------------------------------------------------------------------

#define  GT20_CS1    GPIO_SetBits(  GPIOC, GPIO_Pin_13)  	   //�ֿ�оƬƬѡ��
#define  GT20_CS0    GPIO_ResetBits(GPIOC, GPIO_Pin_13)
 

#define  ZFAdd      36224 	  //ZFAdd��HZAdd����ʾ�ַ��ͺ��ֵ���ʼ��ַ��
#define  HZAdd      93452  	  //MSB��LSB����ʾ GB2312����ĸ��ֽں͵��ֽ�
#define  GB2312tab  87436 	  //GB2312tab����ʾ GB2312ת�����ʼ��ַ��


unsigned long  GB2312_Add   (unsigned char *pGB);
unsigned char  GT_Readbyte  (unsigned long addr);  
void           GT_Read_nByte(unsigned char *BUFF,unsigned long addr,unsigned int n);

//------------------------------------------------
// ����ʹ������
//-------------------------------------------------
void LCD_Show_CN_Char(unsigned short x0,unsigned short y0,unsigned char *pCN);
void LCD_SHOW_CN15x16_String(unsigned short x0,unsigned short y0,unsigned char *str);















