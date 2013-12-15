

#include "includes.h"

#include "GSM_Processes.h"
#include "GSM_TXD.h"



#if GSM_MODULE == DEF_ENABLED

static   OS_STK      GSM_TaskStk[GSM_TASK_STK_SIZE];
//static   OS_EVENT   *GSM_Sem;

GSM_RX_STATE  GSM_RX_nState;	//串口数据处理状态

INT16U    Rx_Index;	//数据指针
INT8U	  GSM_pCommand[GSM_MAX_CMD_LEN];	//
INT8U     GSM_pData[GSM_MAX_DATA_LEN];	//

INT8U	  RX_RN_JP_NUM=0;

INT16U   Test_counter=0;
INT8U    Flag_CMT=0;
INT8U    Flag_CMGR=0;
INT8U    Flag_DATA=0;

extern INT8U    GPSinf[100];
extern  const unsigned char GPSimage[10];

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
//	GSM_START(); //启动GSM模块
    GSM_InitOS(); 
    GSM_InitTarget();
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  启动GSM模块 		          										  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void GSM_START(void)
{
    GPIO_InitTypeDef  gpio_init;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    gpio_init.GPIO_Pin   = GPIO_Pin_8;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &gpio_init);

	GPIO_SetBits(GPIOA, GPIO_Pin_8);	//给GSM模块的电源控制脚一个下降沿启动模块。
	OSTimeDlyHMSM(0, 0, 4, 0);
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	OSTimeDlyHMSM(0, 0, 1, 0);
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
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
                          (void *)0,                                  	/* No arguments passed to OSView_Task() */
                          &GSM_TaskStk[GSM_TASK_STK_SIZE - 1], 			/* Set Top-Of-Stack                     */
                           GSM_TASK_PRIO,                          		/* Lowest priority level                */
                           GSM_TASK_PRIO,
                          &GSM_TaskStk[0],                         		/* Set Bottom-Of-Stack                  */
                           GSM_TASK_STK_SIZE,
                          (void *)0,                                  	/* No TCB extension                     */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR); 	/* Enable stack checking + clear stack  */

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
//char str1[]={"66666660A84F4D4E8E---FF1A6C5F82CF770182CF5DDE5E0254344E2D533A73B273D18857002073B273D18857548C6C886D528DEF4EA453C98DEF53E3002082CF86795927697C002082CF5DDE5DE54E1A56ED533A661F6E7E5B666821002096448FD130027A0D540E60A85C06653652304E0067615F694FE1FF0C5185670963CF8FF060A85F53524D4F4D"};
//char str2[]={"60A84F4D4E8E"};

//  INT8U      err; k[1], 
	INT16U  i=0;
//	INT8U S[100];
	INT8U  TP=0;

	//TFT_ShowString(0,100,(INT8U *)strstr(str1, str2));
//	TFT_ShowNum(200,280,*(INT8U *)strstr(str1, str2));
//	TFT_ShowNum(200,260,strlen(str2));
	Key_Number_Init();
	//MMC7660_Init();

	while(1) 
	 {
	 //(void)OSSemPend(GSM_Sem, 0, &err); //任务挂起，等待OSSemPost(GSM_Sem);执行后才运行任务一次
	  
	  OSTimeDlyHMSM(0, 0, 0,500);
	  i++;
	  
	  //TFT_ShowHEX(210,300,I2C_ReadByte(MMA7660,3));

	  TP=Get_Temperature();
	  //TFT_ShowNum(100,260,ADC_Result[ADC_TEMP]);
	  TFT_ShowString(0,260,"Temperature=");
	  TFT_ShowNum(100,260,TP);

	  #if (TFT_MODULE == DEF_ENABLED)
	  TFT_ShowNum(0,280,i);
	  TFT_ShowNum(0,300,Test_counter);
	  if(Flag_DATA)
	   { 
		TFT_ShowPicture(0,0,240,320,GPSimage);//显示背景图片
		TFT_ShowString(0,80,GSM_pCommand);
	    TFT_ShowString(0,100,GSM_pData);
	    Flag_DATA=0;
	   }
	  #endif
	 if(CMT.isNew) 
	   {
	   	#if (TFT_MODULE	 == DEF_ENABLED)
		    TFT_ShowString(0,180,CMT.PhoneNum);
			TFT_ShowString(0,200,CMT.Date_Time);
			TFT_ShowString(0,220,CMT.Content);
			TFT_ShowString(0,240,CMT.OP_CDM);
			TFT_ShowString(50,240,(INT8U *)CMT.OP_Key_ASCII);
			TFT_ShowNum  (120,240,CMT.OP_Key);
		#endif
		CMT.isNew=0;
	    if( strcmp((char *)CMT.OP_CDM, "GPS?") == NULL && CMT.OP_Key==My_Key_Number)
	     { 
	      if(GPRMCData.DataValid=='A')
		  {
 		   GSM_Send_TXT_CMG(CMT.PhoneNum,GPSinf);
	   	  }
   	      else GSM_Send_GB_SMS(CMT.PhoneNum,"正在搜索GPS信息，请稍后...");
	     }
	   }
	 }
}







#endif //GSM_MODULE == DEF_ENABLED
































