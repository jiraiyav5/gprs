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

#ifndef	INT8U
typedef unsigned char  INT8U;                    
#endif
#ifndef	INT8S
typedef signed   char  INT8S;                   
#endif
#ifndef INT16U
typedef unsigned short INT16U;                  
#endif
#ifndef	INT16S
typedef signed   short INT16S;                  
#endif
#ifndef INT32U
typedef unsigned int   INT32U;                   
#endif
#ifndef	INT32S
typedef signed   int   INT32S;                   
#endif
#ifndef	FP32
typedef float          FP32;                     
#endif
#ifndef	FP64
typedef double         FP64;                     
#endif 





 typedef enum _GSM_RX_STATE
{
	RX_STATE_IDLE =	0,	       	        // 等待‘\r\n’字符的状态
	RX_STATE_CMD,						// 接收协议头的状态，例如：收到‘\r\n’字符后等待接收“GPRMC”
	RX_STATE_DATA,						// 获取协议数据的状态
} GSM_RX_STATE;

 typedef	struct GSM_CMT_Info
{ 
  INT8U    isNew; //标记短信是否为新收到的短信；
  INT8U    PhoneNum[20]; //发送方的号码；
  char 	   Date_Time[25];//字符型的日期和时间
  INT8U	   Content[300]; //短信内容最多70个汉字，一个汉字占4个字节
  INT8U	   OP_CDM[10];   //短信操作命令
  char	   OP_Key_ASCII[8];//操作密码,字符形式；
  INT32U   OP_Key;       //操作密码,长整型；
}GSM_CMT;


 typedef	struct GSM_IPD_Info
{
 INT8U    isNew;
 INT16U	  DataLen;
 INT8U	  Data[300];
}GSM_IPD;


 typedef	struct GSM_CSCA_Info 
{ 
 INT8U    isNew;
 INT8U    isOK;
 INT8U    Phone[20]; //短信中心号码；
}GSM_CSCA;

 typedef	struct GSM_GSMLOC_Info
{
 INT8U    isNew;
 INT8U    Latitude[15];  //纬度
 INT8U   	Longitude[15]; //经度
 INT8U	  DATE[15];
 INT8U    TIME[15];
}GSM_GSMLOC;

extern  GSM_GSMLOC  GSMLOC;


 typedef	struct GSM_CLIP_Info 
{ 
 INT8U    RING;//1=正在响铃
 INT8U	  COUNT;//响铃次数计数器
 INT8U    Phone[20]; //来电显示的号码；
}GSM_CLIP;






