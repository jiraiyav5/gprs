/*
*********************************************************************************************************
*                                               uC/OS-View
*
*                                 (c) Copyright 2005, Micrium, Weston, FL
*                                           All Rights Reserved
*
*                                            ST STM32 (Cortex-M3)
*                                             IAR C Compiler
*
*
* Filename   : OS_VIEWc.C
* Version    : V1.20
* Programmer : Jean J. Labrosse
*********************************************************************************************************
*/

#include <includes.h>
//extern char app_buf[10] ;
extern com_app_t com_app;
char com_rev_tmp[3] = {'\0',};
CPU_INT08U com_buf_cnt = 0;

#if OS_VIEW_MODULE == DEF_ENABLED

/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define  BIT0      0x01
#define  BIT1      0x02
#define  BIT2      0x04
#define  BIT3      0x08
#define  BIT4      0x10
#define  BIT5      0x20
#define  BIT6      0x40
#define  BIT7      0x80

static void  OSView_RxTxISRHandler (void);

/*$PAGE*/
/*
*********************************************************************************************************
*                                           EXIT uC/OS-View
*
* Description: This function is called if your target needs to 'uninstall' uC/OS-View.
*
* Note(s)    :
*********************************************************************************************************
*/

void  OSView_Exit (void)
{
}

/*
*********************************************************************************************************
*                                           Obtain CPU name
*********************************************************************************************************
*/

void  OSView_GetCPUName (INT8U *s)
{
    INT8U  cpu_clk_freq;


    cpu_clk_freq = (INT8U)(BSP_CPU_ClkFreq() / 1000000L);
    strcpy((char *)s, "ST STM32 (Cortex-M3) (xx MHz)");
    s[22] = cpu_clk_freq / 10 + '0';
    s[23] = cpu_clk_freq % 10 + '0';
}

/*
*********************************************************************************************************
*                                  Obtain Interrupt Stack information
*********************************************************************************************************
*/

INT32U  OSView_GetIntStkBase (void)
{
    return (0);                                       /* We are not using an ISR stack                 */
}


INT32U  OSView_GetIntStkSize (void)
{
    return (0);                                       /* We are not using an ISR stack                 */
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                    INITIALISE uC/OS-View COM PORT
*
* Description: Initialize the hardware required for the OS to run. This will work on any target hardware,
*              but may have to be tailored a little (regarding the clock frequency). Of course the same
*              holds true if for some reason you choose to use another timer.
*
* Note(s)    : 1) This function assumes that a free running timer has been initialized.  The timer can
*                 either be a 16 bits or 32 bits timer.  Your application needs to provide a function
*                 called OSView_TmrRd() that reads the current counts of this timer.  The free running
*                 timer is initialized by the BSP function OSView_TmrInit().
*********************************************************************************************************
*/

void  OSView_InitTarget (INT32U baud_rate)
{

    GPIO_InitTypeDef        gpio_init;
    USART_InitTypeDef       usart_init;
    USART_ClockInitTypeDef  usart_clk_init;

	OSView_TmrInit();                                    /* Initialize the free running timer          */


                                                                /* ----------------- INIT USART STRUCT ---------------- */
    usart_init.USART_BaudRate            = baud_rate;
    usart_init.USART_WordLength          = USART_WordLength_8b;
    usart_init.USART_StopBits            = USART_StopBits_1;
    usart_init.USART_Parity              = USART_Parity_No ;
    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;


    usart_clk_init.USART_Clock           = USART_Clock_Disable;
    usart_clk_init.USART_CPOL            = USART_CPOL_Low;
    usart_clk_init.USART_CPHA            = USART_CPHA_2Edge;
    usart_clk_init.USART_LastBit         = USART_LastBit_Disable;


#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_1)
    BSP_PeriphEn(BSP_PERIPH_ID_USART1);

                                                                /* ----------------- SETUP USART1 GPIO ---------------- */
    BSP_PeriphEn(BSP_PERIPH_ID_IOPA);

                                                                /* Configure GPIOA.9 as push-pull                       */
    gpio_init.GPIO_Pin   = GPIO_Pin_9;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio_init);

                                                                /* Configure GPIOA.10 as input floating                 */
    gpio_init.GPIO_Pin   = GPIO_Pin_10;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio_init);

                                                                /* ------------------ SETUP USART1 -------------------- */
    USART_Init(USART1, &usart_init);
    USART_ClockInit(USART1, &usart_clk_init);
    USART_Cmd(USART1, ENABLE);

    BSP_IntVectSet(BSP_INT_ID_USART1, OSView_RxTxISRHandler);
    BSP_IntEn(BSP_INT_ID_USART1);
#endif

#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_2)
    BSP_PeriphEn(BSP_PERIPH_ID_USART2);

                                                                /* ----------------- SETUP USART2 GPIO ---------------- */
    BSP_PeriphEn(BSP_PERIPH_ID_IOPA);

                                                                /* Configure GPIOA.2 as push-pull                       */
    gpio_init.GPIO_Pin   = GPIO_Pin_2;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio_init);

                                                                /* Configure GPIOA.3 as input floating                  */
    gpio_init.GPIO_Pin   = GPIO_Pin_3;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio_init);

                                                                /* ------------------ SETUP USART2 -------------------- */
    USART_Init(USART2, &usart_init);
    USART_ClockInit(USART2, &usart_clk_init);
    USART_Cmd(USART2, ENABLE);

    BSP_IntVectSet(BSP_INT_ID_USART2, OSView_RxTxISRHandler);
    BSP_IntEn(BSP_INT_ID_USART2);
