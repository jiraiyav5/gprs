#ifndef  GPS_H
#define  GPS_H

#ifdef   GPS_GLOBALS
#define  GPS_EXT  
#else
#define  GPS_EXT  extern  
#endif

#define NP_MAX_CMD_LEN			8		// 协议标识头最大字节数 
#define NP_MAX_DATA_LEN			512		// 串口数据缓存最大字节数
#define NP_MAX_CHAN				36		// 最大通道数
#define NP_WAYPOINT_ID_LEN		32		

GPS_EXT	INT32U			m_dwCommandCount;			

extern INT8U  STATUS_str[200];
extern INT8U  GPSinf[100];


typedef	struct tag_SatInfo { 
	INT16U	PRN;						//
	INT16U	SignalQuality;				//
	BOOLEAN	UsedInSolution;				//
	INT16U	Azimuth;					//
	INT16U	Elevation;					//
}SatInfo;

typedef enum tag_NP_STATE 
{
	NP_STATE_SOM =	0,	       	        // 等待‘$’的状态
	NP_STATE_CMD,						// 接收协议头的状态，例如：收到‘$’字符后等待接收“GPRMC”
	NP_STATE_DATA,						// 获取协议数据的状态
	NP_STATE_CHECKSUM_1,				// 获取第1个校验和的状态
	NP_STATE_CHECKSUM_2,				// 获取第2个校验和的状态
} NP_STATE;

/*
$GPGGA,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,M,<10>,M,<11>,<12>*hh<CR><LF>
<1> UTC时间，hhmmss（时分秒）格式
<2> 纬度ddmm.mmmm（度分）格式（前面的0也将被传输）
<3> 纬度半球N（北半球）或S（南半球）
<4> 经度dddmm.mmmm（度分）格式（前面的0也将被传输）
<5> 经度半球E（东经）或W（西经）
<6> GPS状态：0=未定位，1=非差分定位，2=差分定位，6=正在估算
<7> 正在使用解算位置的卫星数量（00~12）（前面的0也将被传输）
<8> HDOP水平精度因子（0.5~99.9）
<9> 海拔高度（-9999.9~99999.9）
<10> 地球椭球面相对大地水准面的高度
<11> 差分时间（从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空）
<12> 差分站ID号0000~1023（前面的0也将被传输，如果不是差分定位将为空）
*/
typedef	struct tag_GPGGA_DATA 
{ 
	INT8U 	Hour;							//
	INT8U 	Minute;							//
	INT8U 	Second;							//
	FP64 	Latitude;						// 小于0 = 南纬, 大于0 = 北纬
	FP64 	Longitude;						// 小于0 = 西经, 大于0 = 东经
	INT8U 	GPSQuality;						// <6> GPS状态：0=未定位，1=非差分定位，2=差分定位，6=正在估算
	INT8U 	NumOfSatsInUse;					//
	FP64 	HDOP;							//
	FP64 	Altitude;						// 海拔高度
	INT32U 	Count;							//
	INT32S 	OldVSpeedSeconds;				//
	FP64 	OldVSpeedAlt;					//
	FP64 	VertSpeed;						//
} GPGGA_DATA;

GPS_EXT  GPGGA_DATA GPGGAData;


/*
$GPGSA,<1>,<2>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<4>,<5>,<6>*hh<CR><LF>
<1> 模式，M=手动，A=自动
<2> 定位类型，1=没有定位，2=2D定位，3=3D定位
<3> PRN码（伪随机噪声码），正在用于解算位置的卫星号（01~32，前面的0也将被传输）。
<4> PDOP位置精度因子（0.5~99.9）
<5> HDOP水平精度因子（0.5~99.9）
<6> VDOP垂直精度因子（0.5~99.9）
*/
typedef	struct tag_GPGSA_DATA 
{ 
	INT8U 	Mode;						
	INT8U 	FixMode;						
	INT16U 	SatsInSolution[NP_MAX_CHAN]; 	
	FP64 	PDOP;							
	FP64 	HDOP;							
	FP64 	VDOP;							
	INT32U 	Count;							
} GPGSA_DATA;

GPS_EXT  GPGSA_DATA GPGSAData; 

