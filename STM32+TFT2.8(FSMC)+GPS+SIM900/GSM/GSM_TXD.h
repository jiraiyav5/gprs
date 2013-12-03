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


void  GSM_TxHandler (void);

char   GSM_Wait_OK(void);
void   GSM_Delays(unsigned int s);
void   GSM_Send_TXT_CMG(INT8U *phone,unsigned  char *str);
void   GSM_Answer_CALL(void);
void   GSM_InvertNumbers(unsigned char* pDst,unsigned char* pSrc);
void   StrCat(INT8U *strTo,INT8U *strFrom);
void   StrCpy(INT8U *strTo,INT8U *strFrom);

void GSM_LBS_INIT(void);
void GSM_LBS_Refresh(void);



void   GSM_Send_CN_MSG(unsigned char  *Target_phone,unsigned char  *Content);
void   GSM_Send_CN_MSG2(unsigned char  *Target_phone,unsigned char  *Content);

void GSM_GPRS_TCP_Connect(unsigned char *IP,unsigned char *PORT);
void GSM_GPRS_UDP_Connect(unsigned char *IP,unsigned char *PORT);
void GSM_GPRS_SEND(unsigned char *str);
void GSM_GPRS_TCP_CLOSE(unsigned char *IP,unsigned char *PORT);

#if defined(GU900)
 void GSM_Send_GB_SMS(INT8U *phone,unsigned  char *str);
#endif









