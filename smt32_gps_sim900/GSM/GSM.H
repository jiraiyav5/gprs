/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						  -��ͨ����ԭ������-          		          						  �~
�~ ���ߣ�Ī����																					  �~
�~   QQ: 1695627698 																			  �~
�~ ������һ��ͨ���ӿƼ�һ 																		  �~
�~ ���䣺mojianlin2011@126.com																	  �~
�~ �Ա�ר���꣺etmcu.taobao.com																	  �~
�~ ����:�˳�����Ϊ��ͨ���߼����������д�ģ�����������ҵĿ�ģ���Ҫ�����븽���ҵ���ϵ��ʽ��		  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/


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

























