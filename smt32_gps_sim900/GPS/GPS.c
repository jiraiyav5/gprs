
#define  GPS_GLOBALS

#include <includes.h>
#if (TFT_MODULE	 == DEF_ENABLED)
#include "GPSimage.h" //TFT背景图片
#endif
#if GPS_MODULE == DEF_ENABLED
#define MAXFIELD	25	  //数据字段最大长度, 也就是GPS输出数据中的两个‘,’之间的字符长度；

static   OS_STK     GPS_TaskStk[GPS_TASK_STK_SIZE];
static   OS_EVENT   *GPS_Sem;

NP_STATE m_nState;					// 等待NMEA头状态标记
INT8U    m_btChecksum;				// 计算校验和
INT8U    m_btReceivedChecksum;		// 计算校验和
INT16U   m_wIndex;					// Index used for command and data
INT8U    m_pCommand[NP_MAX_CMD_LEN];// NMEA command
INT8U    m_pData[NP_MAX_DATA_LEN];	// NMEA data

INT8U    GPSinf[100];
INT8U    STATUS_str[200];
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  局部变量             		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

GPRMC_DATA  GPRMCData;	//发现GPRMC	和GPGGA数据变量不能放在一起定义，否则数据会乱！

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  函数头             		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
extern INT8S Get_Temperature(void);
static  void    GPS_InitOS(void);
static  void    GPS_Task(void *p_arg);
void GPS_START(void);
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  GPS初始化          		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

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
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  GPS模块RST控制脚配置 		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void GPS_START(void)
{
    GPIO_InitTypeDef  gpio_init;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    gpio_init.GPIO_Pin   = GPIO_Pin_3;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &gpio_init);

	GPIO_SetBits(GPIOD, GPIO_Pin_3);	//必须给GPS模块的RST脚一个下降沿，它才会开始输出数据。
	OSTimeDlyHMSM(0, 0, 1, 0);
	GPIO_ResetBits(GPIOD, GPIO_Pin_3);
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						 创建GPS任务       		          								      ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
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
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  GPS任务 		          								              ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
static  void  GPS_Task (void *p_arg)
{
    INT8U      err;
    INT8U      s[100];
	#if (TFT_MODULE	 == DEF_ENABLED)
	TFT_ShowPicture(0,0,240,320,GPSimage);//显示背景图片
	#endif
	s[0]=s[1];//防止编译器warning
	while(1) 
	 {
		(void)OSSemPend(GPS_Sem, 0, &err);

		OSTimeDlyHMSM(0, 0, 1,0);

		#if (OS_VIEW_MODULE == DEF_ENABLED)
		sprintf((char *)s, "\n%2d点%2d分%2d秒 纬度:%f 经度:%f 高度:%f\n", GPGGAData.Hour+8, GPGGAData.Minute, GPGGAData.Second, GPGGAData.Latitude, GPGGAData.Longitude, GPGGAData.Altitude);
		//OSView_TxStr(s, 0);
		#endif

		sprintf((char *)GPSinf, "http://ditu.google.cn/maps?q=%f,%f&t=h&z=17", GPGGAData.Latitude, GPGGAData.Longitude);
        sprintf((char *)STATUS_str,"%2d点%2d分%2d秒\n纬度:%f\n经度:%f\n高度:%f\n温度:%2d\r\n",GPGGAData.Hour+8, GPGGAData.Minute, GPGGAData.Second, GPGGAData.Latitude, GPGGAData.Longitude, GPGGAData.Altitude,Get_Temperature());
		
		#if (TFT_MODULE	 == DEF_ENABLED)
		sprintf((char *)s, "%4d-%2d-%2d %2d:%2d:%2d",GPRMCData.Year,GPRMCData.Month,GPRMCData.Day,GPGGAData.Hour+8, GPGGAData.Minute, GPGGAData.Second);
		TFT_RAMString(65,300,s);
 
 		sprintf((char *)s, "%f",   GPRMCData.Latitude); //纬度
		TFT_RAMString(45,30,s);

 	    sprintf((char *)s, "%f",   GPRMCData.Longitude);//经度
		TFT_RAMString(45,45,s);

		sprintf((char *)s, "%.5f", GPGGAData.Altitude );//海拔高度
		TFT_RAMString(45,60,s);

		sprintf((char *)s, "%.5f", GPRMCData.GroundSpeed);	//速度
		TFT_RAMString(170,30,s);

		sprintf((char *)s, "%.5f", GPRMCData.MagVar);	//方向
		TFT_RAMString(170,45,s);

		sprintf((char *)s, "%2d(%2d in use)", GPGSVData.TotalNumSatsInView,GPGGAData.NumOfSatsInUse);// 卫星数量
		TFT_RAMString(120 ,5,s);

		sprintf((char *)s, "%.5f", GPGGAData.HDOP);	//精确度
		TFT_RAMString(170,60,s);
		#endif
 	 }
}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  GPS中断接收入口函数 		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GPS_RxHandler (INT8U rx_data)
{
	ProcessNMEA(rx_data);
}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  GPS中断发送入口函数 		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GPS_TxHandler (void)
{
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  GPS数据初始化      		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void GPDataReset(void)
{
	int i;

	// GPGGA 数据
	GPGGAData.Hour = 0;						//小时【需要加上时区时间】
	GPGGAData.Minute = 0;					//分
	GPGGAData.Second = 0;					//秒
	GPGGAData.Latitude = 0.0;				//纬度 ：小于0表示南纬，大于0表示北纬 
	GPGGAData.Longitude = 0.0;				//经度 ：小于0表示西经，大于0表示东经
	GPGGAData.GPSQuality = 0;				//<6> GPS状态：0=未定位，1=非差分定位，2=差分定位，6=正在估算
	GPGGAData.NumOfSatsInUse = 0;			//<7> 正在使用解算位置的卫星数量（00~12）
	GPGGAData.HDOP = 0.0;					//<8> HDOP水平精度因子（0.5~99.9）
	GPGGAData.Altitude = 0.0;				//<9> 海拔高度（-9999.9~99999.9）
	GPGGAData.Count = 0;					//GPGGA 数据计数器
	GPGGAData.OldVSpeedSeconds = 0;			//<11> 差分时间（从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空）
	GPGGAData.OldVSpeedAlt = 0.0;			//
	GPGGAData.VertSpeed = 0.0;				//移动速度

	// GPGSA
	GPGSAData.Mode = 'M';					//<1> 模式，M=手动，A=自动 2D/3D
	GPGSAData.FixMode = 1;					//<2> 定位类型，1=没有定位，2=2D定位，3=3D定位
	for(i = 0; i < NP_MAX_CHAN; i++)
	{
		GPGSAData.SatsInSolution[i] = 0;	//<3>正在用于解算位置的卫星号（01~32，前面的0也将被传输）。
	}
	GPGSAData.PDOP = 0.0;					//<4> PDOP位置精度因子（0.5~99.9）
	GPGSAData.HDOP = 0.0;					//<5> HDOP水平精度因子（0.5~99.9）
	GPGSAData.VDOP = 0.0;					//<6> VDOP垂直精度因子（0.5~99.9）
	GPGSAData.Count = 0;					//GPGSA 数据计数器

	// GPGSV
	GPGSVData.TotalNumOfMsg = 0;			//<1> GSV语句的总数
	GPGSVData.TotalNumSatsInView = 0;		//<3> 可见卫星的总数（00~12，前面的0也将被传输）
	for(i = 0; i < NP_MAX_CHAN; i++)
	{
		GPGSVData.SatInfo[i].Azimuth = 0;   //<5> 卫星仰角（00~90度，前面的0也将被传输）
		GPGSVData.SatInfo[i].Elevation = 0;	//<6> 卫星方位角（000~359度，前面的0也将被传输）
		GPGSVData.SatInfo[i].PRN = 0;		//<4> PRN码（伪随机噪声码）（01~32，前面的0也将被传输）
		GPGSVData.SatInfo[i].SignalQuality = 0;	//<7> 信噪比（00~99dB，没有跟踪到卫星时为空，前面的0也将被传输）
		GPGSVData.SatInfo[i].UsedInSolution = FALSE; //是否已用于解算
	}
	GPGSVData.Count = 0;                    //GPGSV 数据计数器

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
	GPRMCData.Hour = 0;						//小时【需要加上时区时间】
	GPRMCData.Minute = 0;					//分
	GPRMCData.Second = 0;					//秒
	GPRMCData.DataValid = 'V';				//<2> 定位状态，A=有效定位，V=无效定位
	GPRMCData.Latitude = 0.0;				//<3> 纬度
	GPRMCData.Longitude = 0.0;				//<5> 经度
	GPRMCData.GroundSpeed = 0.0;			//<7> 地面速率（单位：千米/小时）
	GPRMCData.Course = 0.0;					//<8> 地面航向（000.0~359.9度，以真北为参考基准）
	GPRMCData.Day = 1;						//日
	GPRMCData.Month = 1;					//月
	GPRMCData.Year = 2000;					//年
	GPRMCData.MagVar = 0.0;					//<10> 磁偏角【000.0~180.0度，(+)偏东/(-)偏西】
	GPRMCData.Count = 0;					//GPRMC 数据计数器

	//
	// GPZDA
	//
	GPZDAData.Hour = 0;						//
	GPZDAData.Minute = 0;					//
	GPZDAData.Second = 0;					//
	GPZDAData.Day = 1;						// 1 - 31
	GPZDAData.Month = 1;					// 1 - 12
	GPZDAData.Year = 2000;					//
	GPZDAData.LocalZoneHour = 0;			// 0 ~±13
	GPZDAData.LocalZoneMinute = 0;			// 0 - 59
	GPZDAData.Count = 0;					//
}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  GPS串口数据流处理函数		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void ProcessNMEA(INT8U btData)
{
	switch(m_nState)
	{
		///////////////////////////////////////////////////////////////////////
		//等待'$'的到来
		case NP_STATE_SOM :
			if(btData == '$')
			{
				m_btChecksum = 0;			// checksum	清零
				m_wIndex = 0;				// index 清零
				m_nState = NP_STATE_CMD;	//转到接收协议头
			}
		break;
		///////////////////////////////////////////////////////////////////////
		//接收协议头
		case NP_STATE_CMD :
			if(btData != ',' && btData != '*')
			{
				m_pCommand[m_wIndex++] = btData;
				m_btChecksum ^= btData;

				if(m_wIndex >= NP_MAX_CMD_LEN)//协议头超出长度？
				{
					m_nState = NP_STATE_SOM;
				}
			}
			else
			{
				m_pCommand[m_wIndex] = '\0';	//添加结束符
				m_btChecksum ^= btData;
				m_wIndex = 0;
				m_nState = NP_STATE_DATA;		//转到接收数据
			}
		break;
		///////////////////////////////////////////////////////////////////////
		// 接收数据
		case NP_STATE_DATA :
			if(btData == '*') //接收到校验码字段起始符？
			{
				m_pData[m_wIndex] = '\0'; //数据接收结束
				m_nState = NP_STATE_CHECKSUM_1;//转到计算校验码1
			}
			else 
			{
				if(btData == '\r')//如果收不到‘*’校验码字段起始符，那么
				{
					m_pData[m_wIndex] = '\0';
					ProcessCommand(m_pCommand, m_pData);
					m_nState = NP_STATE_SOM; //回到初始状态
					return;
				}

   			    m_btChecksum ^= btData;
				m_pData[m_wIndex] = btData;
				if(++m_wIndex >= NP_MAX_DATA_LEN) //检查数据是否超出规定长度
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
//按照不同的协议头，分配不同的处理方式
///////////////////////////////////////////////////////////////////////////////
BOOLEAN ProcessCommand(INT8U *pCommand, INT8U *pData)
{
	//
	// GPGGA数据处理
	//
	if( strcmp((char *)pCommand, "GPGGA") == NULL )
	{
		ProcessGPGGA(pData);
		OSSemPost(GPS_Sem);
	}

	//
	// GPGSA数据处理
	//
	else if( strcmp((char *)pCommand, "GPGSA") == NULL )
	{
		ProcessGPGSA(pData);
	}

	//
	// GPGSV数据处理
	//
	else if( strcmp((char *)pCommand, "GPGSV") == NULL )
	{
		ProcessGPGSV(pData);
	}

	//
	// GPRMB数据处理
	//
	else if( strcmp((char *)pCommand, "GPRMB") == NULL )
	{
		ProcessGPRMB(pData);
	}

	//
	// GPRMC数据处理
	//
	else if( strcmp((char *)pCommand, "GPRMC") == NULL )
	{
		ProcessGPRMC(pData);
	}

	//
	// GPZDA数据处理
	//
	else if( strcmp((char *)pCommand, "GPZDA") == NULL )
	{
		ProcessGPZDA(pData);
	}
	//可以效仿以上形式添加自己的GPS模块特有的数据处理
	m_dwCommandCount++;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	检查卫星是否有效
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
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇   函数：GetField(INT8U *pData, INT8U *pField, INT32S nFieldNum, INT32S nMaxFieldLen) 		  ▇
▇	 功能：用于截取指定字段的内容																  ▇		  
▇																								  ▇
▇	 *pData       是原始数据指针；																  ▇
▇	 *pField      是目标数组指针，也就是要存放的地方；											  ▇
▇	 nFieldNum    是字段编号（参见下例）；														  ▇
▇	 nMaxFieldLen 字段的最大长度；																  ▇
▇																								  ▇
▇  例如：																						  ▇
▇	$GPRMC,151007.807,A,3119.8386,N,12042.3768,E,2.66,177.69,110112, ,  ,A*6D					  ▇
▇	$GPRMC,   0      ,1,    2    ,3,     4    ,5,  6 ,   7  ,   8  ,9,10,11						  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
BOOLEAN GetField(INT8U *pData, INT8U *pField, INT32S nFieldNum, INT32S nMaxFieldLen)
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

 	//复制数据
	i2 = 0;
	while(pData[i] != ',' && pData[i] != '*' && pData[i])
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
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名：  ProcessGPRMC(INT8U *pData)														  ▇
▇	  功能  :   处理GPRMC数据    	 													 	      ▇
▇    																							  ▇
▇  数据实例：																					  ▇
▇	//$GPRMC,145048.941,V,         , ,          , ,    ,      ,110112, ,  ,N*4F					  ▇
▇	//$GPRMC,151007.807,A,3119.8386,N,12042.3768,E,2.66,177.69,110112, ,  ,A*6D					  ▇
▇	//$GPRMC,   0      ,1,    2    ,3,     4    ,5,  6 ,   7  ,   8  ,9,10,11					  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void ProcessGPRMC(INT8U *pData)
{
	char pBuff[10];
	INT8U pField[MAXFIELD];

	//分检时间<0>
	if(GetField(pData, pField, 0, MAXFIELD))
	{
	    //小时
		pBuff[0] = pField[0];
		pBuff[1] = pField[1];
		pBuff[2] = '\0';
		GPRMCData.Hour = atoi(pBuff);

		//分
		pBuff[0] = pField[2];
		pBuff[1] = pField[3];
		pBuff[2] = '\0';
		GPRMCData.Minute = atoi(pBuff);

		//秒
		pBuff[0] = pField[4];
		pBuff[1] = pField[5];
		pBuff[2] = '\0';
		GPRMCData.Second = atoi(pBuff);
	}

	//数据是否有效的标志<1>
	if(GetField(pData, pField, 1, MAXFIELD))
	{
		GPRMCData.DataValid = pField[0];
	}
	else
	{
		GPRMCData.DataValid = 'V';
	}

	//纬度<2>
	if(GetField(pData, pField, 2, MAXFIELD))
	{
		GPRMCData.Latitude = atof((char *)pField+2) / 60.0;
		pField[2] = '\0';
		GPRMCData.Latitude += atof((char *)pField);

	}
	//南纬/北纬标志位处理
	if(GetField(pData, pField, 3, MAXFIELD))
	{
		if(pField[0] == 'S')
		{
			GPRMCData.Latitude = -GPRMCData.Latitude;
		}
	}

	//经度<4>
	if(GetField(pData, pField, 4, MAXFIELD))
	{
		GPRMCData.Longitude = atof((char *)pField+3) / 60.0;
		pField[3] = '\0';
		GPRMCData.Longitude += atof((char *)pField);
	}
	//东经/西经标志位处理
	if(GetField(pData, pField, 5, MAXFIELD))
	{
		if(pField[0] == 'W')
		{
			GPRMCData.Longitude = -GPRMCData.Longitude;
		}
	}

	//水平移动速度<6>
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
	//CTU日期数据
	if(GetField(pData, pField, 8, MAXFIELD))
	{	
		//日
		pBuff[0] = pField[0];
		pBuff[1] = pField[1];
		pBuff[2] = '\0';
		GPRMCData.Day = atoi(pBuff);
		
		//月
		pBuff[0] = pField[2];
		pBuff[1] = pField[3];
		pBuff[2] = '\0';
		GPRMCData.Month = atoi(pBuff);

		//年
		pBuff[0] = pField[4];
		pBuff[1] = pField[5];
		pBuff[2] = '\0';
		GPRMCData.Year = atoi(pBuff);
		GPRMCData.Year += 2000;				
	}


	//<9> 磁偏角【000.0~180.0度，(+)偏东/(-)偏西】
	if(GetField(pData, pField, 9, MAXFIELD))
	{
		GPRMCData.MagVar = atof((char *)pField);
	}
	else
	{
		GPRMCData.MagVar = 0.0;
	}
	//<11> 磁偏角方向，E（东）或W（西）
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
