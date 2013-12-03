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
#if GSM_MODULE == DEF_ENABLED

#include "includes.h"
#include "GSM.h"
#include "GSM_TypeDefine.h"

#define MaxField	280

GSM_CMT     CMT;
GSM_CSCA    CSCA;
GSM_IPD     IPD;
GSM_CLIP    CLIP;
GSM_GSMLOC  GSMLOC;

/*


▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇   函数：void Process_CMT(INT8U *pData)							                    		  				  ▇
▇	 功能：处理“+CMT”后面的内容																	  			 	  ▇
▇	  GU900的格式：\r\n+CMT: "+8615990472896", , "2012/03/22 13:32:48+32"\r\nGPS?\r\n  (注意：日期后面没有个逗号) ▇
▇	 SIM900的格式：\r\n+CMT: "+8615990472896","","12/03/22,13:55:14+32"\r\nGPS?\r\n    (注意：日期后面有个逗号)	  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void Process_CMT(INT8U *pData)
{
#if defined GSM_Module_is_SIM900	 //针对GU900模块的解码 	
	char pBuff[20];
	char *Pstr;
#endif
	INT8U pField[MaxField];
	INT16U n,i=0;

	//分检发送人号码<0>
	if(GSM_GetField(pData, pField, 0, MaxField))
	{ i=0;
	  while(pField[i]!='"') {i+=1;}
	  i+=1;
	  if(pField[i]=='+'){i+=1;}
	  for(n=0;n<20;n++)
	   {
	    CMT.PhoneNum[n]=pField[i];
		if(pField[i]=='"'||pField[i]=='\0') {CMT.PhoneNum[n]='\0';  break;}
		i+=1;
	   }
	}
#if defined GSM_Module_is_GU900	 //针对GU900模块的解码
	//分检日期和时间以及短信内容
	if(GSM_GetField(pData, pField,2, MaxField))
	 {
	  i=0;
	  while((pField[i]!='"')&&(i<MaxField)) {i+=1;}
	  i+=1;
	  n=0;
	  while((pField[i]!='"')&&(i<MaxField)) 
	  {
	   CMT.Date_Time[n++]=pField[i++];
	  }
	  CMT.Date_Time[n]='\0';
	  i+=2;
	  n=0;
	  while(pField[i]!='\0')
	   {
	    CMT.Content[n++]=pField[i++];
	   }
	  CMT.Content[n]='\0';
	 }
#elif defined(GSM_Module_is_SIM900)//针对SIM900模块的解码
 
	 if(GSM_GetField(pData, pField, 2, MaxField))
	 {
	  Pstr=(char *)pField+1;//第一个字符是‘“’，略过
	  strcpy(CMT.Date_Time,Pstr);
	 }
  
	if(GSM_GetField(pData, pField, 3, MaxField))
	 {
	  n=0;i=0;
	  while(pField[i]!='"'&&(i<MaxField)) 
	  {
	   pBuff[n++]=pField[i++];
	  }
	  pBuff[n]='\0';
	  strcat(CMT.Date_Time,pBuff);


	  //截取短信内容
	  n=0;i=8;
	  while(pField[i]!='\n'&&pField[i]!='\0') {i+=1;}
	  if(pField[i]=='\n'){i+=1;}
	  while(pField[i]!='\0')
	   {
	    CMT.Content[n++]=pField[i++];
	   }
	  CMT.Content[n]='\0';
	 }
#endif
	 	  //检查短信是否包含操作命令，短信操作命令都以‘#’开头，例如：#pgsm&888888$
	  if(CMT.Content[0]=='#')
	   {i=1;
	    n=0;
	   	while(CMT.Content[i]!='&'&&CMT.Content[i]!='\0'&&n<GSM_OP_CMD_LEN)
		{
		 CMT.OP_CDM[n++]= CMT.Content[i++];
		}
		CMT.OP_CDM[n]='\0';
		n=0;
		i+=1;
		while(CMT.Content[i]!='$'&&CMT.Content[i]!='\0'&&n<GSM_OP_Key_LEN)
		{
		 CMT.OP_Key_ASCII[n++]= CMT.Content[i++];
		}
		CMT.OP_Key_ASCII[n]='\0';
		CMT.OP_Key = atol(CMT.OP_Key_ASCII);
	   }
	 
}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇   函数：void Process_IPD(INT8U *pData)							                    		  ▇
▇	 功能：处理“+IPD”后面的内容												 				      ▇		  
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void Process_IPD(INT8U *pData)
{
 	char pField[MaxField];
	INT16U i=0;
	while(*pData!=':'&&i<MaxField) pField[i++]=*pData++;
	IPD.DataLen=atoi(pField);
	pData++;
	i=0;
	while(*pData!='\0'&&i<GSM_MAX_DATA_LEN) IPD.Data[i++]=*pData++;
	IPD.Data[i]='\0';
}	
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇   函数：void Process_GSMLOC(INT8U *pData)							                    		                ▇
▇	 功能：处理“+GSMLOC”后面的内容												 				                                ▇		  
▇   例如：+CIPGSMLOC: 0,120.705479,31.328155,2012/12/02,13:36:03                                   ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void Process_GSMLOC(INT8U *pData)
{
  INT16U i=0;
  while(*pData!=','&&i<5) {pData++; i++;}
  if(*pData==',')
	  {
      pData++;
			i=0;
			while(*pData!=','&& i<15) {GSMLOC.Longitude[i++]=*pData++;}
			GSMLOC.Longitude[i]='\0';
    
    if(*pData==',')
		  {
       pData++;
			 i=0;
			 while(*pData!=','&& *pData!='\0'&& i<15) {GSMLOC.Latitude[i++]=*pData++;}
			 GSMLOC.Latitude[i]='\0';        
      }
 
    if(*pData==',')
		  {
       pData++;
			 i=0;
			 while(*pData!=','&& *pData!='\0'&& i<15) {GSMLOC.DATE[i++]=*pData++;}
			 GSMLOC.DATE[i]='\0';        
      }
			
    if(*pData==',')
		  {
       pData++;
			 i=0;
			 while(*pData!=','&& *pData!='\0'&& i<15) {GSMLOC.TIME[i++]=*pData++;}
			 GSMLOC.TIME[i]='\0';        
      }			
    }
}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇   函数：void Process_CMGR(INT8U *pData)							                    		  ▇
▇	 功能：处理“+CMGR”后面的内容																  ▇		  
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void Process_CMGR(INT8U *pData)
{

}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇   函数：void Process_CMGR(INT8U *pData)							                    		  ▇
▇	 功能：处理“+CMGR”后面的内容																  ▇		  
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void Process_CSCA(INT8U *pData)
{
	INT8U pField[MaxField];
	INT8U n,i=0;

	//分检短信中心号码<0>
	if(GSM_GetField(pData, pField, 0, MaxField))
	{ i=0;
	  while(pField[i]!='"') {i+=1;}
	  i+=1;
	  if(pField[i]=='+'){i+=1;}
	  for(n=0;n<20;n++)
	   {
	    CSCA.Phone[n]=pField[i];
		if(pField[i]=='"'||pField[i]=='\0') {CSCA.Phone[n]='\0';  break;}
		i+=1;
	   }
	   if(i>5) CSCA.isOK=1;
	}
}






















 #endif //GSM_MODULE == DEF_ENABLED








