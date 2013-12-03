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


#include "GSM_Config.h"


#include "GSM_USER_API.h"
#include "GSM_RXD.h"
#include "GSM_TXD.h"
#include "GSM_Processes.h"
#include "GSM_GB2312_Unicode.h"



void    GSM_Init(void);              			

void    GSM_RxHandler(INT8U data);            	 
void    GSM_TxHandler(void);                  	

void    GSM_InitTarget(void);     			 	 

void    GSM_RxTxISRHandler(void);                

void    GSM_RxIntEn(void);                       
void    GSM_RxIntDis(void);                      

void    GSM_TxIntEn(void);                      
void    GSM_TxIntDis(void);
                      
void    GSM_TxChar(INT8U c);                      
void    GSM_TxString( INT8U *str);


























