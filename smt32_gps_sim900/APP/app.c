
#include <includes.h>

com_app_t com_app;
//char app_buf[10] = {'\0',};

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						����ֲ�����        		          								                              �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
static  OS_STK         App_TaskStartStk[APP_TASK_START_STK_SIZE];

#if (OS_VIEW_MODULE == DEF_ENABLED)
OS_STK     TerminalTaskStk[APP_TASK_TERMINAL_STK_SIZE];
void	     TerminalTask(void *pdata);
void       AppTerminalRx(CPU_INT08U   rx_data);
CPU_INT08U AppTerminalRxMboxData = 0;
OS_EVENT  *AppTerminalRxMbox;
#endif

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    							����Ԥ����      		          								                                �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/

static  void  App_TaskCreate(void);
static  void  App_TaskStart	(void	*p_arg);

/*
���������������������������������������������������
��    ��������  main ()									          								                                ��
��	  ����  :   main()�������������̵���ں���											 	                            ��
���������������������������������������������������*/
INT32S main (void)
{
    CPU_INT08U  os_err;
//	temp[0]=temp[1];
	  BSP_Init(); 
	
    BSP_IntDisAll();  /*��ֹCUP��Ӧ�����ж�*/

    OSInit(); /* uC/OS-II��ʼ��*/

    os_err = OSTaskCreateExt((void (*)(void *)) App_TaskStart,  								//point to the task of poiter
                             (void          * ) 0,															//send a parameter to task
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],			//point to the top of task
                             (INT8U           ) APP_TASK_START_PRIO,										//��������ȼ���
                             (INT16U          ) APP_TASK_START_PRIO,										//����ı�ʶ
                             (OS_STK        * )&App_TaskStartStk[0],										//�����ջջ�׵�ָ��
                             (INT32U          ) APP_TASK_START_STK_SIZE,									//���������
                             (void          * )0,															//ָ�򸽼��������ָ��
                             (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));			//�������ò����趨ѡ��

#if OS_TASK_NAME_EN > 0
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif

    OSStart(); 

    return (0);
}
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ��������  App_TaskStart()																	                                  �~
�~	  ����  :   uCOS�Ŀ�ʼ����		 													 	                                    �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
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
    GPS_RxIntEn();    	/*GPS���ڽ����жϿ���*/
#endif

#if (GSM_MODULE == DEF_ENABLED)
    GSM_Init();                                         		
    GSM_RxIntEn();    	/*GSM���ڽ����жϿ���*/
#endif

#if (OS_VIEW_MODULE == DEF_ENABLED)
    OSView_Init(OS_VIEW_COMM_Speed);         
    OSView_TerminalRxSetCallback(AppTerminalRx);
    OSView_RxIntEn();  	/*OSView���ڽ����жϿ���*/                                        
	AppTerminalRxMbox = OSMboxCreate((void *)0);
#endif

   App_TaskCreate();    /*�������������*/
	 //LCD_SHOW_CN15x16_String(0,200,"��ͨ���ӿƼ�");
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
     
	 IWDG_ReloadCounter();//ι��
   }
}

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ��������  App_TaskCreate()																                                  �~
�~	  ����  :   ���ڴ������������	 													 	                                  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
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
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ������  ��  AppTerminalRx(rx_data)											     		                            �~			  
�~	  ���������  ���յ����ֽ�����  							  							                                �~
�~	  ����    :   uC/OS-View ���Ӻ���	 													 	                                �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/

#if (OS_VIEW_MODULE == DEF_ENABLED)
void  AppTerminalRx(CPU_INT08U rx_data)
{
	AppTerminalRxMboxData = rx_data; 
	OSMboxPost(AppTerminalRxMbox, &AppTerminalRxMboxData);
}
#endif

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~						            uC/OS-II APP HOOKS											                                �~
�~																								                                                �~
�~    HOOK�������������Ӻ���������HOOK������Windows��Ϣ������Ƶ�һ���֣�ͨ�����á����ӡ���		      �~
�~    Ӧ�ó��������ϵͳ����������Ϣ���¼����й��ˣ�����������������޷����ʵ���Ϣ����Ȼ��		    �~
�~    ��ô��Ҳ����Ҫ����һ���Ĵ��۵ġ����ڶ�����ôһ��������̣�ϵͳ���ܻ��ܵ�һ����Ӱ�죬		    �~
�~    ���Դ���ڱ�Ҫ��ʱ���ʹ�á����ӡ�������ʹ����ϼ�ʱ����ɾ����							                  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/

