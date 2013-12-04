


typedef enum _GSM_RX_STATE 
{
	RX_STATE_IDLE =	0,	       	        // 等待‘\r\n’字符的状态
	RX_STATE_CMD,						// 接收协议头的状态，例如：收到‘\r\n’字符后等待接收“GPRMC”
	RX_STATE_DATA,						// 获取协议数据的状态
} GSM_RX_STATE;


typedef	struct GSM_CMT_Info      //短信	"\r\n+CMT: "+8615990472896",,"10/07/29,19:24:56+32"\r\n短信内容\r\n
{ 
  INT8U    isNew; //标记短信是否为新收到的短信；
  INT8U    PhoneNum[20]; //发送方的号码；
  INT8U	   Date_Time[25];//字符型的日期和时间
  INT8U	   Content[300]; //短信内容最多70个汉字，一个汉字占4个字节
  INT8U	   OP_CDM[10];   //短信操作命令
  char	   OP_Key_ASCII[8];//操作密码,字符形式；
  INT32U   OP_Key;       //操作密码,长整型；
}GSM_CMT;


typedef	struct GSM_CSCA_Info      //短信 "\r\n+CMT: "+8615990472896",,"10/07/29,19:24:56+32"\r\n短信内容\r\n
{ 
 INT8U    isNew;
 INT8U    isOK;
 INT8U    Phone[20]; //短信中心号码；
}GSM_CSCA;
















