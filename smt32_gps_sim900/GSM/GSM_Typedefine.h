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
	RX_STATE_IDLE =	0,	       	        // �ȴ���\r\n���ַ���״̬
	RX_STATE_CMD,						// ����Э��ͷ��״̬�����磺�յ���\r\n���ַ���ȴ����ա�GPRMC��
	RX_STATE_DATA,						// ��ȡЭ�����ݵ�״̬
} GSM_RX_STATE;

 typedef	struct GSM_CMT_Info
{ 
  INT8U    isNew; //��Ƕ����Ƿ�Ϊ���յ��Ķ��ţ�
  INT8U    PhoneNum[20]; //���ͷ��ĺ��룻
  char 	   Date_Time[25];//�ַ��͵����ں�ʱ��
  INT8U	   Content[300]; //�����������70�����֣�һ������ռ4���ֽ�
  INT8U	   OP_CDM[10];   //���Ų�������
  char	   OP_Key_ASCII[8];//��������,�ַ���ʽ��
  INT32U   OP_Key;       //��������,�����ͣ�
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
 INT8U    Phone[20]; //�������ĺ��룻
}GSM_CSCA;

 typedef	struct GSM_GSMLOC_Info
{
 INT8U    isNew;
 INT8U    Latitude[15];  //γ��
 INT8U   	Longitude[15]; //����
 INT8U	  DATE[15];
 INT8U    TIME[15];
}GSM_GSMLOC;

extern  GSM_GSMLOC  GSMLOC;


 typedef	struct GSM_CLIP_Info 
{ 
 INT8U    RING;//1=��������
 INT8U	  COUNT;//�������������
 INT8U    Phone[20]; //������ʾ�ĺ��룻
}GSM_CLIP;