#if (OS_APP_HOOKS_EN > 0)
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ������  ��  App_TaskCreateHook(OS_TCB *ptcb)									     		                      �~			  
�~	  ���������  ptcb   is a pointer to the task control block of the task being created.	      �~
�~	  ����    :   This function is called when a task is created.							 	                  �~
�~	  ע��    ��  ����ʱ�ر������ж�															                                �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
	void  App_TaskCreateHook (OS_TCB *ptcb)
	{
	#if (OS_VIEW_MODULE == DEF_ENABLED)
	    OSView_TaskCreateHook(ptcb);
	#endif
	}

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ������  ��  App_TaskDelHook (OS_TCB *ptcb)									     		                        �~			  
�~	  ���������  ptcb   is a pointer to the task control block of the task being created.	      �~
�~	  ����    :   This function is called when a task is deleted.							 	                  �~
�~	  ע��    ��  ����ʱ�ر������ж�															                                �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/

	void  App_TaskDelHook (OS_TCB *ptcb)
	{
	    (void)ptcb;
	}

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ������  ��  App_TaskIdleHook (void)      									     		                          �~			  
�~	  ����    ��  ptcb   is a pointer to the task control block of the task being created.	      �~
�~	              This function is called by OSTaskIdleHook(), which is called by the idle task.  �~
�~                This hook has been added to allow you to do such things as STOP the CPU to	    �~
�~				        conserve power.				 	  								                                      �~
�~	  ע��    ��  ����ʱ�ر������ж�															                                �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/

	#if OS_VERSION >= 251
	void  App_TaskIdleHook (void)
	{
	
	}
	#endif

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ������  ��  App_TaskStatHook (void)      									     		                          �~			  
�~	  ����    ��  This function is called by OSTaskStatHook(), which is called every second 	    �~
�~                by uC/OS-II's	statistics task.  This allows your application to add 		        �~
�~				  functionality to the statistics task.                                                 �~
�~	  ע��    ��  ����ʱ�ر������ж�															                                �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/

void  App_TaskStatHook (void)
{
}

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ������  ��  App_TaskSwHook (void)     									     		                            �~			  
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/

	#if OS_TASK_SW_HOOK_EN > 0
	void  App_TaskSwHook (void)
	{
		#if (OS_VIEW_MODULE == DEF_ENABLED)
		    OSView_TaskSwHook();
		#endif
	}
	#endif

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ������  ��  App_TCBInitHook (OS_TCB *ptcb)    									     	                      �~			  
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/

	#if OS_VERSION >= 204
	void  App_TCBInitHook (OS_TCB *ptcb)
	{
	    (void)ptcb;
	}
	#endif

/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ������  ��  App_TimeTickHook (void)           									     	                      �~			  
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/

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
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ������  ��  TerminalTask(void *pdata)           									     	                    �~
�~	  ����    ��  OSView����		  															                                  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
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
                 sprintf((char *)s, "\nCPUʹ���� = %3u%%\n", OSCPUUsage);
                 OSView_TxStr(s, 1);
                 break;

            case '2':
                 sprintf((char *)s, "\n��������  = %3u\n", OSTaskCtr);
                 OSView_TxStr(s, 1);
                 break;

            default:
                 OSView_TxStr("\n\nMicrium, Inc.",       1);
                 OSView_TxStr("\n1: CPUʹ���� (%)",      1);
                 OSView_TxStr("\n2: �鿴��������",       1);
                 OSView_TxStr("\n?: ������Ϣ\n",         1);
                 break;
        }
    }
}
#endif
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    ������  ��  assert_failed(u8* file, u32 line)           							     	                �~
�~    ����    ��  Reports the name of the source file and the source line number where the 		    �~
�~	              assert_param error has occurred.												                        �~
�~	  ����    : - file: pointer to the source file name 										                      �~
�~              - line: assert_param error line source number			  		  				                �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
#ifdef DEBUG
void assert_failed(u8* file, u32 line) 
{ 
	printf("Wrong parameters value: file %s on line %d\r\n", file, line); 
	while (1) { } 
}
 #endif

