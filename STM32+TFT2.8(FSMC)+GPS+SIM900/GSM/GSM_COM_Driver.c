/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						  -易通电子原创代码-          		          						                        ▇
▇ 作者：莫建林																					                                          ▇
▇   QQ: 1695627698 																			                                        ▇
▇ 旺旺：一易通电子科技一 																		                                    ▇
▇ 邮箱：mojianlin2011@126.com																	                                  ▇
▇ 淘宝专卖店：etmcu.taobao.com																	                                  ▇
▇ 声明:此程序是为易通无线技术开发板而写的，请勿用于商业目的，若要引用请附上我的联系方式！		    ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/


#include <includes.h>

#if (GSM_MODULE == DEF_ENABLED)
/*★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★*/
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						GSM串口配置         		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/


void  GSM_InitTarget(void)
{

    GPIO_InitTypeDef        gpio_init;
    USART_InitTypeDef       usart_init;
    USART_ClockInitTypeDef  usart_clk_init;
																 
    usart_init.USART_BaudRate            = GSM_COMM_Speed;				 //设置串口波特率
    usart_init.USART_WordLength          = USART_WordLength_8b;			 //8位数据
    usart_init.USART_StopBits            = USART_StopBits_1;			// //1位停止位
    usart_init.USART_Parity              = USART_Parity_No ;			 //无校验
    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //没有流量控制
    usart_init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //可收、可发【全双工模式】


    usart_clk_init.USART_Clock           = USART_Clock_Disable;
    usart_clk_init.USART_CPOL            = USART_CPOL_Low;
    usart_clk_init.USART_CPHA            = USART_CPHA_2Edge;
    usart_clk_init.USART_LastBit         = USART_LastBit_Disable;


#if (GSM_COMM_SEL == GSM_UART_1)
    BSP_PeriphEn(BSP_PERIPH_ID_USART1);
                                                           
    BSP_PeriphEn(BSP_PERIPH_ID_IOPA);
                                                         
    gpio_init.GPIO_Pin   = GPIO_Pin_9;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio_init);

                                                                
    gpio_init.GPIO_Pin   = GPIO_Pin_10;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio_init);

                                                                
    USART_Init(USART1, &usart_init);
    USART_ClockInit(USART1, &usart_clk_init);
    USART_Cmd(USART1, ENABLE);

    BSP_IntVectSet(BSP_INT_ID_USART1, GSM_RxTxISRHandler);
    BSP_IntEn(BSP_INT_ID_USART1);
#endif

#if (GSM_COMM_SEL == GSM_UART_2)
    BSP_PeriphEn(BSP_PERIPH_ID_USART2);
                                                          
    BSP_PeriphEn(BSP_PERIPH_ID_IOPA);
                                                            
    gpio_init.GPIO_Pin   = GPIO_Pin_2;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio_init);

                                                                
    gpio_init.GPIO_Pin   = GPIO_Pin_3;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio_init);

                                                                
    USART_Init(USART2, &usart_init);
    USART_ClockInit(USART2, &usart_clk_init);
    USART_Cmd(USART2, ENABLE);

    BSP_IntVectSet(BSP_INT_ID_USART2, GSM_RxTxISRHandler);//设置串口中断函数为： GSM_RxTxISRHandler()
    BSP_IntEn(BSP_INT_ID_USART2);  //使能串口中断
#endif

#if (GSM_COMM_SEL == GSM_UART_3)
    BSP_PeriphEn(BSP_PERIPH_ID_USART3);

                                                                
    BSP_PeriphEn(BSP_PERIPH_ID_IOPB);

                                                                
    gpio_init.GPIO_Pin   = GPIO_Pin_10;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &gpio_init);

                                                                
    gpio_init.GPIO_Pin   = GPIO_Pin_11;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &gpio_init);

                                                                
    USART_Init(USART3, &usart_init);
    USART_ClockInit(USART3, &usart_clk_init);
    USART_Cmd(USART3, ENABLE);

    BSP_IntVectSet(BSP_INT_ID_USART3, GSM_RxTxISRHandler);
    BSP_IntEn(BSP_INT_ID_USART3);