/*
$GPGSV,<1>,<2>,<3>,<4>,<5>,<6>,<7>,…<4>,<5>,<6>,<7>*hh<CR><LF>
<1> GSV语句的总数
<2> 本句GSV的编号
<3> 可见卫星的总数（00~12，前面的0也将被传输）
<4> PRN码（伪随机噪声码）（01~32，前面的0也将被传输）
<5> 卫星仰角（00~90度，前面的0也将被传输）
<6> 卫星方位角（000~359度，前面的0也将被传输）
<7> 信噪比（00~99dB，没有跟踪到卫星时为空，前面的0也将被传输）
注：<4>,<5>,<6>,<7>信息将按照每颗卫星进行循环显示，每条GSV语句最多可以显示4颗卫星的信息。
    其他卫星信息将在下一序列的NMEA0183语句中输出。
*/
typedef	struct tag_GPGSV_DATA 
{ 
	INT8U 	TotalNumOfMsg;					
	INT16U 	TotalNumSatsInView;				
	SatInfo SatInfo[NP_MAX_CHAN];			
	INT32U 	Count;							
} GPGSV_DATA;

GPS_EXT  GPGSV_DATA GPGSVData;

//
// GPRMB
//
typedef	struct tag_GPRMB_DATA 
{ 
	INT8U 	DataStatus;						
	FP64 	CrosstrackError;				
	INT8U 	DirectionToSteer;				
	char 	OriginWaypoint[NP_WAYPOINT_ID_LEN]; 
	char 	DestWaypoint[NP_WAYPOINT_ID_LEN]; 
	FP64 	DestLatitude;					
	FP64 	DestLongitude;					
	FP64 	RangeToDest;					 
	FP64 	BearingToDest;					 
	FP64 	DestClosingVelocity;			 
	INT8U 	ArrivalStatus;					 
	INT32U Count;						
} GPRMB_DATA;

GPS_EXT  GPRMB_DATA  GPRMBData;

/*
$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>
<1> UTC时间，hhmmss（时分秒）格式
<2> 定位状态，A=有效定位，V=无效定位
<3> 纬度ddmm.mmmm（度分）格式（前面的0也将被传输）
<4> 纬度半球N（北半球）或S（南半球）
<5> 经度dddmm.mmmm（度分）格式（前面的0也将被传输）
<6> 经度半球E（东经）或W（西经）
<7> 地面速率（000.0~999.9节，前面的0也将被传输）
<8> 地面航向（000.0~359.9度，以真北为参考基准，前面的0也将被传输）
<9> UTC日期，ddmmyy（日月年）格式
<10> 磁偏角（000.0~180.0度，前面的0也将被传输）
<11> 磁偏角方向，E（东）或W（西）
<12> 模式指示（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）
*/
typedef	struct tag_GPRMC_DATA 
{ 
	INT8U 	Hour;							//
	INT8U 	Minute;							//
	INT8U 	Second;							//
	INT8U 	DataValid;						// 
	FP64 	Latitude;						// 
	FP64 	Longitude;						// 
	FP64 	GroundSpeed;					// 
	FP64 	Course;							// 
	INT8U 	Day;							//
	INT8U 	Month;							//
	INT16U 	Year;							//
	FP64 	MagVar;							// 
	INT32U 	Count;							//
} GPRMC_DATA; 

extern  GPRMC_DATA  GPRMCData;

//
// GPZDA
//
typedef	struct tag_GPZDA_DATA 
{ 
	INT8U 	Hour;							//
	INT8U 	Minute;							//
	INT8U 	Second;							//
	INT8U 	Day;							// 1 ~ 31
	INT8U 	Month;							// 1 ~ 12
	INT16U 	Year;							//
	INT8U 	LocalZoneHour;					// 0 ~ ±13
	INT8U 	LocalZoneMinute;				// 0 ~ 59
	INT32U 	Count;							//
} GPZDA_DATA;

GPS_EXT  GPZDA_DATA  GPZDAData;

