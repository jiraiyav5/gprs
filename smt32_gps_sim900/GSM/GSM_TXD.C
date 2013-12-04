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
/*
 这个程序模块中主要包含了串口发送部分的动能，比如：发送英文短信，发送中文短信。。。

*/
#include "includes.h"
#include "GSM.h"
#include "GSM_TypeDefine.h"

extern INT8U GSM_pCommand[GSM_MAX_CMD_LEN];


void GSM_Delays(unsigned int s)
 {
   unsigned int k;
    while(s--)
      {
	   k=3000;
       while(k--);
      }
  }
/******************************************************************* 
函数功能：等待串口接收到“OK”，最多等待5秒
*********************************************************************/
char GSM_Wait_OK(void)
{
 unsigned char t=50;
  
  while(strcmp((char *)GSM_pCommand, "OK") != NULL && t--) OSTimeDlyHMSM(0, 0, 0,100);

  if(t==0) return 0;

  return 1;
}

/******************************************************************* 
函数功能：正常顺序的字符串转换为两两颠倒的字符串，若长度为奇数，补'F'凑成偶数
如："8613851872468" --> "683158812764F8"
输入参数：pSrc: 源字符串指针
		  pDst: 目标字符串指针
返回值: 目标字符串长度
*********************************************************************/
void GSM_InvertNumbers(unsigned char* pDst,unsigned char* pSrc)
{
    INT16U nSrcLength=0;   // 字符串长度									   
    INT16U i=0;
	unsigned char strFrom[30];
	 
	StrCpy(strFrom,pSrc);
	    
    nSrcLength = strlen((char *)strFrom);//得到原始字符串的长度
    if(nSrcLength & 1)//源串长度是奇数吗？
		 {//如果是奇数，那就在源串末尾加个‘F’，让它的长度变为偶数。
		  StrCat(strFrom,"F");
	 	  i=0;
		  while(strFrom[i]!='\0')
		    {
             pDst[i]   = strFrom[i+1];   
             pDst[i+1] = strFrom[i];       		  
		     i+=2;
			}
		  pDst[i] = '\0';
		 }
	else //如果不是奇数，就直接转换
		 {
		  i=0;
		  while(strFrom[i]!='\0')
		    {
             pDst[i]   = strFrom[i+1];   
             pDst[i+1] = strFrom[i];       		  
		     i+=2;
			}
		  pDst[i] = '\0';
		 }
}
/*************************************************/
//字符串合并
/*************************************************/
void  StrCat(INT8U *strTo,INT8U *strFrom)
 {
   INT16U n=0,i=0;
 
   while(strTo[i] != '\0') i++;
   while(strFrom[n] != '\0')  strTo[i++] = strFrom[n++];
   strTo[i]='\0';
 }
