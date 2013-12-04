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


�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~   ������void Process_CMT(INT8U *pData)							                    		  				  �~
�~	 ���ܣ�������+CMT�����������																	  			 	  �~
�~	  GU900�ĸ�ʽ��\r\n+CMT: "+8615990472896", , "2012/03/22 13:32:48+32"\r\nGPS?\r\n  (ע�⣺���ں���û�и�����) �~
�~	 SIM900�ĸ�ʽ��\r\n+CMT: "+8615990472896","","12/03/22,13:55:14+32"\r\nGPS?\r\n    (ע�⣺���ں����и�����)	  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
void Process_CMT(INT8U *pData)
{
#if defined GSM_Module_is_SIM900	 //���GU900ģ��Ľ��� 	
	char pBuff[20];
	char *Pstr;
#endif
	INT8U pField[MaxField];
	INT16U n,i=0;

	//�ּ췢���˺���<0>
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
#if defined GSM_Module_is_GU900	 //���GU900ģ��Ľ���
	//�ּ����ں�ʱ���Լ���������
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
#elif defined(GSM_Module_is_SIM900)//���SIM900ģ��Ľ���
 
	 if(GSM_GetField(pData, pField, 2, MaxField))
	 {
	  Pstr=(char *)pField+1;//��һ���ַ��ǡ��������Թ�
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


	  //��ȡ��������
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
	 	  //�������Ƿ��������������Ų�������ԡ�#����ͷ�����磺#pgsm&888888$
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
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~   ������void Process_IPD(INT8U *pData)							                    		  �~
�~	 ���ܣ�������+IPD�����������												 				      �~		  
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
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
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~   ������void Process_GSMLOC(INT8U *pData)							                    		                �~
�~	 ���ܣ�������+GSMLOC�����������												 				                                �~		  
�~   ���磺+CIPGSMLOC: 0,120.705479,31.328155,2012/12/02,13:36:03                                   �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
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
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~   ������void Process_CMGR(INT8U *pData)							                    		  �~
�~	 ���ܣ�������+CMGR�����������																  �~		  
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
void Process_CMGR(INT8U *pData)
{

}

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~   ������void Process_CMGR(INT8U *pData)							                    		  �~
�~	 ���ܣ�������+CMGR�����������																  �~		  
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
void Process_CSCA(INT8U *pData)
{
	INT8U pField[MaxField];
	INT8U n,i=0;

	//�ּ�������ĺ���<0>
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







