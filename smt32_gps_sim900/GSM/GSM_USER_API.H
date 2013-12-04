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

#define  MMA7660  0x98	 //重力传感器MMA7660的IIC器件地址

extern INT32U My_Key_Number;


void MMC7660_Init(void);
void Key_Number_Init(void);
void Key_Number_Store(void);
void Key_Number_Renew(void);
INT8S Get_Temperature(void);


unsigned char SPI_WriteByte(unsigned char data);
void   SpiDelay(unsigned int DelayCnt);
void   Gsenser_SHOW_REG(void);
void   ADXL345_Init(void);
void   Gsenser_SHOW_REG(void);
void   Gsenser_WriteREG(INT8U REG,INT8U DATA);
INT8U  Gsenser_ReadREG(INT8U REG);
INT16U Gsenser_ReadREG2(INT8U REG);
void   SHOW_SPI_REG(void);
void   Gsenser_SHOW_XYZ(void);
void   ADXL345_Test(void);





