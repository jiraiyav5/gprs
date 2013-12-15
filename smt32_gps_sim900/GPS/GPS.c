
#define  GPS_GLOBALS

#include <includes.h>
#if (TFT_MODULE	 == DEF_ENABLED)
#include "GPSimage.h" //TFT����ͼƬ
#endif
#if GPS_MODULE == DEF_ENABLED
#define MAXFIELD	25	  //�����ֶ���󳤶�, Ҳ����GPS��������е�������,��֮����ַ����ȣ�

static   OS_STK     GPS_TaskStk[GPS_TASK_STK_SIZE];
static   OS_EVENT   *GPS_Sem;

NP_STATE m_nState;					// �ȴ�NMEAͷ״̬���
INT8U    m_btChecksum;				// ����У���
INT8U    m_btReceivedChecksum;		// ����У���
INT16U   m_wIndex;					// Index used for command and data
INT8U    m_pCommand[NP_MAX_CMD_LEN];// NMEA command
INT8U    m_pData[NP_MAX_DATA_LEN];	// NMEA data

INT8U    GPSinf[100];
INT8U    STATUS_str[200];
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						  �ֲ�����             		          								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/

GPRMC_DATA  GPRMCData;	//����GPRMC	��GPGGA���ݱ������ܷ���һ���壬�������ݻ��ң�

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						  ����ͷ             		          								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
extern INT8S Get_Temperature(void);
static  void    GPS_InitOS(void);
static  void    GPS_Task(void *p_arg);
void GPS_START(void);
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						  GPS��ʼ��          		          								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/