#endif

#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_3)
    BSP_PeriphEn(BSP_PERIPH_ID_USART3);

                                                                /* ----------------- SETUP USART3 GPIO ---------------- */
    BSP_PeriphEn(BSP_PERIPH_ID_IOPB);

                                                                /* Configure GPIOB.10 as push-pull                      */
    gpio_init.GPIO_Pin   = GPIO_Pin_10;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &gpio_init);

                                                                /* Configure GPIOB.11 as input floating                 */
    gpio_init.GPIO_Pin   = GPIO_Pin_11;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &gpio_init);

                                                                /* ------------------ SETUP USART3 -------------------- */
    USART_Init(USART3, &usart_init);
    USART_ClockInit(USART3, &usart_clk_init);
    USART_Cmd(USART3, ENABLE);

    BSP_IntVectSet(BSP_INT_ID_USART3, OSView_RxTxISRHandler);
    BSP_IntEn(BSP_INT_ID_USART3);
#endif
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                       Disable Rx Interrupts
*********************************************************************************************************
*/

void  OSView_RxIntDis (void)
{
#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_1)
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
#endif

#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_2)
    USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
#endif

#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_3)
    USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
#endif
}

/*
*********************************************************************************************************
*                                       Enable Rx Interrupts
*********************************************************************************************************
*/

void  OSView_RxIntEn (void)
{
#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_1)
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif

#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_2)
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
#endif

#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_3)
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
#endif
}

/*
*********************************************************************************************************
*                                 Rx Communication handler for uC/OS-View
*
* Description: This function is called by OSView_RxISR (see OS_VIEWa.ASM) to process a received
*              character interrupt.
*
* Note(s)    : This adaptation of uC/OS-View assumes that a 'combined' interrupt is generated by the UART
*              and thus this function is not needed.
*********************************************************************************************************
*/

void  OSView_RxISRHandler (void)
{
}

/*
*********************************************************************************************************
*                                Rx/Tx Communication handler for uC/OS-View
*
* Description: This function is NOT called because the M16C has a separate Rx and Tx ISR.
*********************************************************************************************************
*/

static void  OSView_RxTxISRHandler (void)
{

    USART_TypeDef  *usart;
    CPU_INT08U      rx_data;
    CPU_SR          cpu_sr;


    CPU_CRITICAL_ENTER();                                       /* Tell uC/OS-II that we are starting an ISR            */
    OSIntNesting++;
    CPU_CRITICAL_EXIT();


#if   (OS_VIEW_COMM_SEL == OS_VIEW_UART_1)
    usart = USART1;
#elif (OS_VIEW_COMM_SEL == OS_VIEW_UART_2)
    usart = USART2;
#elif (OS_VIEW_COMM_SEL == OS_VIEW_UART_3)
    usart = USART3;
#else
    OSIntExit();
    return;
#endif

    if (USART_GetITStatus(usart, USART_IT_RXNE) != RESET) 
	{
        rx_data = USART_ReceiveData(usart) & 0xFF;              /* Read one byte from the receive data register         */
        USART_SendData( USART1, rx_data);
        ////////////////////////////////////////////
        #if 1        
        if(com_app.com_cnt == 10) {
            com_app.buf_ok  =com_app.buf_ok | (1<<com_buf_cnt);
            com_buf_cnt++;
            if(com_buf_cnt == 3)  com_buf_cnt = 0;
            com_app.com_cnt = 0;                
        }
       
        sprintf(com_rev_tmp,"%x",rx_data);
        strcat(com_app.app_buf[com_buf_cnt], com_rev_tmp);
    
        
        com_app.com_cnt ++;
        #endif
        ////////////////////////////////////////////
        OSView_RxHandler(rx_data);

        USART_ClearITPendingBit(usart, USART_IT_RXNE);          /* Clear the USART1 Receive interrupt                   */
    }

    if (USART_GetITStatus(usart, USART_IT_TXE) != RESET) 
	{
        OSView_TxHandler();

        USART_ClearITPendingBit(usart, USART_IT_TXE);           /* Clear the USART1 transmit interrupt                  */
    }

    OSIntExit();                                                /* Tell uC/OS-II that we are leaving the ISR            */
}

/*
*********************************************************************************************************
*                                      Communication for uC/OS-View
*
* Description: Send 1 character to COM Port
*********************************************************************************************************
*/

void  OSView_Tx1 (INT8U c)
{
#if OS_VIEW_COMM_SEL == OS_VIEW_UART_1
	USART_SendData(USART1, c);
#endif

#if OS_VIEW_COMM_SEL == OS_VIEW_UART_2
	USART_SendData(USART2, c);
#endif

#if OS_VIEW_COMM_SEL == OS_VIEW_UART_3
	USART_SendData(USART3, c);
#endif
}

/*
*********************************************************************************************************
*                                       Disable Tx Interrupts
*********************************************************************************************************
*/

void  OSView_TxIntDis (void)
{
#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_1)
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
#endif

#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_2)
    USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
#endif

#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_3)
    USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
#endif
}

/*
*********************************************************************************************************
*                                       Enable Tx Interrupts
*********************************************************************************************************
*/

void  OSView_TxIntEn (void)
{
#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_1)
    USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
#endif

#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_2)
    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
#endif

#if (OS_VIEW_COMM_SEL == OS_VIEW_UART_3)
    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
#endif
}

#endif