/*************************************************/
//字符串拷贝
/*************************************************/
void StrCpy(INT8U *strTo,INT8U *strFrom)
{
 INT16U i=0;

 while(strFrom[i] != '\0') strTo[i] = strFrom[i++];
 strTo[i] = '\0';
}
 
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  GSM中断发送入口函数 		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GSM_TxHandler (void)
{
}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇ 函数名：void GSM_Configuration(void)								       						  ▇
▇ 函数功能：GSM模块工作模式配置																  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void GSM_Configuration(void)
{
  GSM_TxString("AT\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+IPR=115200\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CMGF=1\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
	#ifdef GSM_Module_is_SIM900
  GSM_TxString("AT+CIPHEAD=1\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
	#endif
  GSM_TxString("AT+CSCS=\"GSM\"\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CPMS=\"SM\"\r\n"); //短信存到SIM卡
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CLIP=1\r\n"); //来电显示号码(如果开通了来电显示的话)
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CNMI=2,2,0,0,1\r\n");//有新短信就直接发送给单片机
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CSCA?\r\n");//获取短信中心号码
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CMGD=1,4\r\n"); //删除SIM卡上所有短信,如果SIM卡存满了短信，将无法接收新的短信！
  OSTimeDlyHMSM(0, 0, 1,0);
  #ifdef GSM_SUPPORT_LBS
	GSM_LBS_INIT();
  #endif
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇ 函数名：void GSM_GPRS_Connect(unsigned char *IP,unsigned char *PORT)   						  ▇
▇ 函数功能：连接GPRS网络																		  ▇
▇	   参数：IP	 : 目标主机的IP地址或者域名														  ▇
▇			 PORT: 目标主机的端口号																  ▇
▇ 应用实例：GSM_GPRS_TCP_Connect("etmcu.xicp.net","5800");				                		  ▇
▇           GSM_GPRS_TCP_Connect("114.254.135.18","5800");				                		  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
#if defined(GSM_Module_is_GU900) 
void GSM_GPRS_TCP_Connect(unsigned char *IP,unsigned char *PORT) 
{
  GSM_TxString("AT+CIPMUX=0\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CIPMODE=0\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CIPCFG=0,50,0\r\n");//设置模块发送的心跳包数据格式为“0x01,0x02,0xA0”，空闲等待50秒后发送 
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CIPPACK=0,\"0102A0\"\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CIPSTART=\"TCP\",\"");
  GSM_TxString(IP);
  GSM_TxString("\",");
  GSM_TxString(PORT);
  GSM_TxString(",2\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
}
void GSM_GPRS_UDP_Connect(unsigned char *IP,unsigned char *PORT) 
{
  GSM_TxString("AT+CIPMUX=0\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CIPMODE=0\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CIPCFG=0,50,0\r\n");//设置模块发送的心跳包数据格式为“0x01,0x02,0xA0”，空闲等待50秒后发送 
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CIPPACK=0,\"0102A0\"\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CIPSTART=\"UDP\",\"");
  GSM_TxString(IP);
  GSM_TxString("\",");
  GSM_TxString(PORT);
  GSM_TxString(",2\r\n");
  OSTimeDlyHMSM(0, 0, 1,0);
}
#elif defined(GSM_Module_is_SIM900)
void GSM_GPRS_TCP_Connect(unsigned char *IP,unsigned char *PORT) 
{
  GSM_TxString("AT+CGATT=1\r\n");//附着网络
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CSTT\r\n");   // 设置APN
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CIICR\r\n");// 激活移动场景
  OSTimeDlyHMSM(0, 0, 2,0);
  GSM_TxString("AT+CIFSR\r\n");// 获得本地模块内部IP地址
  OSTimeDlyHMSM(0, 0, 2,0);
  GSM_TxString("AT+CIPSTART=\"TCP\",\"");// 建立TCP/IP连接
  GSM_TxString(IP);
  GSM_TxString("\",");
  GSM_TxString(PORT);
  GSM_TxString("\r\n");
  OSTimeDlyHMSM(0, 0, 5,0);
}
void GSM_GPRS_UDP_Connect(unsigned char *IP,unsigned char *PORT) 
{
  GSM_TxString("AT+CGATT=1\r\n");//附着网络
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CSTT\r\n");   // 设置APN
  OSTimeDlyHMSM(0, 0, 1,0);
  GSM_TxString("AT+CIICR\r\n");// 激活移动场景
  OSTimeDlyHMSM(0, 0, 2,0);
  GSM_TxString("AT+CIFSR\r\n");// 获得本地模块内部IP地址
  OSTimeDlyHMSM(0, 0, 2,0);
  GSM_TxString("AT+CIPSTART=\"UDP\",\"");// 建立TCP/IP连接
  GSM_TxString(IP);
  GSM_TxString("\",");
  GSM_TxString(PORT);
  GSM_TxString("\r\n");
  OSTimeDlyHMSM(0, 0, 5,0);
}


#endif

void GSM_GPRS_TCP_CLOSE(unsigned char *IP,unsigned char *PORT) 
 {
  GSM_TxString("AT+CIPCLOSE=1\r\n");//快速关闭连接
  OSTimeDlyHMSM(0, 0, 1,0);
 }
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇ 函数名：void GSM_GPRS_SEND(unsigned char *str)           					            	  ▇
▇ 函数功能：发送TCP\IP数据																		  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void GSM_GPRS_SEND(unsigned char *str)
 {
  INT16U LEN=0;
  INT8U	 buf[50];

  LEN=strlen((char *)str);
  sprintf((char *)buf,"AT+CIPSEND=%d\r\n",LEN);
  GSM_TxString(buf);
  OSTimeDlyHMSM(0, 0, 0,500);
  GSM_TxString(str);
  OSTimeDlyHMSM(0, 0, 0,500);
 }
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇ 函数名：void GSM_Send_TXT_CMG(INT8U *phone,unsigned  char *str)       						  ▇
▇ 函数功能：呼叫目标号码																		  ▇
▇   参数：  *phone  目标号码；																	  ▇
▇   注意：只支持手机号码，座机号码没有测试 													  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void GSM_CALL(INT8U *phone)
{ 
  if(*phone=='1')//例如：15990472896
        GSM_TxString("ATD");
  else	GSM_TxString("ATD+");//例如：8615990472896 	带国码的
  GSM_TxString(phone);
  GSM_TxString(";\r\n");//注意有个分号‘;’
  OSTimeDlyHMSM(0, 0, 0,500);
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇ 函数名：void GSM_Answer_CALL(void)								       						  ▇
▇ 函数功能：接听电话																			  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void GSM_Answer_CALL(void)
{ 
  GSM_TxString("ATA\r\n");
  OSTimeDlyHMSM(0, 0, 0,100);
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇ 函数名：void GSM_Hang_UP(void)									       						  ▇
▇ 函数功能：挂断电话																			  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void GSM_Hang_UP(void)
{ 
  GSM_TxString("ATH\r\n");
  OSTimeDlyHMSM(0, 0, 0,100);
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇ 函数名：void GSM_LBS_Refresh(void)									       						                          ▇
▇ 函数功能：基站定位更新																		                                      ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void GSM_LBS_INIT(void)
{
  GSM_TxString("AT+SAPBR=3,1,\"Contype\",\"GPRS\"\r\n");
  OSTimeDlyHMSM(0, 0, 0,100);
  GSM_TxString("AT+SAPBR=3,1,\"APN\",\"CMNET\"\r\n");
  OSTimeDlyHMSM(0, 0, 0,100);	
  GSM_TxString("AT+SAPBR=1,1\r\n");
  OSTimeDlyHMSM(0, 0, 0,100);
  GSM_TxString("AT+SAPBR=2,1\r\n");
  //OSTimeDlyHMSM(0, 0, 0,100);
	GSM_Wait_OK();
}
/////////////////////////////////////////////////////
void GSM_LBS_Refresh(void)
{
  GSM_TxString("AT+CIPGSMLOC=1,1\r\n");
  GSM_Wait_OK();
}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇ 函数名：void GSM_Send_TXT_CMG(INT8U *phone,unsigned  char *str)       						  ▇
▇ 函数功能：发送一条ASCII字符短信																  ▇
▇   参数：  *phone  接收短信的手机号码；														  ▇
▇		     *str	   TXT型短信内容；															  ▇
▇ 应用举例 ：GSM_Send_TXT_CMG("15990472896","I love ETMCU board！"); 							  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void GSM_Send_TXT_CMG(INT8U *phone,unsigned  char *str)
{ 
  GSM_TxString("\r\nAT+CMGF=1\r\n"); //设置以TXT格式发送接收短信
  OSTimeDlyHMSM(0, 0, 0,500);
  GSM_TxString("AT+CMGS=\"");
  GSM_TxString(phone);
  GSM_TxString("\"\r\n");
  OSTimeDlyHMSM(0, 0, 0,500);
  GSM_TxString(str);
  GSM_TxChar(0X1A);
  OSTimeDlyHMSM(0, 0, 0,100);
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇ 函数名：void GSM_Send_TXT_CMG(INT8U *phone,unsigned  char *str)       						  ▇
▇ 函数功能：发送一条ASCII字符短信																  ▇
▇   参数：  *phone  接收短信的手机号码；														  ▇
▇		     *str	   TXT型短信内容；															  ▇
▇ 应用举例 ：GSM_Send_GB_SMS("15990472896","I love ETMCU board！"); 							  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
#if defined(GSM_Module_is_GU900)
 void GSM_Send_GB_SMS(INT8U *phone,unsigned  char *str)
  {
   GSM_TxString("\r\nAT+CMGF=1\r\n"); //设置以TXT格式发送接收短信
   OSTimeDlyHMSM(0, 0, 0,500);
   GSM_TxString("\r\nAT+CSCS=\"GB2312\"\r\n"); //设置采用国标2312字符集
   OSTimeDlyHMSM(0, 0, 0,500);
   GSM_TxString("AT+CMGS=\"");
   GSM_TxString(phone);
   GSM_TxString("\"\r\n");
   OSTimeDlyHMSM(0, 0, 0,500);
   GSM_TxString(str);
   GSM_TxChar(0X1A);
   OSTimeDlyHMSM(0, 0, 0,100);
  }

void GSM_Send_MyStatus(INT8U *phone)
{  
   GSM_TxString("\r\nAT+CMGF=1\r\n"); //设置以TXT格式发送接收短信
   OSTimeDlyHMSM(0, 0, 0,500);
   GSM_TxString("\r\nAT+CSCS=\"GB2312\"\r\n"); //设置采用国标2312字符集
   OSTimeDlyHMSM(0, 0, 0,500);
   GSM_TxString("AT+CMGS=\"");
   GSM_TxString(phone);
   GSM_TxString("\"\r\n");
   OSTimeDlyHMSM(0, 0, 0,500);
   GSM_TxString(STATUS_str);
   GSM_TxChar(0X1A);
   OSTimeDlyHMSM(0, 0, 0,100);
}
#endif
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇ 函数名：void GSM_Send_CN_MSG(unsigned char  *Target_phone,unsigned char  *Content)			  ▇
▇ 函数功能：发送一条中文短信																	              ▇
▇   参数：  *Target_phone 目标手机号码															  ▇
▇		     *Content	   短信内容；															  ▇
▇ 调用示例：GSM_Send_CN_MSG("8615990472896","你好！Hellow World!");							  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
extern  GSM_CSCA  CSCA;
INT8U strHead[100];
INT8U strMSG[300];
void GSM_Send_CN_MSG(unsigned char  *Target_phone,unsigned char  *Content)
{ 
   INT8U SMS_Center[20];
   INT8U tgPhone[20];

   INT8U strLEN[5];
   INT8U pEND[2]={0x1A,'\0'};

   GSM_TxString("AT+CSCA?\r\n");//获取短信中心号码
   OSTimeDlyHMSM(0, 0, 1,0);
   GSM_TxString("AT+CMGF=0\r\n"); //设置以PDU格式发送接收短信
   OSTimeDlyHMSM(0, 0, 1,0);
   GSM_TxString("AT+CSCS=\"GSM\"\r\n"); //确保字符集为“GSM”字符集
   OSTimeDlyHMSM(0, 0, 1,0);
   
 if(CSCA.isOK==1)
  {//短信中心号码有效才发送
   GSM_InvertNumbers(SMS_Center,CSCA.Phone);
  }
 else
  {
   GSM_TxString("AT+CMGF=1\r\n"); //退出前改回TXT模式
   OSTimeDlyHMSM(0, 0, 1,0);
   return;//否则退出
  } 
   GSM_InvertNumbers(tgPhone,Target_phone);
   GB2UnicodeStr(strMSG,Content); //短信内容转成Unicode字符串，放到strMSG[]
   StrCat(strMSG,pEND);	//在末尾加个发送结束符0x1A，也就是所谓的<Ctrl+Z>

   //拼接各个部分
   StrCpy(strHead,"0891");
   StrCat(strHead,SMS_Center);
   StrCat(strHead,"1100");
   I2HEXa8(strLEN,strlen((char *)Target_phone));//转成十六进制形式的字符串
   StrCat(strHead,strLEN);
   StrCat(strHead,"91");
   StrCat(strHead,tgPhone);
   StrCat(strHead,"0008A7");
   I2HEXa8(strLEN,strlen((char *)strMSG)/2 ); //转成十六进制形式的字符串
   StrCat(strHead,strLEN);

   I2DECa8(strLEN ,(strlen((char *)strMSG)/2)+15 );//转成十进制形式的字符串

   GSM_TxString("AT+CMGS=");
   GSM_TxString(strLEN);
   GSM_TxString("\r\n");
   OSTimeDlyHMSM(0, 0, 0,800 );
   GSM_TxString(strHead);
   GSM_TxString(strMSG);

   IWDG_ReloadCounter();//喂狗
   GSM_Wait_OK(); //等待发送结束
   GSM_TxString("AT+CMGF=1\r\n"); //发送结束后，改回TXT模式，要不接收到的短信都是PDU格式的，以至于无法接收短信命令！
   OSTimeDlyHMSM(0, 0, 0,500);
}

INT8U  MSGstr[400];
INT8U  temp[300];   //函数内部不能放太大的数组，否则单片机会死机！
void GSM_Send_CN_MSG2(unsigned char  *Target_phone,unsigned char  *Content)
{ 
   INT8U tgPhone[20];
   INT8U strLEN[5];
   INT8U pEND[2]={0x1A,0};
   
  
   GSM_TxString((INT8U *)"AT+CMGF=0\r\n"); //设置以PDU格式发送接收短信
   OSTimeDlyHMSM(0, 0, 0,200 ); 
   
   //拼接各个部分
   strcpy((char *)MSGstr,(char *)"003100");
   I2HEXa8(strLEN,strlen((char *)Target_phone));//转成十六进制形式的字符串
   StrCat(MSGstr,strLEN);
   StrCat(MSGstr,"91");
   GSM_InvertNumbers(tgPhone,Target_phone);
   StrCat(MSGstr,tgPhone);
   StrCat(MSGstr,"000801");
   GB2UnicodeStr(temp,Content); //短信内容转成Unicode字符串，放到strMSG[]
   I2HEXa8(strLEN,strlen((char *)temp)/2); //转成十六进制形式的字符串
   StrCat(MSGstr,strLEN);
   StrCat(MSGstr,temp);
   StrCat(MSGstr,pEND);
   IWDG_ReloadCounter();//喂狗
 //  TFT_ShowString(0,100,MSGstr);

   I2DECa8(strLEN ,(strlen((char *)MSGstr)/2)-1);//转成十进制形式的字符串

   GSM_TxString("AT+CMGS=");
   GSM_TxString(strLEN);
   GSM_TxString("\r\n");
   OSTimeDlyHMSM(0, 0, 0,800 ); 
   GSM_TxString(MSGstr);
   OSTimeDlyHMSM(0,0,4,0); 
   GSM_Wait_OK(); //等待发送结束
   GSM_TxString("AT+CMGF=1\r\n"); //发送结束后，改回TXT模式，要不接收到的短信都是PDU格式的，以至于无法接收短信命令！
   OSTimeDlyHMSM(0, 0, 0,800 ); 
}
























