#ifndef  GPS_H
#define  GPS_H

#ifdef   GPS_GLOBALS
#define  GPS_EXT  
#else
#define  GPS_EXT  extern  
#endif

#define NP_MAX_CMD_LEN			8		// Э���ʶͷ����ֽ��� 
#define NP_MAX_DATA_LEN			512		// �������ݻ�������ֽ���
#define NP_MAX_CHAN				36		// ���ͨ����
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
	NP_STATE_SOM =	0,	       	        // �ȴ���$����״̬
	NP_STATE_CMD,						// ����Э��ͷ��״̬�����磺�յ���$���ַ���ȴ����ա�GPRMC��
	NP_STATE_DATA,						// ��ȡЭ�����ݵ�״̬
	NP_STATE_CHECKSUM_1,				// ��ȡ��1��У��͵�״̬
	NP_STATE_CHECKSUM_2,				// ��ȡ��2��У��͵�״̬
} NP_STATE;

/*
$GPGGA,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,M,<10>,M,<11>,<12>*hh<CR><LF>
<1> UTCʱ�䣬hhmmss��ʱ���룩��ʽ
<2> γ��ddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<3> γ�Ȱ���N�������򣩻�S���ϰ���
<4> ����dddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<5> ���Ȱ���E����������W��������
<6> GPS״̬��0=δ��λ��1=�ǲ�ֶ�λ��2=��ֶ�λ��6=���ڹ���
<7> ����ʹ�ý���λ�õ�����������00~12����ǰ���0Ҳ�������䣩
<8> HDOPˮƽ�������ӣ�0.5~99.9��
<9> ���θ߶ȣ�-9999.9~99999.9��
<10> ������������Դ��ˮ׼��ĸ߶�
<11> ���ʱ�䣨�����һ�ν��յ�����źſ�ʼ��������������ǲ�ֶ�λ��Ϊ�գ�
<12> ���վID��0000~1023��ǰ���0Ҳ�������䣬������ǲ�ֶ�λ��Ϊ�գ�
*/
typedef	struct tag_GPGGA_DATA 
{ 
	INT8U 	Hour;							//
	INT8U 	Minute;							//
	INT8U 	Second;							//
	FP64 	Latitude;						// С��0 = ��γ, ����0 = ��γ
	FP64 	Longitude;						// С��0 = ����, ����0 = ����
	INT8U 	GPSQuality;						// <6> GPS״̬��0=δ��λ��1=�ǲ�ֶ�λ��2=��ֶ�λ��6=���ڹ���
	INT8U 	NumOfSatsInUse;					//
	FP64 	HDOP;							//
	FP64 	Altitude;						// ���θ߶�
	INT32U 	Count;							//
	INT32S 	OldVSpeedSeconds;				//
	FP64 	OldVSpeedAlt;					//
	FP64 	VertSpeed;						//
} GPGGA_DATA;

GPS_EXT  GPGGA_DATA GPGGAData;


