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
#include "includes.h"
#include "GSM.h"
#include "GSM_TypeDefine.h"
																								
#if GSM_MODULE == DEF_ENABLED																					  

static   OS_STK      GSM_TaskStk[GSM_TASK_STK_SIZE];
//static   OS_EVENT   *GSM_Sem;

extern  INT8U    GPSinf[100];
extern  const unsigned char GPSimage[153600];
extern 	GSM_RX_STATE  GSM_RX_nState;	//串口数据处理状态
extern  INT8U	  GSM_pCommand[GSM_MAX_CMD_LEN];	//
extern  INT16U    Test_counter;
extern  INT8U     Flag_CMT;
extern  INT8U     Flag_CMGR;
extern  INT8U     Flag_DATA;
extern  GSM_CMT     CMT;
extern  GSM_CSCA    CSCA;
extern  GSM_IPD     IPD;
extern  GSM_CLIP    CLIP;
extern  GSM_GSMLOC  GSMLOC;

extern  INT8U	  GSM_pCommand[GSM_MAX_CMD_LEN];	//
extern  INT8U     GSM_pData[GSM_MAX_DATA_LEN];	//

static  void    GSM_InitOS(void);
static  void    GSM_Task(void *p_arg);
void GSM_START(void);
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  GSM初始化          		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GSM_Init (void)
{
	GSM_RX_nState = RX_STATE_IDLE;
	if(GSM_ON1_OFF0()==0)GSM_START(); //启动GSM模块	注：软件调试期间最好是用手动的方式
  GSM_InitOS(); 
  GSM_InitTarget();
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  启动GSM模块 		          										  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void GSM_START(void)
{
    INT8U t=0;
  //给GSM模块的电源控制脚一个下降沿启动模块。
	GSM_POWER_KEY_HIGH();
	IWDG_ReloadCounter();//喂狗
	OSTimeDlyHMSM(0, 0, 0,500);
  GSM_POWER_KEY_LOW();
	t=3;
	while(t--)
	{
	  IWDG_ReloadCounter();//喂狗
	  OSTimeDlyHMSM(0, 0, 1, 0);
	}
  GSM_POWER_KEY_HIGH();
	OSTimeDlyHMSM(0, 0, 0,500);
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						 创建GSM任务       		          								      ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
static  void  GSM_InitOS (void)
{
    INT8U  err;

    //GSM_Sem = OSSemCreate(0);
    //OSEventNameSet(GSM_Sem, (INT8U *)"GSM Signal", &err);

    (void)OSTaskCreateExt(GSM_Task,
                          (void *)0,                                  
                          &GSM_TaskStk[GSM_TASK_STK_SIZE - 1], 		
                           GSM_TASK_PRIO,                          	
                           GSM_TASK_PRIO,
                          &GSM_TaskStk[0],                         	
                           GSM_TASK_STK_SIZE,
                          (void *)0,                                  
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); 

#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(GSM_TASK_PRIO, (INT8U *)"uC/GSM Task", &err);
#endif
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  GSM任务 		          								              ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
static  void  GSM_Task (void *p_arg)
{
//  INT8U      err;
	INT16U  i=0;

	#ifdef  GSM_SUPPORT_LBS
	INT16U  LBS_REFRESH_COUNT=0;
	#endif
	INT8U  INIT_FLAG=1,TP=0;
	ADXL345_Init();
	Key_Number_Init();


	if(GSM_ON1_OFF0()==0) {GSM_START();} 

	
	while(1) 
	 {
	  IWDG_ReloadCounter();//喂狗
	  OSTimeDlyHMSM(0, 0, 0,100);
	
    //if(GSM_ON1_OFF0()==0) {GSM_START();}
			 
    #ifdef GSM_SUPPORT_LBS
		LBS_REFRESH_COUNT++;	 
		if(INIT_FLAG==0 && LBS_REFRESH_COUNT>=300)
	     {//30秒更新一次基站定位经纬度
		    LBS_REFRESH_COUNT=0;
				GSM_LBS_Refresh(); 
		   }	 
		#endif	 
	  if(i==150 && INIT_FLAG) 
	      {
	       LCD_SHOW_CN15x16_String(0,100,"初始化SIM900模块...");
				 GSM_Configuration();
//		   GSM_GPRS_TCP_Connect("etmcu.xicp.net","5800");
		   INIT_FLAG=0;
		   //counter=0;
	      }//开机后15秒，配置GSM模块
	  
	  TP=Get_Temperature();//计算温度

#if (TFT_MODULE == DEF_ENABLED)

	  if(Flag_DATA)
	   {//刷新屏幕 
		  TFT_ShowPicture(0,0,240,320,GPSimage);//显示背景图片
		  TFT_ShowString(0,80,GSM_pCommand);
	    TFT_ShowString(0,100,GSM_pData);
	    Flag_DATA=0;
	   }
		#ifdef GSM_SUPPORT_LBS 
		if(GSMLOC.isNew)//显示基站定位结果
  	   {
       LCD_SHOW_CN15x16_String(0,195,"基站定位结果：");
			 LCD_SHOW_CN15x16_String(0,215,"纬度:");
  		 TFT_ShowString(35,215,GSMLOC.Latitude); 
			 LCD_SHOW_CN15x16_String(120,215,"经度:");
 			 TFT_ShowString(155,215,GSMLOC.Longitude);
//     GSMLOC.isNew=0;			 
  		 } 
		#endif	
	  ADXL345_Test();//显示重力加速度传感器内部关键寄存器内容
	  //TFT_ShowNum(100,260,ADC_Result[ADC_TEMP]);//显示ADC转换结果
	  //TFT_ShowString(0,260,"Temperature=");
		LCD_SHOW_CN15x16_String(0,280,"NTC温度:");		
	  TFT_ShowNum(65,280,TP); //显示计算出来的温度值
		LCD_SHOW_CN15x16_String(110,280,"计数器:");		
	  TFT_ShowNum(170,280,i++);//显示本任务运行次数计数器
		LCD_SHOW_CN15x16_String(0,300,"GPS时间:");		
	 // TFT_ShowNum(70,300,Test_counter);//显示串口接收到的字节数
#endif

	 ///////////////////////////////////
	 if(CMT.isNew)//新短信处理 
	   {
	    CMT.isNew=0;
	    //if( strcmp((char *)CMT.OP_CDM, "GPS?") == NULL && CMT.OP_Key==My_Key_Number)//需要密码
	    if( strcmp((char *)CMT.OP_CDM, "GOOGLE") == NULL)//不需要密码 
		 { 
	      if(GPRMCData.DataValid=='A')
		    {
 		     GSM_Send_TXT_CMG(CMT.PhoneNum,GPSinf);
	   	  }
   	      else 
		    {
			 #ifdef GSM_Module_is_GU900
		   GSM_Send_GB_SMS(CMT.PhoneNum,"正在搜索GPS信息，请稍候...");
			 #else
			 GSM_Send_TXT_CMG(CMT.PhoneNum,"GPS is searching,please wait...!"); 
			 #endif
			}
	   }
		 else if( strcmp((char *)CMT.OP_CDM, "LEDON") == NULL)//点亮LED灯
		 {
		   BSP_RLED_ON();
		 }
		 else if( strcmp((char *)CMT.OP_CDM, "LEDOFF") == NULL)//关掉LED灯
		 {
		   BSP_RLED_OFF();
		 }
		 else if(strcmp((char *)CMT.OP_CDM, "CALLME") == NULL)//呼叫发短信的人的手机号码
		 {
		   GSM_CALL(CMT.PhoneNum);
		 }
		 else if( strcmp((char *)CMT.OP_CDM, "STATUS") == NULL)
		 {//发送一条包含时间、经纬度、温度信息的中文短信
 		  #ifdef GSM_Module_is_GU900
 		   GSM_Send_MyStatus(CMT.PhoneNum);
 		  #else
		    GSM_Send_CN_MSG2(CMT.PhoneNum,STATUS_str);
 		  #endif
		 }

	   	#if (TFT_MODULE	 == DEF_ENABLED)
		  TFT_ShowString(0,180,CMT.PhoneNum);//显示发短信人的手机号码
			TFT_ShowString(0,200,(INT8U *)CMT.Date_Time);//显示发送时间
			TFT_ShowString(0,220,CMT.Content);//显示短信内容
			TFT_ShowString(0,240,CMT.OP_CDM); //显示短信操作命令
			TFT_ShowString(50,240,(INT8U *)CMT.OP_Key_ASCII);//显示短信操作命令中的密码(字符型的)
			TFT_ShowNum  (120,240,CMT.OP_Key); //显示短信操作命令中的密码(数值型的)
		  #endif
	   }
	 //////////////////////////////////////////////
	 //网络数据必须带识别头+IPD和数据长度，例如：\r\n+IPD,4:Google Map?\r\n
	 if(IPD.isNew)//网络数据控制
	   {
	    #if (TFT_MODULE	 == DEF_ENABLED)
		    TFT_ShowNum(0,200,IPD.DataLen);
			TFT_ShowString(0,220,IPD.Data);
		#endif
	    
		if( strcmp((char *)IPD.Data, "Google Map?") == NULL)
	     { 
          if(GPRMCData.DataValid=='A')
		    {
 		     GSM_GPRS_SEND(GPSinf);
		   //GSM_Send_TXT_CMG("15990472896",GPSinf);
	   	  }
   	      else 
		    {
			GSM_GPRS_SEND("GPS is searching,please wait!");
			//GSM_Send_TXT_CMG("15990472896","GPS is searching,please wait!");
			}
	   }
		IPD.isNew = 0;
	   }
	  ///////////////////////////////
	  if(CLIP.RING)	//来电处理
	   {
		   CLIP.RING=0;
		   #if (TFT_MODULE == DEF_ENABLED)
		    TFT_ShowString(0,180,"RING");
			  TFT_ShowNum  (120,240,CLIP.COUNT);
		   #endif
		  if(CLIP.COUNT>2) {GSM_Answer_CALL(); CLIP.COUNT=0; }  //响铃2声后自动接听电话
	   }
	  if( !Key_Left() )//按左键
	    { 
	     BSP_RLED_ON();		//点亮LED灯
		   TFT_ShowPicture(0,0,240,320,GPSimage);//显示背景图片,即清屏
		   GSM_GPRS_UDP_Connect("etmcu.xicp.net","5800");	//连接远程TCP服务器
		   //GSM_GPRS_TCP_Connect("117.83.54.228","5800");	//连接远程TCP服务器
	    }
	  if( !Key_Right() )//按右键 
	    {
		   BSP_RLED_OFF();	//熄灭LED灯
		   GSM_GPRS_SEND("$GPRMC,060525.491,A,3156.9445,N,12025.8057,E,0.59,181.00,200309,,,A*69");//发送测试数据
	    }
	  if( !Key_Up() )//按上键 
	    {
	      GSM_Answer_CALL();	//来电时，按上键接听电话
	    } 
	  if( !Key_Down()  )//按下键 
	    {
	      //while(!Key_Down());
        //GSM_Hang_UP();		//通话中，按下键挂断电话
		    GSM_Send_CN_MSG("8615990472896","你好！Hellow World!");//测试发送一条中英文混合的PDU短信
 	    }

	 }//while(1) 
}

#endif //GSM_MODULE == DEF_ENABLED
