void  GPS_Init (void)
{
	m_nState = NP_STATE_SOM;
	m_dwCommandCount = 0;
	GPDataReset();

	GPS_START();

    GPS_InitOS();
    GPS_InitTarget();                           			
}
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						  GPSģ��RST���ƽ����� 		          								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
void GPS_START(void)
{
    GPIO_InitTypeDef  gpio_init;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    gpio_init.GPIO_Pin   = GPIO_Pin_3;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &gpio_init);

	GPIO_SetBits(GPIOD, GPIO_Pin_3);	//�����GPSģ���RST��һ���½��أ����ŻῪʼ������ݡ�
	OSTimeDlyHMSM(0, 0, 1, 0);
	GPIO_ResetBits(GPIOD, GPIO_Pin_3);
}
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						 ����GPS����       		          								      �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
static  void  GPS_InitOS (void)
{
    INT8U  err;

    GPS_Sem = OSSemCreate(0);
    OSEventNameSet(GPS_Sem, (INT8U *)"GPS Signal", &err);

    (void)OSTaskCreateExt(GPS_Task,
                          (void *)0,                                  	
                          &GPS_TaskStk[GPS_TASK_STK_SIZE - 1], 			
                          GPS_TASK_PRIO,                          		
                          GPS_TASK_PRIO,
                          &GPS_TaskStk[0],                         	
                          GPS_TASK_STK_SIZE,
                          (void *)0,                                  
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); 

#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(GPS_TASK_PRIO, (INT8U *)"uC/GPS Task", &err);
#endif
}
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						  GPS���� 		          								              �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
static  void  GPS_Task (void *p_arg)
{
    INT8U      err;
    INT8U      s[100];
	#if (TFT_MODULE	 == DEF_ENABLED)
	TFT_ShowPicture(0,0,240,320,GPSimage);//��ʾ����ͼƬ
	#endif
	s[0]=s[1];//��ֹ������warning
	while(1) 
	 {
		(void)OSSemPend(GPS_Sem, 0, &err);

		OSTimeDlyHMSM(0, 0, 1,0);

		#if (OS_VIEW_MODULE == DEF_ENABLED)
		sprintf((char *)s, "\n%2d��%2d��%2d�� γ��:%f ����:%f �߶�:%f\n", GPGGAData.Hour+8, GPGGAData.Minute, GPGGAData.Second, GPGGAData.Latitude, GPGGAData.Longitude, GPGGAData.Altitude);
		//OSView_TxStr(s, 0);
		#endif

		sprintf((char *)GPSinf, "http://ditu.google.cn/maps?q=%f,%f&t=h&z=17", GPGGAData.Latitude, GPGGAData.Longitude);
        sprintf((char *)STATUS_str,"%2d��%2d��%2d��\nγ��:%f\n����:%f\n�߶�:%f\n�¶�:%2d\r\n",GPGGAData.Hour+8, GPGGAData.Minute, GPGGAData.Second, GPGGAData.Latitude, GPGGAData.Longitude, GPGGAData.Altitude,Get_Temperature());
		
		#if (TFT_MODULE	 == DEF_ENABLED)
		sprintf((char *)s, "%4d-%2d-%2d %2d:%2d:%2d",GPRMCData.Year,GPRMCData.Month,GPRMCData.Day,GPGGAData.Hour+8, GPGGAData.Minute, GPGGAData.Second);
		TFT_RAMString(65,300,s);
 
 		sprintf((char *)s, "%f",   GPRMCData.Latitude); //γ��
		TFT_RAMString(45,30,s);

 	    sprintf((char *)s, "%f",   GPRMCData.Longitude);//����
		TFT_RAMString(45,45,s);

		sprintf((char *)s, "%.5f", GPGGAData.Altitude );//���θ߶�
		TFT_RAMString(45,60,s);

		sprintf((char *)s, "%.5f", GPRMCData.GroundSpeed);	//�ٶ�
		TFT_RAMString(170,30,s);

		sprintf((char *)s, "%.5f", GPRMCData.MagVar);	//����
		TFT_RAMString(170,45,s);

		sprintf((char *)s, "%2d(%2d in use)", GPGSVData.TotalNumSatsInView,GPGGAData.NumOfSatsInUse);// ��������
		TFT_RAMString(120 ,5,s);

		sprintf((char *)s, "%.5f", GPGGAData.HDOP);	//��ȷ��
		TFT_RAMString(170,60,s);
		#endif
 	 }
}

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						  GPS�жϽ�����ں��� 		          								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
void  GPS_RxHandler (INT8U rx_data)
{
	ProcessNMEA(rx_data);
}

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						  GPS�жϷ�����ں��� 		          								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
void  GPS_TxHandler (void)
{
}
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						  GPS���ݳ�ʼ��      		          								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
void GPDataReset(void)
{
	int i;

	// GPGGA ����
	GPGGAData.Hour = 0;						//Сʱ����Ҫ����ʱ��ʱ�䡿
	GPGGAData.Minute = 0;					//��
	GPGGAData.Second = 0;					//��
	GPGGAData.Latitude = 0.0;				//γ�� ��С��0��ʾ��γ������0��ʾ��γ 
	GPGGAData.Longitude = 0.0;				//���� ��С��0��ʾ����������0��ʾ����
	GPGGAData.GPSQuality = 0;				//<6> GPS״̬��0=δ��λ��1=�ǲ�ֶ�λ��2=��ֶ�λ��6=���ڹ���
	GPGGAData.NumOfSatsInUse = 0;			//<7> ����ʹ�ý���λ�õ�����������00~12��
	GPGGAData.HDOP = 0.0;					//<8> HDOPˮƽ�������ӣ�0.5~99.9��
	GPGGAData.Altitude = 0.0;				//<9> ���θ߶ȣ�-9999.9~99999.9��
	GPGGAData.Count = 0;					//GPGGA ���ݼ�����
	GPGGAData.OldVSpeedSeconds = 0;			//<11> ���ʱ�䣨�����һ�ν��յ�����źſ�ʼ��������������ǲ�ֶ�λ��Ϊ�գ�
	GPGGAData.OldVSpeedAlt = 0.0;			//
	GPGGAData.VertSpeed = 0.0;				//�ƶ��ٶ�

	// GPGSA
	GPGSAData.Mode = 'M';					//<1> ģʽ��M=�ֶ���A=�Զ� 2D/3D
	GPGSAData.FixMode = 1;					//<2> ��λ���ͣ�1=û�ж�λ��2=2D��λ��3=3D��λ
	for(i = 0; i < NP_MAX_CHAN; i++)
	{
		GPGSAData.SatsInSolution[i] = 0;	//<3>�������ڽ���λ�õ����Ǻţ�01~32��ǰ���0Ҳ�������䣩��
	}
	GPGSAData.PDOP = 0.0;					//<4> PDOPλ�þ������ӣ�0.5~99.9��
	GPGSAData.HDOP = 0.0;					//<5> HDOPˮƽ�������ӣ�0.5~99.9��
	GPGSAData.VDOP = 0.0;					//<6> VDOP��ֱ�������ӣ�0.5~99.9��
	GPGSAData.Count = 0;					//GPGSA ���ݼ�����

	// GPGSV
	GPGSVData.TotalNumOfMsg = 0;			//<1> GSV��������
	GPGSVData.TotalNumSatsInView = 0;		//<3> �ɼ����ǵ�������00~12��ǰ���0Ҳ�������䣩
	for(i = 0; i < NP_MAX_CHAN; i++)
	{
		GPGSVData.SatInfo[i].Azimuth = 0;   //<5> �������ǣ�00~90�ȣ�ǰ���0Ҳ�������䣩
		GPGSVData.SatInfo[i].Elevation = 0;	//<6> ���Ƿ�λ�ǣ�000~359�ȣ�ǰ���0Ҳ�������䣩
		GPGSVData.SatInfo[i].PRN = 0;		//<4> PRN�루α��������룩��01~32��ǰ���0Ҳ�������䣩
		GPGSVData.SatInfo[i].SignalQuality = 0;	//<7> ����ȣ�00~99dB��û�и��ٵ�����ʱΪ�գ�ǰ���0Ҳ�������䣩
		GPGSVData.SatInfo[i].UsedInSolution = FALSE; //�Ƿ������ڽ���
	}
	GPGSVData.Count = 0;                    //GPGSV ���ݼ�����

	// GPRMB
	GPRMBData.DataStatus = 'V';				
	GPRMBData.CrosstrackError = 0.0;		//
	GPRMBData.DirectionToSteer = '?';		//
	GPRMBData.OriginWaypoint[0] = '\0';		//
	GPRMBData.DestWaypoint[0] = '\0';		//
	GPRMBData.DestLatitude = 0.0;			//
	GPRMBData.DestLongitude = 0.0;			//
	GPRMBData.RangeToDest = 0.0;			// 
	GPRMBData.BearingToDest = 0.0;			//
	GPRMBData.DestClosingVelocity = 0.0;	//
	GPRMBData.ArrivalStatus = 'V';			//
	GPRMBData.Count = 0;					//

	//
	// GPRMC
	//
	GPRMCData.Hour = 0;						//Сʱ����Ҫ����ʱ��ʱ�䡿
	GPRMCData.Minute = 0;					//��
	GPRMCData.Second = 0;					//��
	GPRMCData.DataValid = 'V';				//<2> ��λ״̬��A=��Ч��λ��V=��Ч��λ
	GPRMCData.Latitude = 0.0;				//<3> γ��
	GPRMCData.Longitude = 0.0;				//<5> ����
	GPRMCData.GroundSpeed = 0.0;			//<7> �������ʣ���λ��ǧ��/Сʱ��
	GPRMCData.Course = 0.0;					//<8> ���溽��000.0~359.9�ȣ����汱Ϊ�ο���׼��
	GPRMCData.Day = 1;						//��
	GPRMCData.Month = 1;					//��
	GPRMCData.Year = 2000;					//��
	GPRMCData.MagVar = 0.0;					//<10> ��ƫ�ǡ�000.0~180.0�ȣ�(+)ƫ��/(-)ƫ����
	GPRMCData.Count = 0;					//GPRMC ���ݼ�����

	//
	// GPZDA
	//
	GPZDAData.Hour = 0;						//
	GPZDAData.Minute = 0;					//
	GPZDAData.Second = 0;					//
	GPZDAData.Day = 1;						// 1 - 31
	GPZDAData.Month = 1;					// 1 - 12
	GPZDAData.Year = 2000;					//
	GPZDAData.LocalZoneHour = 0;			// 0 ~��13
	GPZDAData.LocalZoneMinute = 0;			// 0 - 59
	GPZDAData.Count = 0;					//
}

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						  GPS����������������		          								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
void ProcessNMEA(INT8U btData)
{
	switch(m_nState)
	{
		///////////////////////////////////////////////////////////////////////
		//�ȴ�'$'�ĵ���
		case NP_STATE_SOM :
			if(btData == '$')
			{
				m_btChecksum = 0;			// checksum	����
				m_wIndex = 0;				// index ����
				m_nState = NP_STATE_CMD;	//ת������Э��ͷ
			}
		break;
		///////////////////////////////////////////////////////////////////////
		//����Э��ͷ
		case NP_STATE_CMD :
			if(btData != ',' && btData != '*')
			{
				m_pCommand[m_wIndex++] = btData;
				m_btChecksum ^= btData;

				if(m_wIndex >= NP_MAX_CMD_LEN)//Э��ͷ�������ȣ�
				{
					m_nState = NP_STATE_SOM;
				}
			}
			else
			{
				m_pCommand[m_wIndex] = '\0';	//��ӽ�����
				m_btChecksum ^= btData;
				m_wIndex = 0;
				m_nState = NP_STATE_DATA;		//ת����������
			}
		break;
		///////////////////////////////////////////////////////////////////////
		// ��������
		case NP_STATE_DATA :
			if(btData == '*') //���յ�У�����ֶ���ʼ����
			{
				m_pData[m_wIndex] = '\0'; //���ݽ��ս���
				m_nState = NP_STATE_CHECKSUM_1;//ת������У����1
			}
			else 
			{
				if(btData == '\r')//����ղ�����*��У�����ֶ���ʼ������ô
				{
					m_pData[m_wIndex] = '\0';
					ProcessCommand(m_pCommand, m_pData);
					m_nState = NP_STATE_SOM; //�ص���ʼ״̬
					return;
				}

   			    m_btChecksum ^= btData;
				m_pData[m_wIndex] = btData;
				if(++m_wIndex >= NP_MAX_DATA_LEN) //��������Ƿ񳬳��涨����
				{
					m_nState = NP_STATE_SOM;
				}
			}
		break;
		///////////////////////////////////////////////////////////////////////
		case NP_STATE_CHECKSUM_1 :
			if( (btData - '0') <= 9)
			{
				m_btReceivedChecksum = (btData - '0') << 4;
			}
			else
			{
				m_btReceivedChecksum = (btData - 'A' + 10) << 4;
			}

			m_nState = NP_STATE_CHECKSUM_2;

		break;
		///////////////////////////////////////////////////////////////////////
		case NP_STATE_CHECKSUM_2 :
			if( (btData - '0') <= 9)
			{
				m_btReceivedChecksum |= (btData - '0');
			}
			else
			{
				m_btReceivedChecksum |= (btData - 'A' + 10);
			}

			if(m_btChecksum == m_btReceivedChecksum)
			{
				ProcessCommand(m_pCommand, m_pData);
			}

			m_nState = NP_STATE_SOM;
		break;

		///////////////////////////////////////////////////////////////////////
		default : m_nState = NP_STATE_SOM;
	}
}

