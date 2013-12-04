#include  <stm32f10x_lib.h>


//#define  TFT_DRIVER_ILI9320	 //����TFT����IC�ͺ���ȷ������������������
#define  TFT_DRIVER_ILI9328

///////////////////////////////////////////////////////////
//            ����TFT����IC�Ļ�ͼ�Ĵ�����ַ
///////////////////////////////////////////////////////////
 #define  X_Sart_REG	 0x0050
 #define  Y_Sart_REG	 0x0052
 #define  X_END_REG	     0x0051
 #define  Y_END_REG	     0x0053
 #define  X_Pointer	     0x0020
 #define  Y_Pointer	     0x0021
 #define  Sart_CMD       0x0022

#define TFT_MAX_X   240      //����X���������  
#define TFT_MAX_Y   320      //����Y���������

static unsigned short int FontType=816;   //�ַ���С
static unsigned char TypeX=8;   //�����ַ���ˮƽ����ĵ���
static unsigned char TypeY=16;  //�����ַ��Ĵ�ֱ����ĵ��� 

static unsigned short int Back_color=0x0000;  //������ɫ
static unsigned short int FONT_color=0xf800;  //ǰ����ɫ

static unsigned short int TFT_Y;
static unsigned short int TFT_X;

 #define  TFT_RST1      GPIO_SetBits(GPIOB, GPIO_Pin_1) 
 #define  TFT_RST0	  GPIO_ResetBits(GPIOB, GPIO_Pin_1)


 #define Bank1_TFT_D    ((u32)0x60020000)    
 #define Bank1_TFT_C    ((u32)0x60000000)	 

 #define TFT_WriteRAM(RGB_Code)  	 *(volatile u16 *)(Bank1_TFT_D)= RGB_Code

 #define TFT_SHOW_START()            *(volatile u16 *)(Bank1_TFT_C)= Sart_CMD

 #define TFT_Send_REG(REG,REGValue)  *(volatile u16 *)(Bank1_TFT_C)= REG;\
									 *(volatile u16 *)(Bank1_TFT_D)= REGValue

 #define TFT_SetPoint(x,y,point)    TFT_SetCursor(x,y);  \
                                    TFT_SHOW_START();	 \
                                    TFT_WriteRAM(point)

 #define TFT_SetWindow(x0,y0,x1,y1)	TFT_Send_REG(X_Sart_REG,x0);\
									TFT_Send_REG(X_END_REG ,x1);\
									TFT_Send_REG(Y_Sart_REG,y0);\
									TFT_Send_REG(Y_END_REG ,y1)

 #define TFT_SetCursor(x,y)	        TFT_Send_REG(X_Pointer,x);\
									TFT_Send_REG(Y_Pointer,y);\
                                    TFT_SHOW_START()


void DelayLCD(unsigned long t);
void TFT_CtrlLinesConfig(void);
void TFT_FSMCConfig(void);
void TFT_Init(void);
void Clear_TFT(void);
//void TFT_SetCursor(u16 x,u16 y);
void TFT_SET_Window(u16 x0,u16 y0,u16 x1,u16 y1);
u16  TFT_GetPoint (u16 x,u16 y);
void Set_Font_Type(unsigned short int type);//����ASCII�ַ���С
void TFT_ShowChar(    unsigned char x,unsigned short int y,unsigned char num);//��ʾһ��ASCII�ַ�
void TFT_ShowNum(     unsigned char x,unsigned short int y,long  num);
char TFT_Show3Num(    unsigned char x,unsigned short int y,unsigned char num);//��ʾһ��3λ��ʮ��������0~255��
void TFT_Show5Num(    unsigned char x,unsigned short int y,unsigned short int num);//��ʾһ��5λ��ʮ��������0~65535��
void TFT_ShowHEX(     unsigned char x,unsigned short int y,unsigned char num);//��ʾһ��2λ��ʮ����������0~255��
void TFT_ShowHEX16(unsigned char x,unsigned short int y,unsigned short int num);
void TFT_ShowString(  unsigned char x,unsigned short int y,const unsigned char *p);//��ʾһ���ַ���
void TFT_RAMString(   unsigned char x,unsigned short int y,unsigned char *p);//��ʾһ����RAM�е��ַ���
void TFT_ShowPicture( unsigned char x,unsigned short int y,unsigned char w,unsigned short  int h,const unsigned char  *image);
void TFT_ShowPicture2(unsigned char x,unsigned short int y,unsigned char w,unsigned short  int h,const unsigned char  *image);   

 u16 TP_ReadX(void);
 u16 TP_ReadY(void);







