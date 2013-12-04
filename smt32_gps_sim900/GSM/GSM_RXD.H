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

                        
void    GSM_RxISRHandler(void); 

void    GSM_COM_ISR(INT8U rxData);
BOOLEAN GSM_GetField(INT8U *pData, INT8U *pField, INT32S nFieldNum, INT32S nMaxFieldLen);
BOOLEAN GSM_Process_switch(INT8U *pCommand, INT8U *pData);
void GSM_Send_GB_SMS(INT8U *phone,unsigned  char *str);
void GSM_Send_MyStatus(INT8U *phone);
void GSM_Configuration(void);
void GSM_CALL(INT8U *phone);
void GSM_Answer_CALL(void);
void GSM_Hang_UP(void);