/*
$GPGSA,<1>,<2>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<4>,<5>,<6>*hh<CR><LF>
<1> ģʽ��M=�ֶ���A=�Զ�
<2> ��λ���ͣ�1=û�ж�λ��2=2D��λ��3=3D��λ
<3> PRN�루α��������룩���������ڽ���λ�õ����Ǻţ�01~32��ǰ���0Ҳ�������䣩��
<4> PDOPλ�þ������ӣ�0.5~99.9��
<5> HDOPˮƽ�������ӣ�0.5~99.9��
<6> VDOP��ֱ�������ӣ�0.5~99.9��
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
$GPGSV,<1>,<2>,<3>,<4>,<5>,<6>,<7>,��<4>,<5>,<6>,<7>*hh<CR><LF>
<1> GSV��������
<2> ����GSV�ı��
<3> �ɼ����ǵ�������00~12��ǰ���0Ҳ�������䣩
<4> PRN�루α��������룩��01~32��ǰ���0Ҳ�������䣩
<5> �������ǣ�00~90�ȣ�ǰ���0Ҳ�������䣩
<6> ���Ƿ�λ�ǣ�000~359�ȣ�ǰ���0Ҳ�������䣩
<7> ����ȣ�00~99dB��û�и��ٵ�����ʱΪ�գ�ǰ���0Ҳ�������䣩
ע��<4>,<5>,<6>,<7>��Ϣ������ÿ�����ǽ���ѭ����ʾ��ÿ��GSV�����������ʾ4�����ǵ���Ϣ��
    ����������Ϣ������һ���е�NMEA0183����������
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
<1> UTCʱ�䣬hhmmss��ʱ���룩��ʽ
<2> ��λ״̬��A=��Ч��λ��V=��Ч��λ
<3> γ��ddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<4> γ�Ȱ���N�������򣩻�S���ϰ���
<5> ����dddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<6> ���Ȱ���E����������W��������
<7> �������ʣ�000.0~999.9�ڣ�ǰ���0Ҳ�������䣩
<8> ���溽��000.0~359.9�ȣ����汱Ϊ�ο���׼��ǰ���0Ҳ�������䣩
<9> UTC���ڣ�ddmmyy�������꣩��ʽ
<10> ��ƫ�ǣ�000.0~180.0�ȣ�ǰ���0Ҳ�������䣩
<11> ��ƫ�Ƿ���E��������W������
<12> ģʽָʾ����NMEA0183 3.00�汾�����A=������λ��D=��֣�E=���㣬N=������Ч��
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
	INT8U 	LocalZoneHour;					// 0 ~ ��13
	INT8U 	LocalZoneMinute;				// 0 ~ 59
	INT32U 	Count;							//
} GPZDA_DATA;

GPS_EXT  GPZDA_DATA  GPZDAData;

/*
һ�� NMEA0183��׼���
1�� Global Positioning System Fix Data��GGA��GPS��λ��Ϣ
$GPGGA,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,M,<10>,M,<11>,<12>*hh<CR><LF>
<1> UTCʱ�䣬hhmmss��ʱ���룩��ʽ
<2> γ��ddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<3> γ�Ȱ���N�������򣩻�S���ϰ���
<4> ����dddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<5> ���Ȱ���E����������W��������
<6> GPS״̬��0=δ��λ��1=�ǲ�ֶ�λ��2=��ֶ�λ��6=���ڹ���
<7> ����ʹ�ý���λ�õ�����������00~12����ǰ���0Ҳ�������䣩
<8> HDOPˮƽ�������ӣ�0.5~99.9��
<9> ���θ߶ȣ�-9999.9~99999.9��
<10> ������������Դ��ˮ׼��ĸ߶�
<11> ���ʱ�䣨�����һ�ν��յ�����źſ�ʼ��������������ǲ�ֶ�λ��Ϊ�գ�
<12> ���վID��0000~1023��ǰ���0Ҳ�������䣬������ǲ�ֶ�λ��Ϊ�գ�


2�� GPS DOP and Active Satellites��GSA����ǰ������Ϣ
$GPGSA,<1>,<2>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<3>,<4>,<5>,<6>*hh<CR><LF>
<1> ģʽ��M=�ֶ���A=�Զ�
<2> ��λ���ͣ�1=û�ж�λ��2=2D��λ��3=3D��λ
<3> PRN�루α��������룩���������ڽ���λ�õ����Ǻţ�01~32��ǰ���0Ҳ�������䣩��
<4> PDOPλ�þ������ӣ�0.5~99.9��
<5> HDOPˮƽ�������ӣ�0.5~99.9��
<6> VDOP��ֱ�������ӣ�0.5~99.9��


3�� GPS Satellites in View��GSV���ɼ�������Ϣ
$GPGSV,<1>,<2>,<3>,<4>,<5>,<6>,<7>,��<4>,<5>,<6>,<7>*hh<CR><LF>
<1> GSV��������
<2> ����GSV�ı��
<3> �ɼ����ǵ�������00~12��ǰ���0Ҳ�������䣩
<4> PRN�루α��������룩��01~32��ǰ���0Ҳ�������䣩
<5> �������ǣ�00~90�ȣ�ǰ���0Ҳ�������䣩
<6> ���Ƿ�λ�ǣ�000~359�ȣ�ǰ���0Ҳ�������䣩
<7> ����ȣ�00~99dB��û�и��ٵ�����ʱΪ�գ�ǰ���0Ҳ�������䣩
ע��<4>,<5>,<6>,<7>��Ϣ������ÿ�����ǽ���ѭ����ʾ��ÿ��GSV�����������ʾ4�����ǵ���Ϣ������������Ϣ������һ���е�NMEA0183����������


4�� Recommended Minimum Specific GPS/TRANSIT Data��RMC���Ƽ���λ��Ϣ
$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>
<1> UTCʱ�䣬hhmmss��ʱ���룩��ʽ
<2> ��λ״̬��A=��Ч��λ��V=��Ч��λ
<3> γ��ddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<4> γ�Ȱ���N�������򣩻�S���ϰ���
<5> ����dddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<6> ���Ȱ���E����������W��������
<7> �������ʣ�000.0~999.9�ڣ�ǰ���0Ҳ�������䣩
<8> ���溽��000.0~359.9�ȣ����汱Ϊ�ο���׼��ǰ���0Ҳ�������䣩
<9> UTC���ڣ�ddmmyy�������꣩��ʽ
<10> ��ƫ�ǣ�000.0~180.0�ȣ�ǰ���0Ҳ�������䣩
<11> ��ƫ�Ƿ���E��������W������
<12> ģʽָʾ����NMEA0183 3.00�汾�����A=������λ��D=��֣�E=���㣬N=������Ч��


5�� Track Made Good and Ground Speed��VTG�������ٶ���Ϣ
$GPVTG,<1>,T,<2>,M,<3>,N,<4>,K,<5>*hh<CR><LF>
<1> ���汱Ϊ�ο���׼�ĵ��溽��000~359�ȣ�ǰ���0Ҳ�������䣩
<2> �Դű�Ϊ�ο���׼�ĵ��溽��000~359�ȣ�ǰ���0Ҳ�������䣩
<3> �������ʣ�000.0~999.9�ڣ�ǰ���0Ҳ�������䣩
<4> �������ʣ�0000.0~1851.8����/Сʱ��ǰ���0Ҳ�������䣩
<5> ģʽָʾ����NMEA0183 3.00�汾�����A=������λ��D=��֣�E=���㣬N=������Ч��


6�� Geographic Position��GLL����λ������Ϣ
$GPGLL,<1>,<2>,<3>,<4>,<5>,<6>,<7>*hh<CR><LF>
<1> γ��ddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<2> γ�Ȱ���N�������򣩻�S���ϰ���
<3> ����dddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<4> ���Ȱ���E����������W��������
<5> UTCʱ�䣬hhmmss��ʱ���룩��ʽ
<6> ��λ״̬��A=��Ч��λ��V=��Ч��λ
<7> ģʽָʾ����NMEA0183 3.00�汾�����A=������λ��D=��֣�E=���㣬N=������Ч��


���� GARMIN��������
7�� Estimated Error Information��PGRME�����������Ϣ
$PGRME,<1>,M,<2>,M,<3>,M*hh<CR><LF>
<1> HPE��ˮƽ��������0.0~999.9��
<2> VPE����ֱ��������0.0~999.9��
<3> EPE��λ�ù�������0.0~999.9��


8�� GPS Fix Data Sentence��PGRMF��GPS��λ��Ϣ
$PGRMF,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>,<13>,<14>,<15>*hh<CR><LF>
<1> GPS������0~1023��
<2> GPS������0~604799��
<3> UTC���ڣ�ddmmyy�������꣩��ʽ
<4> UTCʱ�䣬hhmmss��ʱ���룩��ʽ
<5> GPS������
<6> γ��ddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<7> γ�Ȱ���N�������򣩻�S���ϰ���
<8> ����dddmm.mmmm���ȷ֣���ʽ��ǰ���0Ҳ�������䣩
<9> ���Ȱ���E����������W��������
<10> ģʽ��M=�ֶ���A=�Զ�
<11> ��λ���ͣ�0=û�ж�λ��1=2D��λ��2=3D��λ
<12> �������ʣ�0~1851����/Сʱ��
<13> ���溽��000~359�ȣ����汱Ϊ�ο���׼��
<14> PDOPλ�þ������ӣ�0~9����������ȡ����
<15> TDOPʱ�侫�����ӣ�0~9����������ȡ����


9�� Map Datum��PGRMM������ϵͳ��Ϣ
$PGRMM,<1>*hh<CR><LF>
<1> ��ǰʹ�õ�����ϵ���ƣ����ݳ��ȿɱ䣬�硰WGS 84����
ע������Ϣ����MapSource����ʵʱ���ӵ�ʱ��ʹ�á�


10�� Sensor Status Information��PGRMT������״̬��Ϣ
$PGRMT,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>*hh<CR><LF>
<1> ��Ʒ�ͺź�����汾�����ݳ��ȿɱ䣬�硰GPS 15L/15H VER 2.05����
<2> ROMУ����ԣ�P=ͨ����F=ʧ��
<3> ���ջ����������ϣ�P=ͨ����F=ʧ��
<4> �洢�����ݣ�R=���֣�L=��ʧ
<5> ʱ�ӵ���Ϣ��R=���֣�L=��ʧ
<6> ����������Ư�ƣ�P=ͨ����F=��⵽����Ư��
<7> ���ݲ������ɼ���C=���ڲɼ������û�вɼ���Ϊ��
<8> GPS���ջ��¶ȣ���λΪ���϶�
<9> GPS���ջ��������ݣ�R=���֣�L=��ʧ
ע�������ÿ���ӷ���һ�Σ�����ѡ��Ĳ������޹ء�


11�� 3D velocity Information��PGRMV����ά�ٶ���Ϣ
$PGRMV,<1>,<2>,<3>*hh<CR><LF>
<1> �����ٶȣ�514.4~514.4��/��
<2> �����ٶȣ�514.4~514.4��/��
<3> �����ٶȣ�999.9~9999.9��/��


12�� DGPS Beacon Information��PGRMB���ű�����Ϣ
$PGRMB,<1>,<2>,<3>,<4>,<5>,K,<6>,<7>,<8>*hh<CR><LF>
<1> �ű�վƵ�ʣ�0.0��283.5~325.0kHz�����Ϊ0.5kHz��
<2> �ű�����ʣ�0��25��50��100��200bps��
<3> SNR�ű��ź�����ȣ�0~31��
<4> �ű�����������0~100��
<5> ���ű�վ�ľ��룬��λΪ����
<6> �ű���ջ���ͨѶ״̬��0=�����ߣ�1=���źţ�2=���ڵ�г��3=���ڽ��գ�4=����ɨ��
<7> ���Դ��R=RTCM��W=WAAS��N=�ǲ�ֶ�λ
<8> ���״̬��A=�Զ���W=��ΪWAAS��R=��ΪRTCM��N=�����ղ���ź�

ע��hh��У��ͣ�$��*֮�������ַ������У��ͣ����ֽ���������㣬�õ�У��ͺ���ת��16���Ƹ�ʽ��ASCII�ַ�����
ע��<CR><LF>--֡�������س��ͻ���
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
