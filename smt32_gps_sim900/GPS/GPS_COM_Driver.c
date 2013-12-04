#include <includes.h>

#if (GPS_MODULE == DEF_ENABLED)
/*★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★*/
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						GPS串口配置         		          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GPS_InitTarget ()
{

    GPIO_InitTypeDef        gpio_init;
    USART_InitTypeDef       usart_init;
    USART_ClockInitTypeDef  usart_clk_init;
                                                                
    usart_init.USART_BaudRate            = GPS_COMM_Speed;
    usart_init.USART_WordLength          = USART_WordLength_8b;
    usart_init.USART_StopBits            = USART_StopBits_1;
    usart_init.USART_Parity              = USART_Parity_No ;
    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;


    usart_clk_init.USART_Clock           = USART_Clock_Disable;
    usart_clk_init.USART_CPOL            = USART_CPOL_Low;
    usart_clk_init.USART_CPHA            = USART_CPHA_2Edge;
    usart_clk_init.USART_LastBit         = USART_LastBit_Disable;


#if (GPS_COMM_SEL == GPS_UART_1)
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

    BSP_IntVectSet(BSP_INT_ID_USART1, GPS_RxTxISRHandler);
    BSP_IntEn(BSP_INT_ID_USART1);
#endif

#if (GPS_COMM_SEL == GPS_UART_2)
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

    BSP_IntVectSet(BSP_INT_ID_USART2, GPS_RxTxISRHandler);
    BSP_IntEn(BSP_INT_ID_USART2);
#endif

#if (GPS_COMM_SEL == GPS_UART_3)
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

    BSP_IntVectSet(BSP_INT_ID_USART3, GPS_RxTxISRHandler);
    BSP_IntEn(BSP_INT_ID_USART3);
#endif
}

/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    							禁止GPS串口接收中断    		          							  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/

void  GPS_RxIntDis (void)
{
#if (GPS_COMM_SEL == GPS_UART_1)
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
#endif

#if (GPS_COMM_SEL == GPS_UART_2)
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
#endif

#if (GPS_COMM_SEL == GPS_UART_3)
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
#endif
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						开启GPS串口接收中断      		       								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GPS_RxIntEn (void)
{
#if (GPS_COMM_SEL == GPS_UART_1)
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif

#if (GPS_COMM_SEL == GPS_UART_2)
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
#endif

#if (GPS_COMM_SEL == GPS_UART_3)
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
#endif
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    							GPS_RxISRHandler (void)          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GPS_RxISRHandler (void)
{
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    						GPS_RxTxISRHandler (void)      								          ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GPS_RxTxISRHandler (void)
{

    USART_TypeDef  *usart;
    CPU_INT08U      rx_data;
    CPU_SR          cpu_sr;


    CPU_CRITICAL_ENTER();                                      
    OSIntNesting++;
    CPU_CRITICAL_EXIT();


#if   (GPS_COMM_SEL == GPS_UART_1)
    usart = USART1;
#elif (GPS_COMM_SEL == GPS_UART_2)
    usart = USART2;
#elif (GPS_COMM_SEL == GPS_UART_3)
    usart = USART3;
#else
    OSIntExit();
    return;
#endif

    if (USART_GetITStatus(usart, USART_IT_RXNE) != RESET) 
	{     //接收字节数据
        rx_data = USART_ReceiveData(usart) & 0xFF;              
        GPS_RxHandler(rx_data);

        USART_ClearITPendingBit(usart, USART_IT_RXNE);          
    }

    if (USART_GetITStatus(usart, USART_IT_TXE) != RESET) 
	{      //发送中断
        GPS_TxHandler();
        USART_ClearITPendingBit(usart, USART_IT_TXE);           
    }

	//溢出-如果发生溢出需要先读SR,再读DR寄存器则可清除不断入中断的问题
    if(USART_GetFlagStatus(usart,USART_FLAG_ORE)==SET)
     {
      USART_ClearFlag(usart,USART_FLAG_ORE); //读SR其实就是清除标志
      USART_ReceiveData(usart); //读DR 
     }


    OSIntExit();                                                
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    							通过GPS串口发送一个字节          								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GPS_Tx1 (INT8U c)
{
#if GPS_COMM_SEL == GPS_UART_1
	USART_SendData(USART1, c);
#endif

#if GPS_COMM_SEL == GPS_UART_2
	USART_SendData(USART2, c);
#endif

#if GPS_COMM_SEL == GPS_UART_3
	USART_SendData(USART3, c);
#endif
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    							禁止GPS串口发送中断             								  ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
void  GPS_TxIntDis (void)
{
#if (GPS_COMM_SEL == GPS_UART_1)
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
#endif

#if (GPS_COMM_SEL == GPS_UART_2)
    USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
#endif

#if (GPS_COMM_SEL == GPS_UART_3)
    USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
#endif
}
/*
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇
▇    							开启GPS串口发送中断          								      ▇
▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇*/
	void  GPS_TxIntEn (void)
	{
	#if (GPS_COMM_SEL == GPS_UART_1)
	    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	#endif
	
	#if (GPS_COMM_SEL == GPS_UART_2)
	    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	#endif
	
	#if (GPS_COMM_SEL == GPS_UART_3)
	    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	#endif
	}
/*★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★*/
#endif
