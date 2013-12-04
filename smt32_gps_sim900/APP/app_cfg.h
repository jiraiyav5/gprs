
#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

#pragma import(__use_c99_library)


/*
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※                                     功能模块开关
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/

#define  OS_VIEW_MODULE        DEF_DISABLED	     	
#define  GPS_MODULE						 DEF_ENABLED
#define  GSM_MODULE						 DEF_ENABLED			
#define  DEBUG_MODULE					 DEF_DISABLED		
#define	 TFT_MODULE						 DEF_ENABLED

#define	 APP_USE_CNLib 				 	 DEF_DISABLED	  //汉字字库编译开关

/*
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※                                     任务优先级配置
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/
#define  APP_TASK_START_PRIO                       3
#define  GPS_TASK_PRIO               				       4
#define  GSM_TASK_PRIO               				       5


#define  APP_TASK_TERMINAL_PRIO							  11

#define  OS_VIEW_TASK_PRIO               (OS_LOWEST_PRIO - 3)
#define  OS_VIEW_TASK_ID 				         (OS_LOWEST_PRIO - 3)
#define  OS_TASK_TMR_PRIO                (OS_LOWEST_PRIO - 2)

/*
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※                                     任务堆栈大小配置
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※*/

#define  APP_TASK_START_STK_SIZE                       512

#define  APP_TASK_TERMINAL_STK_SIZE						         128

#define  OS_VIEW_TASK_STK_SIZE                         128

#define  GPS_TASK_STK_SIZE                           	 128
#define  GSM_TASK_STK_SIZE                           	 128

/*
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※                                        串口分配 
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
*/
#define  DEBUG_UART_1                         	1
#define  DEBUG_UART_2                         	2
#define  DEBUG_UART_3                         	3
#define  DEBUG_COMM_SEL							            DEBUG_UART_1
#define  DEBUG_COMM_Speed                       115200

#define  GSM_UART_1                         	1
#define  GSM_UART_2                         	2
#define  GSM_UART_3                         	3
#define  GSM_COMM_SEL							GSM_UART_2
#define  GSM_COMM_Speed                         115200

#define  GPS_UART_1                         	1
#define  GPS_UART_2                         	2
#define  GPS_UART_3                         	3
#define  GPS_COMM_SEL							GPS_UART_3	    
#define  GPS_COMM_Speed                         9600

#define  OS_VIEW_UART_1                         1
#define  OS_VIEW_UART_2                         2
#define  OS_VIEW_UART_3                         3
#define  OS_VIEW_PARSE_TASK                     DEF_ENABLED           	
#define  OS_VIEW_TIMER_SEL                      2                  
#define  OS_VIEW_COMM_SEL                  		OS_VIEW_UART_1        
#define  OS_VIEW_COMM_Speed                     115200	 
/*
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
※                                                  LIB
※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
*/

#define  uC_CFG_OPTIMIZE_ASM_EN                 DEF_ENABLED
#define  LIB_STR_CFG_FP_EN                      DEF_DISABLED

#endif