#endif
}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    							禁止GSM串口接收中断    		          							  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

void  GSM_RxIntDis (void)
{
#if (GSM_COMM_SEL == GSM_UART_1)
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
#endif

#if (GSM_COMM_SEL == GSM_UART_2)
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
#endif

#if (GSM_COMM_SEL == GSM_UART_3)
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
#endif
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						开启GSM串口接收中断      		       								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GSM_RxIntEn (void)
{
#if (GSM_COMM_SEL == GSM_UART_1)
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif

#if (GSM_COMM_SEL == GSM_UART_2)
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
#endif

#if (GSM_COMM_SEL == GSM_UART_3)
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
#endif
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    							GSM_RxISRHandler (void)          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GSM_RxISRHandler (void)
{
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						GSM_RxTxISRHandler (void)      								          ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GSM_RxTxISRHandler(void)
{

    USART_TypeDef  *usart;
    CPU_INT08U      rx_data;
    CPU_SR          cpu_sr;


    CPU_CRITICAL_ENTER(); 
    OSIntNesting++;
    CPU_CRITICAL_EXIT();


#if   (GSM_COMM_SEL == GSM_UART_1)
    usart = USART1;
#elif (GSM_COMM_SEL == GSM_UART_2)
    usart = USART2;
#elif (GSM_COMM_SEL == GSM_UART_3)
    usart = USART3;
#else
    OSIntExit();
    return;
#endif
	//溢出-如果发生溢出需要先读SR,再读DR寄存器则可清除不断入中断的问题
    if(USART_GetFlagStatus(usart,USART_FLAG_ORE)==SET)
     {
      USART_ClearFlag(usart,USART_FLAG_ORE); //读SR其实就是清除标志
      USART_ReceiveData(usart); //读DR 
     } 

    if (USART_GetITStatus(usart, USART_IT_RXNE) != RESET) 
	{
        rx_data = USART_ReceiveData(usart) & 0xFF; 
             
        GSM_RxHandler(rx_data);
	   //有可能这个函数还没有执行完，数据又到来，就会产生USART_FLAG_ORE中断
    	
		USART_ClearITPendingBit(usart, USART_IT_RXNE);
	}

    if (USART_GetITStatus(usart, USART_IT_TXE) != RESET)
	{
													   //这段是为了避免STM32 USART 第一个字节发不出去的BUG 	
		USART_ITConfig(usart, USART_IT_TXE, DISABLE); //禁止发缓冲器空中断， 
        USART_ClearITPendingBit(usart, USART_IT_TXE);
	}
    

    OSIntExit();                                                
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    							通过GSM串口发送一个字节          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GSM_TxChar(INT8U c)
{
#if GSM_COMM_SEL == GSM_UART_1
	USART_SendData(USART1, c);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
#endif

#if GSM_COMM_SEL == GSM_UART_2
	USART_SendData(USART2, c);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
#endif

#if GSM_COMM_SEL == GSM_UART_3
	USART_SendData(USART3, c);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET);
#endif
}

void  GSM_TxString(INT8U *str)
{

   while(*str!='\0')
    {	
	   GSM_TxChar(*str++);
    }
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    							禁止GSM串口发送中断             								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GSM_TxIntDis (void)
{
#if (GSM_COMM_SEL == GSM_UART_1)
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
#endif

#if (GSM_COMM_SEL == GSM_UART_2)
    USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
#endif

#if (GSM_COMM_SEL == GSM_UART_3)
    USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
#endif
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    							开启GSM串口发送中断          								      ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
	void  GSM_TxIntEn (void)
	{
	#if (GSM_COMM_SEL == GSM_UART_1)
	    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	#endif					
	
	#if (GSM_COMM_SEL == GSM_UART_2)
	    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	#endif
	
	#if (GSM_COMM_SEL == GSM_UART_3)
	    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	#endif
	}

/*★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★*/
#endif
