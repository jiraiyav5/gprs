
#include <includes.h>

com_app_t com_app;
//char app_buf[10] = {'\0',};

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						定义局部变量        		          								                              ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
static  OS_STK         App_TaskStartStk[APP_TASK_START_STK_SIZE];

#if (OS_VIEW_MODULE == DEF_ENABLED)
OS_STK     TerminalTaskStk[APP_TASK_TERMINAL_STK_SIZE];
void	     TerminalTask(void *pdata);
void       AppTerminalRx(CPU_INT08U   rx_data);
CPU_INT08U AppTerminalRxMboxData = 0;
OS_EVENT  *AppTerminalRxMbox;
#endif

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    							函数预定义      		          								                                ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

static  void  App_TaskCreate(void);
static  void  App_TaskStart	(void	*p_arg);

/*
★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
★    函数名：  main ()									          								                                ★
★	  功能  :   main()函数是整个工程的入口函数											 	                            ★
★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★*/
INT32S main (void)
{
    CPU_INT08U  os_err;
//	temp[0]=temp[1];
	  BSP_Init(); 
	
    BSP_IntDisAll();  /*禁止CUP响应所有中断*/

    OSInit(); /* uC/OS-II初始化*/

    os_err = OSTaskCreateExt((void (*)(void *)) App_TaskStart,  								//point to the task of poiter
                             (void          * ) 0,															//send a parameter to task
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],			//point to the top of task
                             (INT8U           ) APP_TASK_START_PRIO,										//任务的优先级别
                             (INT16U          ) APP_TASK_START_PRIO,										//任务的标识
                             (OS_STK        * )&App_TaskStartStk[0],										//任务堆栈栈底的指针
                             (INT32U          ) APP_TASK_START_STK_SIZE,									//任务的容量
                             (void          * )0,															//指向附加数据域的指针
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));			//用于设置操作设定选项