///////////////////////////////////////////////////////////////////////////////
//���ղ�ͬ��Э��ͷ�����䲻ͬ�Ĵ���ʽ
///////////////////////////////////////////////////////////////////////////////
BOOLEAN ProcessCommand(INT8U *pCommand, INT8U *pData)
{
	//
	// GPGGA���ݴ���
	//
	if( strcmp((char *)pCommand, "GPGGA") == NULL )
	{
		ProcessGPGGA(pData);
		OSSemPost(GPS_Sem);
	}

	//
	// GPGSA���ݴ���
	//
	else if( strcmp((char *)pCommand, "GPGSA") == NULL )
	{
		ProcessGPGSA(pData);
	}

	//
	// GPGSV���ݴ���
	//
	else if( strcmp((char *)pCommand, "GPGSV") == NULL )
	{
		ProcessGPGSV(pData);
	}

	//
	// GPRMB���ݴ���
	//
	else if( strcmp((char *)pCommand, "GPRMB") == NULL )
	{
		ProcessGPRMB(pData);
	}

	//
	// GPRMC���ݴ���
	//
	else if( strcmp((char *)pCommand, "GPRMC") == NULL )
	{
		ProcessGPRMC(pData);
	}

	//
	// GPZDA���ݴ���
	//
	else if( strcmp((char *)pCommand, "GPZDA") == NULL )
	{
		ProcessGPZDA(pData);
	}
	//����Ч��������ʽ����Լ���GPSģ�����е����ݴ���
	m_dwCommandCount++;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	��������Ƿ���Ч
///////////////////////////////////////////////////////////////////////////////
BOOLEAN IsSatUsedInSolution(INT16U wSatID)
{
	INT32S i;
	if(wSatID == 0) return FALSE;
	for(i = 0; i < 12; i++)
	{
		if(wSatID == GPGSAData.SatsInSolution[i])
		{
			return TRUE;
		}
	}

	return FALSE;
}
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~   ������GetField(INT8U *pData, INT8U *pField, INT32S nFieldNum, INT32S nMaxFieldLen) 		  �~
�~	 ���ܣ����ڽ�ȡָ���ֶε�����																  �~		  
�~																								  �~
�~	 *pData       ��ԭʼ����ָ�룻																  �~
�~	 *pField      ��Ŀ������ָ�룬Ҳ����Ҫ��ŵĵط���											  �~
�~	 nFieldNum    ���ֶα�ţ��μ���������														  �~
�~	 nMaxFieldLen �ֶε���󳤶ȣ�																  �~
�~																								  �~
�~  ���磺																						  �~
�~	$GPRMC,151007.807,A,3119.8386,N,12042.3768,E,2.66,177.69,110112, ,  ,A*6D					  �~
�~	$GPRMC,   0      ,1,    2    ,3,     4    ,5,  6 ,   7  ,   8  ,9,10,11						  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
BOOLEAN GetField(INT8U *pData, INT8U *pField, INT32S nFieldNum, INT32S nMaxFieldLen)
{
	INT32S i, i2, nField;
  
  	//��֤�����Ƿ���Ч
	if(pData == NULL || pField == NULL || nMaxFieldLen <= 0)
	{
		return FALSE;
	}

 	//ת��Ŀ���ֶ�
	i = 0;
	nField = 0;
	while(nField != nFieldNum && pData[i])
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

	if(pData[i] == ',' || pData[i] == '*')
	{
		pField[0] = '\0';
		return FALSE;
	}

 	//��������
	i2 = 0;
	while(pData[i] != ',' && pData[i] != '*' && pData[i])
	{
		pField[i2] = pData[i];
		i2++; i++;

		//��������Ƿ����
		if(i2 >= nMaxFieldLen)
		{
			i2 = nMaxFieldLen-1;
			break;
		}
	}
	pField[i2] = '\0';

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ProcessGPZDA(INT8U *pData)
{
	char 	pBuff[10];
	INT8U 	pField[MAXFIELD];

	if(GetField(pData, pField, 0, MAXFIELD))
	{
		pBuff[0] = pField[0];
		pBuff[1] = pField[1];
		pBuff[2] = '\0';
		GPZDAData.Hour = atoi(pBuff);

		pBuff[0] = pField[2];
		pBuff[1] = pField[3];
		pBuff[2] = '\0';
		GPZDAData.Minute = atoi(pBuff);

		pBuff[0] = pField[4];
		pBuff[1] = pField[5];
		pBuff[2] = '\0';
		GPZDAData.Second = atoi(pBuff);
	}

	if(GetField(pData, pField, 1, MAXFIELD))
	{
		GPZDAData.Day = atoi((char *)pField);
	}
	else
	{
		GPZDAData.Day = 1;
	}

	if(GetField(pData, pField, 2, MAXFIELD))
	{
		GPZDAData.Month = atoi((char *)pField);
	}
	else
	{
		GPZDAData.Month = 1;
	}

	if(GetField(pData, pField, 3, MAXFIELD))
	{
		GPZDAData.Year = atoi((char *)pField);
	}
	else
	{
		GPZDAData.Year = 1;
	}

	if(GetField(pData, pField, 4, MAXFIELD))
	{
		GPZDAData.LocalZoneHour = atoi((char *)pField);
	}
	else
	{
		GPZDAData.LocalZoneHour = 0;
	}

	if(GetField(pData, pField, 5, MAXFIELD))
	{
		GPZDAData.LocalZoneMinute = atoi((char *)pField);
	}
	else
	{
		GPZDAData.LocalZoneMinute = 0;
	}

	GPZDAData.Count++;
}

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ��������  ProcessGPRMC(INT8U *pData)														  �~
�~	  ����  :   ����GPRMC����    	 													 	      �~
�~    																							  �~
�~  ����ʵ����																					  �~
�~	//$GPRMC,145048.941,V,         , ,          , ,    ,      ,110112, ,  ,N*4F					  �~
�~	//$GPRMC,151007.807,A,3119.8386,N,12042.3768,E,2.66,177.69,110112, ,  ,A*6D					  �~
�~	//$GPRMC,   0      ,1,    2    ,3,     4    ,5,  6 ,   7  ,   8  ,9,10,11					  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
void ProcessGPRMC(INT8U *pData)
{
	char pBuff[10];
	INT8U pField[MAXFIELD];

	//�ּ�ʱ��<0>
	if(GetField(pData, pField, 0, MAXFIELD))
	{
	    //Сʱ
		pBuff[0] = pField[0];
		pBuff[1] = pField[1];
		pBuff[2] = '\0';
		GPRMCData.Hour = atoi(pBuff);

		//��
		pBuff[0] = pField[2];
		pBuff[1] = pField[3];
		pBuff[2] = '\0';
		GPRMCData.Minute = atoi(pBuff);

		//��
		pBuff[0] = pField[4];
		pBuff[1] = pField[5];
		pBuff[2] = '\0';
		GPRMCData.Second = atoi(pBuff);
	}

	//�����Ƿ���Ч�ı�־<1>
	if(GetField(pData, pField, 1, MAXFIELD))
	{
		GPRMCData.DataValid = pField[0];
	}
	else
	{
		GPRMCData.DataValid = 'V';
	}

	//γ��<2>
	if(GetField(pData, pField, 2, MAXFIELD))
	{
		GPRMCData.Latitude = atof((char *)pField+2) / 60.0;
		pField[2] = '\0';
		GPRMCData.Latitude += atof((char *)pField);

	}
	//��γ/��γ��־λ����
	if(GetField(pData, pField, 3, MAXFIELD))
	{
		if(pField[0] == 'S')
		{
			GPRMCData.Latitude = -GPRMCData.Latitude;
		}
	}

	//����<4>
	if(GetField(pData, pField, 4, MAXFIELD))
	{
		GPRMCData.Longitude = atof((char *)pField+3) / 60.0;
		pField[3] = '\0';
		GPRMCData.Longitude += atof((char *)pField);
	}
	//����/������־λ����
	if(GetField(pData, pField, 5, MAXFIELD))
	{
		if(pField[0] == 'W')
		{
			GPRMCData.Longitude = -GPRMCData.Longitude;
		}
	}

	//ˮƽ�ƶ��ٶ�<6>
	if(GetField(pData, pField, 6, MAXFIELD))
	{
		GPRMCData.GroundSpeed = atof((char *)pField);
	}
	else
	{
		GPRMCData.GroundSpeed = 0.0;
	}

	//
	if(GetField(pData, pField, 7, MAXFIELD))
	{
		GPRMCData.Course = atof((char *)pField);
	}
	else
	{
		GPRMCData.Course = 0.0;
	}

	//
	//CTU��������
	if(GetField(pData, pField, 8, MAXFIELD))
	{	
		//��
		pBuff[0] = pField[0];
		pBuff[1] = pField[1];
		pBuff[2] = '\0';
		GPRMCData.Day = atoi(pBuff);
		
		//��
		pBuff[0] = pField[2];
		pBuff[1] = pField[3];
		pBuff[2] = '\0';
		GPRMCData.Month = atoi(pBuff);

		//��
		pBuff[0] = pField[4];
		pBuff[1] = pField[5];
		pBuff[2] = '\0';
		GPRMCData.Year = atoi(pBuff);
		GPRMCData.Year += 2000;				
	}


	//<9> ��ƫ�ǡ�000.0~180.0�ȣ�(+)ƫ��/(-)ƫ����
	if(GetField(pData, pField, 9, MAXFIELD))
	{
		GPRMCData.MagVar = atof((char *)pField);
	}
	else
	{
		GPRMCData.MagVar = 0.0;
	}
	//<11> ��ƫ�Ƿ���E��������W������
	if(GetField(pData, pField, 10, MAXFIELD))
	{
		if(pField[0] == 'W')
		{
			GPRMCData.MagVar = -GPRMCData.MagVar;
		}
	}

	GPRMCData.Count++;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ProcessGPRMB(INT8U *pData)
{
	INT8U pField[MAXFIELD];

	if(GetField(pData, pField, 0, MAXFIELD))
	{
		GPRMBData.DataStatus = pField[0];
	}
	else
	{
		GPRMBData.DataStatus = 'V';
	}

	if(GetField(pData, pField, 1, MAXFIELD))
	{
		GPRMBData.CrosstrackError = atof((char *)pField);
	}
	else
	{
		GPRMBData.CrosstrackError = 0.0;
	}

	if(GetField(pData, pField, 2, MAXFIELD))
	{
		GPRMBData.DirectionToSteer = pField[0];
	}
	else
	{
		GPRMBData.DirectionToSteer = '?';
	}

	if(GetField(pData, pField, 3, MAXFIELD))
	{
		strcpy(GPRMBData.OriginWaypoint, (char *)pField);
	}
	else
	{
		GPRMBData.OriginWaypoint[0] = '\0';
	}

	if(GetField(pData, pField, 4, MAXFIELD))
	{
		strcpy(GPRMBData.DestWaypoint, (char *)pField);
	}
	else
	{
		GPRMBData.DestWaypoint[0] = '\0';
	}

	if(GetField(pData, pField, 5, MAXFIELD))
	{
		GPRMBData.DestLatitude = atof((char *)pField+2) / 60.0;
		pField[2] = '\0';
		GPRMBData.DestLatitude += atof((char *)pField);

	}
	if(GetField(pData, pField, 6, MAXFIELD))
	{
		if(pField[0] == 'S')
		{
			GPRMBData.DestLatitude = -GPRMBData.DestLatitude;
		}
	}

	if(GetField(pData, pField, 7, MAXFIELD))
	{
		GPRMBData.DestLongitude = atof((char *)pField+3) / 60.0;
		pField[3] = '\0';
		GPRMBData.DestLongitude += atof((char *)pField);
	}
	if(GetField(pData, pField, 8, MAXFIELD))
	{
		if(pField[0] == 'W')
		{
			GPRMBData.DestLongitude = -GPRMBData.DestLongitude;
		}
	}

	if(GetField(pData, pField, 9, MAXFIELD))
	{
		GPRMBData.RangeToDest = atof((char *)pField);
	}
	else
	{
		GPRMBData.CrosstrackError = 0.0;
	}

	if(GetField(pData, pField, 10, MAXFIELD))
	{
		GPRMBData.BearingToDest = atof((char *)pField);
	}
	else
	{
		GPRMBData.BearingToDest = 0.0;
	}

	if(GetField(pData, pField, 11, MAXFIELD))
	{
		GPRMBData.DestClosingVelocity = atof((char *)pField);
	}
	else
	{
		GPRMBData.DestClosingVelocity = 0.0;
	}

	if(GetField(pData, pField, 12, MAXFIELD))
	{
		GPRMBData.ArrivalStatus = pField[0];
	}
	else
	{
		GPRMBData.DestClosingVelocity = 'V';
	}

	GPRMBData.Count++;
}

 ///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ProcessGPGSV(INT8U *pData)
{
	INT32S i, nTotalNumOfMsg = 0, nMsgNum = 0;
	INT8U pField[MAXFIELD];

	if(GetField(pData, pField, 0, MAXFIELD))
	{
		nTotalNumOfMsg = atoi((char *)pField);

		if(nTotalNumOfMsg > 9 || nTotalNumOfMsg < 0) return; 
	}
	if(nTotalNumOfMsg < 1 || nTotalNumOfMsg*4 >= NP_MAX_CHAN)
	{
		return;
	}

	if(GetField(pData, pField, 1, MAXFIELD))
	{
		nMsgNum = atoi((char *)pField);

		if(nMsgNum > 9 || nMsgNum < 0) return; 
	}

	if(GetField(pData, pField, 2, MAXFIELD))
	{
		GPGSVData.TotalNumSatsInView = atoi((char *)pField);
	}

	for(i = 0; i < 4; i++)
	{
		if(GetField(pData, pField, 3 + 4*i, MAXFIELD))
		{
			GPGSVData.SatInfo[i+(nMsgNum-1)*4].PRN = atoi((char *)pField);
		}
		else
		{
			GPGSVData.SatInfo[i+(nMsgNum-1)*4].PRN = 0;
		}

		if(GetField(pData, pField, 4 + 4*i, MAXFIELD))
		{
			GPGSVData.SatInfo[i+(nMsgNum-1)*4].Elevation = atoi((char *)pField);
		}
		else
		{
			GPGSVData.SatInfo[i+(nMsgNum-1)*4].Elevation = 0;
		}

		if(GetField(pData, pField, 5 + 4*i, MAXFIELD))
		{
			GPGSVData.SatInfo[i+(nMsgNum-1)*4].Azimuth = atoi((char *)pField);
		}
		else
		{
			GPGSVData.SatInfo[i+(nMsgNum-1)*4].Azimuth = 0;
		}

		if(GetField(pData, pField, 6 + 4*i, MAXFIELD))
		{
			GPGSVData.SatInfo[i+(nMsgNum-1)*4].SignalQuality = atoi((char *)pField);
		}
		else
		{
			GPGSVData.SatInfo[i+(nMsgNum-1)*4].SignalQuality = 0;
		}

		GPGSVData.SatInfo[i+(nMsgNum-1)*4].UsedInSolution = IsSatUsedInSolution(GPGSVData.SatInfo[i+(nMsgNum-1)*4].PRN);
	}

	GPGSVData.Count++;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ProcessGPGSA(INT8U *pData)
{
	INT8U pField[MAXFIELD];
	char pBuff[10];
	INT32S i;

	if(GetField(pData, pField, 0, MAXFIELD))
	{
		GPGSAData.Mode = pField[0];
	}

	if(GetField(pData, pField, 1, MAXFIELD))
	{
		GPGSAData.FixMode = pField[0] - '0';
	}

	for(i = 0; i < 12; i++)
	{
		if(GetField(pData, pField, 2 + i, MAXFIELD))
		{
			pBuff[0] = pField[0];
			pBuff[1] = pField[1];
			pBuff[2] = '\0';
			GPGSAData.SatsInSolution[i] = atoi(pBuff);
		}
		else
		{
			GPGSAData.SatsInSolution[i] = 0;
		}
	}

	if(GetField(pData, pField, 14, MAXFIELD))
	{
		GPGSAData.PDOP = atof((char *)pField);
	}
	else
	{
		GPGSAData.PDOP = 0.0;
	}

	if(GetField(pData, pField, 15, MAXFIELD))
	{
		GPGSAData.HDOP = atof((char *)pField);
	}
	else
	{
		GPGSAData.HDOP = 0.0;
	}

	if(GetField(pData, pField, 16, MAXFIELD))
	{
		GPGSAData.VDOP = atof((char *)pField);
	}
	else
	{
		GPGSAData.VDOP = 0.0;
	}

	GPGSAData.Count++;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void ProcessGPGGA(INT8U *pData)
{
	INT8U pField[MAXFIELD];
	char pBuff[10];
	INT32S nSeconds;

	if(GetField(pData, pField, 0, MAXFIELD))
	{

		pBuff[0] = pField[0];
		pBuff[1] = pField[1];
		pBuff[2] = '\0';
		GPGGAData.Hour = atoi(pBuff);

		pBuff[0] = pField[2];
		pBuff[1] = pField[3];
		pBuff[2] = '\0';
		GPGGAData.Minute = atoi(pBuff);

		pBuff[0] = pField[4];
		pBuff[1] = pField[5];
		pBuff[2] = '\0';
		GPGGAData.Second = atoi(pBuff);
	}

	if(GetField(pData, pField, 1, MAXFIELD))
	{
		GPGGAData.Latitude = atof((char *)pField+2) / 60.0;
		pField[2] = '\0';
		GPGGAData.Latitude += atof((char *)pField);

	}
	if(GetField(pData, pField, 2, MAXFIELD))
	{
		if(pField[0] == 'S')
		{
			GPGGAData.Latitude = -GPGGAData.Latitude;
		}
	}

	if(GetField(pData, pField, 3, MAXFIELD))
	{
		GPGGAData.Longitude = atof((char *)pField+3) / 60.0;
		pField[3] = '\0';
		GPGGAData.Longitude += atof((char *)pField);
	}
	if(GetField(pData, pField, 4, MAXFIELD))
	{
		if(pField[0] == 'W')
		{
			GPGGAData.Longitude = -GPGGAData.Longitude;
		}
	}

	if(GetField(pData, pField, 5, MAXFIELD))
	{
		GPGGAData.GPSQuality = pField[0] - '0';
	}

	if(GetField(pData, pField, 6, MAXFIELD))
	{
		pBuff[0] = pField[0];
		pBuff[1] = pField[1];
		pBuff[2] = '\0';
		GPGGAData.NumOfSatsInUse = atoi(pBuff);
	}

	if(GetField(pData, pField, 7, MAXFIELD))
	{
		GPGGAData.HDOP = atof((char *)pField);
	}

	if(GetField(pData, pField, 8, MAXFIELD))
	{
		GPGGAData.Altitude = atof((char *)pField);
	}

	nSeconds = (INT32S)GPGGAData.Minute * 60 + (INT32S)GPGGAData.Second;
	if(nSeconds > GPGGAData.OldVSpeedSeconds)
	{
		double dDiff = (double)(GPGGAData.OldVSpeedSeconds-nSeconds);
		double dVal = dDiff/60.0;
		if(dVal != 0.0)
		{
			GPGGAData.VertSpeed = (GPGGAData.OldVSpeedAlt - GPGGAData.Altitude) / dVal;
		}
	}
	GPGGAData.OldVSpeedAlt = GPGGAData.Altitude;
	GPGGAData.OldVSpeedSeconds = nSeconds;

	GPGGAData.Count++;
}
#endif
