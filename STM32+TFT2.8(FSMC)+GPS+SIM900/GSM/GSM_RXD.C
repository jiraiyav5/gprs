/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  -易通电子原创代码-          		          						                        ▇
▇ 作者：莫建林																					                                          ▇
▇   QQ: 1695627698 																			                                        ▇
▇ 旺旺：一易通电子科技一 																		                                    ▇
▇ 邮箱：mojianlin2011@126.com																	                                  ▇
▇ 淘宝专卖店：etmcu.taobao.com																	                                  ▇
▇ 声明:此程序是为易通无线技术开发板而写的，请勿用于商业目的，若要引用请附上我的联系方式！		    ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

#include "includes.h"
#include "GSM.h"
#include "GSM_TypeDefine.h"

#if GSM_MODULE == DEF_ENABLED


GSM_RX_STATE  GSM_RX_nState;	//串口数据处理状态

INT16U    Rx_Index;	//数据指针
INT8U	  GSM_pCommand[GSM_MAX_CMD_LEN];	//
INT8U     GSM_pData[GSM_MAX_DATA_LEN];	//

INT8U	  RX_RN_JP_NUM=0;

INT16U    Test_counter=0;
INT8U     Flag_CMT=0;
INT8U     Flag_CMGR=0;
INT8U     Flag_DATA=0;
INT8U     Flag_CIPGSMLOC=0;

