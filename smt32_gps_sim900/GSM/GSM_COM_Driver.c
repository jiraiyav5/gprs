/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						  -��ͨ����ԭ������-          		          						                        �~
�~ ���ߣ�Ī����																					                                          �~
�~   QQ: 1695627698 																			                                        �~
�~ ������һ��ͨ���ӿƼ�һ 																		                                    �~
�~ ���䣺mojianlin2011@126.com																	                                  �~
�~ �Ա�ר���꣺etmcu.taobao.com																	                                  �~
�~ ����:�˳�����Ϊ��ͨ���߼����������д�ģ�����������ҵĿ�ģ���Ҫ�����븽���ҵ���ϵ��ʽ��		    �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/


#include <includes.h>

#if (GSM_MODULE == DEF_ENABLED)
/*��������������������������������������������������*/
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						GSM��������         		          								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/


void  GSM_InitTarget(void)
{

    GPIO_InitTypeDef        gpio_init;
    USART_InitTypeDef       usart_init;
    USART_ClockInitTypeDef  usart_clk_init;
																 
    usart_init.USART_BaudRate            = GSM_COMM_Speed;				 //���ô��ڲ�����
    usart_init.USART_WordLength          = USART_WordLength_8b;			 //8λ����
    usart_init.USART_StopBits            = USART_StopBits_1;			// //1λֹͣλ
    usart_init.USART_Parity              = USART_Parity_No ;			 //��У��
    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //û����������
    usart_init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  //���ա��ɷ���ȫ˫��ģʽ��


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

    BSP_IntVectSet(BSP_INT_ID_USART2, GSM_RxTxISRHandler);//���ô����жϺ���Ϊ�� GSM_RxTxISRHandler()
    BSP_IntEn(BSP_INT_ID_USART2);  //ʹ�ܴ����ж�
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
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    							��ֹGSM���ڽ����ж�    		          							  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/

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
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						����GSM���ڽ����ж�      		       								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
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
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    							GSM_RxISRHandler (void)          								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
void  GSM_RxISRHandler (void)
{
}
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    						GSM_RxTxISRHandler (void)      								          �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
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
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����
    if(USART_GetFlagStatus(usart,USART_FLAG_ORE)==SET)
     {
      USART_ClearFlag(usart,USART_FLAG_ORE); //��SR��ʵ���������־
      USART_ReceiveData(usart); //��DR 
     } 

    if (USART_GetITStatus(usart, USART_IT_RXNE) != RESET) 
	{
        rx_data = USART_ReceiveData(usart) & 0xFF; 
             
        GSM_RxHandler(rx_data);
	   //�п������������û��ִ���꣬�����ֵ������ͻ����USART_FLAG_ORE�ж�
    	
		USART_ClearITPendingBit(usart, USART_IT_RXNE);
	}

    if (USART_GetITStatus(usart, USART_IT_TXE) != RESET)
	{
													   //�����Ϊ�˱���STM32 USART ��һ���ֽڷ�����ȥ��BUG 	
		USART_ITConfig(usart, USART_IT_TXE, DISABLE); //��ֹ�����������жϣ� 
        USART_ClearITPendingBit(usart, USART_IT_TXE);
	}
    

    OSIntExit();                                                
}
/*
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    							ͨ��GSM���ڷ���һ���ֽ�          								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
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
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    							��ֹGSM���ڷ����ж�             								  �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
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
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~
�~    							����GSM���ڷ����ж�          								      �~
�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~�~*/
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

/*��������������������������������������������������*/
#endif