/*
一、 NMEA0183标准语句
1、 Global Positioning System Fix Data（GGA）GPS定位信息
$GPGGA,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,M,<10>,M,<11>,<12>*hh<CR><LF>
<1> UTC时间，hhmmss（时分秒）格式
<2> 纬度ddmm.mmmm（度分）格式（前面的0也将被传输）
<3> 纬度半球N（北半球）或S（南半球）
<4> 经度dddmm.mmmm（度分）格式（前面的0也将被传输）
<5> 经度半球E（东经）或W（西经）
<6> GPS状态：0=未定位，1=非差分定位，2=差分定位，6=正在估算
<7> 正在使用解算位置的卫星数量（00~12）（前面的0也将被传输）
<8> HDOP水平精度因子（0.5~99.9）
<9> 海拔高度（-9999.9~99999.9）
<10> 地球椭球面相对大地水准面的高度
<11> 差分时间（从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空）
<12> 差分站ID号0000~1023（前面的0也将被传输，如果不是差分定位将为空）


2、 GPS DOP and Active Satellites（GSA）当前卫星信息
$GPGSA,<1>,<2>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<4>,<5>,<6>*hh<CR><LF>
<1> 模式，M=手动，A=自动
<2> 定位类型，1=没有定位，2=2D定位，3=3D定位
<3> PRN码（伪随机噪声码），正在用于解算位置的卫星号（01~32，前面的0也将被传输）。
<4> PDOP位置精度因子（0.5~99.9）
<5> HDOP水平精度因子（0.5~99.9）
<6> VDOP垂直精度因子（0.5~99.9）


3、 GPS Satellites in View（GSV）可见卫星信息
$GPGSV,<1>,<2>,<3>,<4>,<5>,<6>,<7>,…<4>,<5>,<6>,<7>*hh<CR><LF>
<1> GSV语句的总数
<2> 本句GSV的编号
<3> 可见卫星的总数（00~12，前面的0也将被传输）
<4> PRN码（伪随机噪声码）（01~32，前面的0也将被传输）
<5> 卫星仰角（00~90度，前面的0也将被传输）
<6> 卫星方位角（000~359度，前面的0也将被传输）
<7> 信噪比（00~99dB，没有跟踪到卫星时为空，前面的0也将被传输）
注：<4>,<5>,<6>,<7>信息将按照每颗卫星进行循环显示，每条GSV语句最多可以显示4颗卫星的信息。其他卫星信息将在下一序列的NMEA0183语句中输出。


4、 Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐定位信息
$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>
<1> UTC时间，hhmmss（时分秒）格式
<2> 定位状态，A=有效定位，V=无效定位
<3> 纬度ddmm.mmmm（度分）格式（前面的0也将被传输）
<4> 纬度半球N（北半球）或S（南半球）
<5> 经度dddmm.mmmm（度分）格式（前面的0也将被传输）
<6> 经度半球E（东经）或W（西经）
<7> 地面速率（000.0~999.9节，前面的0也将被传输）
<8> 地面航向（000.0~359.9度，以真北为参考基准，前面的0也将被传输）
<9> UTC日期，ddmmyy（日月年）格式
<10> 磁偏角（000.0~180.0度，前面的0也将被传输）
<11> 磁偏角方向，E（东）或W（西）
<12> 模式指示（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）


5、 Track Made Good and Ground Speed（VTG）地面速度信息
$GPVTG,<1>,T,<2>,M,<3>,N,<4>,K,<5>*hh<CR><LF>
<1> 以真北为参考基准的地面航向（000~359度，前面的0也将被传输）
<2> 以磁北为参考基准的地面航向（000~359度，前面的0也将被传输）
<3> 地面速率（000.0~999.9节，前面的0也将被传输）
<4> 地面速率（0000.0~1851.8公里/小时，前面的0也将被传输）
<5> 模式指示（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）


6、 Geographic Position（GLL）定位地理信息
$GPGLL,<1>,<2>,<3>,<4>,<5>,<6>,<7>*hh<CR><LF>
<1> 纬度ddmm.mmmm（度分）格式（前面的0也将被传输）
<2> 纬度半球N（北半球）或S（南半球）
<3> 经度dddmm.mmmm（度分）格式（前面的0也将被传输）
<4> 经度半球E（东经）或W（西经）
<5> UTC时间，hhmmss（时分秒）格式
<6> 定位状态，A=有效定位，V=无效定位
<7> 模式指示（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）


二、 GARMIN定义的语句
7、 Estimated Error Information（PGRME）估计误差信息
$PGRME,<1>,M,<2>,M,<3>,M*hh<CR><LF>
<1> HPE（水平估计误差），0.0~999.9米
<2> VPE（垂直估计误差），0.0~999.9米
<3> EPE（位置估计误差），0.0~999.9米


8、 GPS Fix Data Sentence（PGRMF）GPS定位信息
$PGRMF,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>,<13>,<14>,<15>*hh<CR><LF>
<1> GPS周数（0~1023）
<2> GPS秒数（0~604799）
<3> UTC日期，ddmmyy（日月年）格式
<4> UTC时间，hhmmss（时分秒）格式
<5> GPS跳秒数
<6> 纬度ddmm.mmmm（度分）格式（前面的0也将被传输）
<7> 纬度半球N（北半球）或S（南半球）
<8> 经度dddmm.mmmm（度分）格式（前面的0也将被传输）
<9> 经度半球E（东经）或W（西经）
<10> 模式，M=手动，A=自动
<11> 定位类型，0=没有定位，1=2D定位，2=3D定位
<12> 地面速率（0~1851公里/小时）
<13> 地面航向（000~359度，以真北为参考基准）
<14> PDOP位置精度因子（0~9，四舍五入取整）
<15> TDOP时间精度因子（0~9，四舍五入取整）


9、 Map Datum（PGRMM）坐标系统信息
$PGRMM,<1>*hh<CR><LF>
<1> 当前使用的坐标系名称（数据长度可变，如“WGS 84”）
注：该信息在与MapSource进行实时连接的时候使用。


10、 Sensor Status Information（PGRMT）工作状态信息
$PGRMT,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>*hh<CR><LF>
<1> 产品型号和软件版本（数据长度可变，如“GPS 15L/15H VER 2.05”）
<2> ROM校验测试，P=通过，F=失败
<3> 接收机不连续故障，P=通过，F=失败
<4> 存储的数据，R=保持，L=丢失
<5> 时钟的信息，R=保持，L=丢失
<6> 振荡器不连续漂移，P=通过，F=检测到过度漂移
<7> 数据不连续采集，C=正在采集，如果没有采集则为空
<8> GPS接收机温度，单位为摄氏度
<9> GPS接收机配置数据，R=保持，L=丢失
注：本语句每分钟发送一次，与所选择的波特率无关。


11、 3D velocity Information（PGRMV）三维速度信息
$PGRMV,<1>,<2>,<3>*hh<CR><LF>
<1> 东向速度，514.4~514.4米/秒
<2> 北向速度，514.4~514.4米/秒
<3> 上向速度，999.9~9999.9米/秒


12、 DGPS Beacon Information（PGRMB）信标差分信息
$PGRMB,<1>,<2>,<3>,<4>,<5>,K,<6>,<7>,<8>*hh<CR><LF>
<1> 信标站频率（0.0，283.5~325.0kHz，间隔为0.5kHz）
<2> 信标比特率（0，25，50，100或200bps）
<3> SNR信标信号信噪比（0~31）
<4> 信标数据质量（0~100）
<5> 与信标站的距离，单位为公里
<6> 信标接收机的通讯状态，0=检查接线，1=无信号，2=正在调谐，3=正在接收，4=正在扫描
<7> 差分源，R=RTCM，W=WAAS，N=非差分定位
<8> 差分状态，A=自动，W=仅为WAAS，R=仅为RTCM，N=不接收差分信号

注：hh—校验和，$与*之间所有字符代码的校验和（各字节做异或运算，得到校验和后，再转换16进制格式的ASCII字符。）
注：<CR><LF>--帧结束，回车和换行
*/