extern GSM_CMT   CMT;
extern GSM_CSCA  CSCA;
extern GSM_IPD   IPD;
extern GSM_CLIP  CLIP;
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  GSM中断接收入口函数 		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GSM_RxHandler (INT8U rx_data)
{
	GSM_COM_ISR(rx_data);
	Test_counter++;
}	

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  GSM串口中断数据流处理函数		          							  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void GSM_COM_ISR(INT8U rxData)
{
	switch(GSM_RX_nState)
	{
		///////////////////////////////////////////////////////////////////////
		case RX_STATE_IDLE :  // 等待‘\r\n’
			if((rxData == '\n')||(rxData == '+'))	  
			{
				Rx_Index = 0;  // 数据指针清零
				GSM_RX_nState = RX_STATE_CMD; //转到接受协议头的状态
			}
		  break;
		///////////////////////////////////////////////////////////////////////
		//接受协议头信息，比如：收到短息数据 ：+CMT:
		case RX_STATE_CMD :
			if(rxData != ','&&rxData != ':' && rxData != '\r')
			{
				GSM_pCommand[Rx_Index++] = rxData;  //接受协议头内容

				//检查协议头长度是否超出预定长度
				if(Rx_Index >= GSM_MAX_CMD_LEN)
				{
				 GSM_RX_nState = RX_STATE_IDLE;

				}
			}
			else  //AT+CMGF=1\r\n\r\nOK\r\n
			{			
				if(Rx_Index==0){ GSM_RX_nState = RX_STATE_IDLE;}
				else 
				   {
				   	GSM_pCommand[Rx_Index] = '\0';	// 字符串结束符
				    GSM_RX_nState = RX_STATE_DATA;		//从下一个接收的字节开始，转到RX_STATE_DATA状态处理
				    Rx_Index = 0;
					if( strcmp((char *)GSM_pCommand, "+CMT") == NULL ) //接收到新的短信数据
	                     { Flag_CMT=1;//在此标记一下，以免后面切换时重复比较
						   RX_RN_JP_NUM=1;
	                     }
				    else if( strcmp((char *)GSM_pCommand, "+CMGR") == NULL ) //接收到新的短信数据
					   {
						  Flag_CMGR=1;
						  RX_RN_JP_NUM=3;
						 }
						else if( strcmp((char *)GSM_pCommand, "+CIPGSMLOC") == NULL ) //接收到新的短信数据
					     {
						    Flag_CIPGSMLOC=1;
						    RX_RN_JP_NUM=0;
						   } 
				    else   RX_RN_JP_NUM=0;
				   }
			} 
		break;	  
		///////////////////////////////////////////////////////////////////////
		//接收数据状态
		case RX_STATE_DATA :
			if(rxData == '\r') 
			{				   
			   if(RX_RN_JP_NUM==0)
			   {									
				GSM_pData[Rx_Index] = '\0';
				GSM_Process_switch(GSM_pCommand, GSM_pData);
				GSM_RX_nState = RX_STATE_IDLE;
				return;
			   }
			 if(RX_RN_JP_NUM>0) RX_RN_JP_NUM-=1; 
			  //RX_RN_JP_NUM是需要跳过的回车换行符（“\r\n”）的个数；例如新短信语句内包含一个回车换行符
			  //\r\n+CMT: "+8615990472896",,"10/07/29,19:24:56+32"\r\n短信内容\r\n	
			}
			else 
			{
				GSM_pData[Rx_Index] = rxData;
				if(++Rx_Index >= GSM_MAX_DATA_LEN) 
				{
					GSM_RX_nState = RX_STATE_IDLE;
				}
			}
		break;
		///////////////////////////////////////////////////////////////////////
		default : GSM_RX_nState = RX_STATE_IDLE;
	}
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇   函数：BOOLEAN GSM_Process_switch(INT8U *pCommand, INT8U *pData)      ▇
▇	 功能：根据不同的命令头，切换到相应的处理程序												  ▇		  
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
BOOLEAN GSM_Process_switch(INT8U *pCommand, INT8U *pData)
{

	if( Flag_CMGR) //接收到新的短信数据
	 {
	  Flag_CMGR=0;
	  Process_CMGR(pData);
	 }
	if(Flag_CMT)
	 {
	  Flag_CMT=0;
	  Process_CMT(pData);
	  CMT.isNew=1; 
	  }
	if( strcmp((char *)pCommand, "+CSCA") == NULL ) //处理短信中心号码信息
	  {
	   Process_CSCA(pData);
	   CSCA.isNew=1;
	  }
	if( strcmp((char *)pCommand, "+IPD") == NULL ) //处理TCP数据
	  {
	   Process_IPD(pData);
	   IPD.isNew=1;
	  }

	if( strcmp((char *)pCommand, "+CLIP") == NULL ) //处理来电
	  {
	   //Process_CLIP(pData);
	   CLIP.RING=1;
	   CLIP.COUNT+=1;
	  }
	if(Flag_CIPGSMLOC) //处理基站定位数据
	  {
	   Process_GSMLOC(pData);
     GSMLOC.isNew=1;
		 Flag_CIPGSMLOC=0;
	  }

	Flag_DATA=1;

	return TRUE;
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇   函数：GetField(INT8U *pData, INT8U *pField, INT32S nFieldNum, INT32S nMaxFieldLen) 		  ▇
▇	 功能：用于截取指定字段的内容																  ▇		  
▇																								  ▇
▇	 *pData       是原始数据指针；																  ▇
▇	 *pField      是目标数组指针，也就是要存放的地方；											  ▇
▇	 nFieldNum    是字段编号（参见下例）；														  ▇
▇	 nMaxFieldLen 字段的最大长度；																  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
BOOLEAN GSM_GetField(INT8U *pData, INT8U *pField, INT32S nFieldNum, INT32S nMaxFieldLen)
{
	INT32S i, i2, nField;
  
  	//验证参数是否有效
	if(pData == NULL || pField == NULL || nMaxFieldLen <= 0)
	{
		return FALSE;
	}

 	//转到目标字段
	i = 0;
	nField = 0;
	while(nField != nFieldNum && pData[i]!='\0')
	{
		if(pData[i] == ',')
		{
			nField++;
		}

		i++;

		if(pData[i] == NULL)
		{
			pField[0] = '\0';
			return FALSE;
		}
	}

	if(pData[i] == ',' )
	{
		pField[0] = '\0';
		return FALSE;
	}

 	//复制数据
	i2 = 0;
	while(pData[i] != ',' && pData[i]!='\0')
	{
		pField[i2] = pData[i];
		i2++; i++;

		//检查数据是否过长
		if(i2 >= nMaxFieldLen)
		{
			i2 = nMaxFieldLen-1;
			break;
		}
	}
	pField[i2] = '\0';

	return TRUE;
}


#endif //GSM_MODULE == DEF_ENABLED

