#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif

    OSStart(); 

    return (0);
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名：  App_TaskStart()																	                                  ▇
▇	  功能  :   uCOS的开始任务		 													 	                                    ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
static  void  App_TaskStart (void *p_arg)
{ 	
//   unsigned short cn=0;
    
	(void)p_arg;
  
    BSP_Init(); 

    OS_CPU_SysTickInit();
	 
#if (TFT_MODULE	 == DEF_ENABLED)
	TFT_Init();
#endif	

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();        
#endif

#if (GPS_MODULE == DEF_ENABLED)
    GPS_Init();                                         		
    GPS_RxIntEn();    	/*GPS串口接收中断开启*/
#endif

#if (GSM_MODULE == DEF_ENABLED)
    GSM_Init();                                         		
    GSM_RxIntEn();    	/*GSM串口接收中断开启*/
#endif

#if (OS_VIEW_MODULE == DEF_ENABLED)
    OSView_Init(OS_VIEW_COMM_Speed);         
    OSView_TerminalRxSetCallback(AppTerminalRx);
    OSView_RxIntEn();  	/*OSView串口接收中断开启*/                                        
	AppTerminalRxMbox = OSMboxCreate((void *)0);
#endif

   App_TaskCreate();    /*创建更多的任务*/
	 //LCD_SHOW_CN15x16_String(0,200,"易通电子科技");
//	 BSP_BEEP_ON();
   BSP_RLED_ON();
   OSTimeDlyHMSM(0, 0, 0, 500); 
   BSP_RLED_OFF();
 	 BSP_BEEP_OFF(); 
    while (1) 
	 { 
	 
//	 BSP_GLED_ON();
   OSTimeDlyHMSM(0, 0, 0, 500); 

	 BSP_GLED_OFF();
   OSTimeDlyHMSM(0, 0, 0, 500);
     
	 IWDG_ReloadCounter();//喂狗
   }
}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名：  App_TaskCreate()																                                  ▇
▇	  功能  :   用于创建更多的任务	 													 	                                  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
static  void  App_TaskCreate (void)
{
    
#if (OS_VIEW_MODULE == DEF_ENABLED)
	CPU_INT08U  os_err;
    os_err = OSTaskCreateExt((void (*)(void *)) TerminalTask,
                             (void          * ) 0,
                             (OS_STK        * )&TerminalTaskStk[APP_TASK_TERMINAL_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_TERMINAL_PRIO,
                             (INT16U          ) APP_TASK_TERMINAL_PRIO,
                             (OS_STK        * )&TerminalTaskStk[0],
                             (INT32U          ) APP_TASK_TERMINAL_STK_SIZE,
                             (void          * ) 0,
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#if OS_TASK_NAME_EN > 0
    	OSTaskNameSet(APP_TASK_TERMINAL_PRIO, "Terminal Task", &os_err);
#endif
#endif
}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名  ：  AppTerminalRx(rx_data)											     		                            ▇			  
▇	  输入参数：  接收到的字节数据  							  							                                ▇
▇	  功能    :   uC/OS-View 的子函数	 													 	                                ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

#if (OS_VIEW_MODULE == DEF_ENABLED)
void  AppTerminalRx(CPU_INT08U rx_data)
{
	AppTerminalRxMboxData = rx_data; 
	OSMboxPost(AppTerminalRxMbox, &AppTerminalRxMboxData);
}
#endif

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇						            uC/OS-II APP HOOKS											                                ▇
▇																								                                                ▇
▇    HOOK函数又名：钩子函数。钩子HOOK函数是Windows消息处理机制的一部分，通过设置“钩子”，		      ▇
▇    应用程序可以在系统级对所有消息、事件进行过滤，访问在正常情况下无法访问的消息。当然，		    ▇
▇    这么做也是需要付出一定的代价的。由于多了这么一道处理过程，系统性能会受到一定的影响，		    ▇
▇    所以大家在必要的时候才使用“钩子”，并在使用完毕及时将其删除。							                  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

#if (OS_APP_HOOKS_EN > 0)
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名  ：  App_TaskCreateHook(OS_TCB *ptcb)									     		                      ▇			  
▇	  输入参数：  ptcb   is a pointer to the task control block of the task being created.	      ▇
▇	  功能    :   This function is called when a task is created.							 	                  ▇
▇	  注意    ：  调用时关闭所有中断															                                ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
	void  App_TaskCreateHook (OS_TCB *ptcb)
	{
	#if (OS_VIEW_MODULE == DEF_ENABLED)
	    OSView_TaskCreateHook(ptcb);
	#endif
	}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名  ：  App_TaskDelHook (OS_TCB *ptcb)									     		                        ▇			  
▇	  输入参数：  ptcb   is a pointer to the task control block of the task being created.	      ▇
▇	  功能    :   This function is called when a task is deleted.							 	                  ▇
▇	  注意    ：  调用时关闭所有中断															                                ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

	void  App_TaskDelHook (OS_TCB *ptcb)
	{
	    (void)ptcb;
	}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名  ：  App_TaskIdleHook (void)      									     		                          ▇			  
▇	  功能    ：  ptcb   is a pointer to the task control block of the task being created.	      ▇
▇	              This function is called by OSTaskIdleHook(), which is called by the idle task.  ▇
▇                This hook has been added to allow you to do such things as STOP the CPU to	    ▇
▇				        conserve power.				 	  								                                      ▇
▇	  注意    ：  调用时关闭所有中断															                                ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

	#if OS_VERSION >= 251
	void  App_TaskIdleHook (void)
	{
	
	}
	#endif

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名  ：  App_TaskStatHook (void)      									     		                          ▇			  
▇	  功能    ：  This function is called by OSTaskStatHook(), which is called every second 	    ▇
▇                by uC/OS-II's	statistics task.  This allows your application to add 		        ▇
▇				  functionality to the statistics task.                                                 ▇
▇	  注意    ：  调用时关闭所有中断															                                ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

void  App_TaskStatHook (void)
{
}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名  ：  App_TaskSwHook (void)     									     		                            ▇			  
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

	#if OS_TASK_SW_HOOK_EN > 0
	void  App_TaskSwHook (void)
	{
		#if (OS_VIEW_MODULE == DEF_ENABLED)
		    OSView_TaskSwHook();
		#endif
	}
	#endif

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名  ：  App_TCBInitHook (OS_TCB *ptcb)    									     	                      ▇			  
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

	#if OS_VERSION >= 204
	void  App_TCBInitHook (OS_TCB *ptcb)
	{
	    (void)ptcb;
	}
	#endif

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名  ：  App_TimeTickHook (void)           									     	                      ▇			  
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

	#if OS_TIME_TICK_HOOK_EN > 0
		void  App_TimeTickHook (void)
		{
			#if (OS_VIEW_MODULE == DEF_ENABLED)
			    OSView_TickHook();
			#endif
		}
	#endif
#endif

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名  ：  TerminalTask(void *pdata)           									     	                    ▇
▇	  功能    ：  OSView任务		  															                                  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
#if (OS_VIEW_MODULE == DEF_ENABLED)
void  TerminalTask(void *pdata)
{
    INT8U      s[100];
    INT8U      *key;
    INT8U      err;

    (void)pdata;   /* Prevent compiler warning                 */
	                                         
    while (1) 
	{
        key = (INT8U *)OSMboxPend(AppTerminalRxMbox, 0, &err);
        switch (*key) {
            case '1':
                 sprintf((char *)s, "\nCPU使用率 = %3u%%\n", OSCPUUsage);
                 OSView_TxStr(s, 1);
                 break;

            case '2':
                 sprintf((char *)s, "\n总任务数  = %3u\n", OSTaskCtr);
                 OSView_TxStr(s, 1);
                 break;

            default:
                 OSView_TxStr("\n\nMicrium, Inc.",       1);
                 OSView_TxStr("\n1: CPU使用率 (%)",      1);
                 OSView_TxStr("\n2: 查看任务总数",       1);
                 OSView_TxStr("\n?: 帮助信息\n",         1);
                 break;
        }
    }
}
#endif
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    函数名  ：  assert_failed(u8* file, u32 line)           							     	                ▇
▇    描述    ：  Reports the name of the source file and the source line number where the 		    ▇
▇	              assert_param error has occurred.												                        ▇
▇	  参数    : - file: pointer to the source file name 										                      ▇
▇              - line: assert_param error line source number			  		  				                ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
#ifdef DEBUG
void assert_failed(u8* file, u32 line) 
{ 
	printf("Wrong parameters value: file %s on line %d\r\n", file, line); 
	while (1) { } 
}
 #endif