void    GPS_Init(void);              			 

void    GPS_RSTIO_INIT(void);
void    GPS_RST(unsigned char x);

void    GPS_RxHandler(INT8U data);            	 
void    GPS_TxHandler(void);                  	 

void    GPS_InitTarget(void);     			 	

void    GPS_RxTxISR(void);                       
void    GPS_RxTxISRHandler(void);                

void    GPS_RxISR(void);                         
void    GPS_RxISRHandler(void);                  
void    GPS_RxIntEn(void);                     
void    GPS_RxIntDis(void);                     

void    GPS_TxISR(void);                       
void    GPS_TxISRHandler(void);             
void    GPS_TxIntEn(void);                   
void    GPS_TxIntDis(void);           
void    GPS_Tx1(INT8U c);                  

BOOLEAN GetField(INT8U *pData, INT8U *pField, INT32S nFieldNum, INT32S nMaxFieldLen);
BOOLEAN IsSatUsedInSolution(INT16U wSatID);
BOOLEAN ProcessCommand(INT8U *pCommand, INT8U *pData);

void GPDataReset(void);
void ProcessNMEA(INT8U btData);
void ProcessGPZDA(INT8U *pData);
void ProcessGPRMC(INT8U *pData);
void ProcessGPRMB(INT8U *pData);
void ProcessGPGSV(INT8U *pData);
void ProcessGPGSA(INT8U *pData);
void ProcessGPGGA(INT8U *pData);

#endif
