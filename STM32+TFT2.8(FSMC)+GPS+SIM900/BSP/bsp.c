/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
* Filename      : bsp.c
* Version       : V1.10
* Programmer(s) : Timo
*********************************************************************************************************
*/

#define  BSP_MODULE

#include <includes.h>
#include <bsp.h>

static  void  BSP_IWDG_Init(void);
static  void  BSP_ADC_Init (void);
static  void  ADC_RCVTabISRHandler(void);
static  void  BSP_SPI_Init (void);
void BSP_GPIO_Init(void);
#if DEBUG_MODULE == DEF_ENABLED
static  void  Debug_InitTarget(void);
#endif


INT8U	ADC_CurConvCount = 0;
INT8U	ADC_Complete=0;
INT16U 	ADC_RCVTab    [ADC_RCVTab_SIZE];//接收DMA传过来的ADC转换结果
INT32S 	ADC_ValueMux  [ADC_RCVTab_SIZE];//用于累加
INT32S 	ADC_Result    [ADC_RCVTab_SIZE];//ADC最终结果
/*
*********************************************************************************************************
*         硬件初始化
*********************************************************************************************************
*/
void  BSP_Init (void)
{
	BSP_IntInit();

    RCC_DeInit();  //芯片时钟初始化
    RCC_HSEConfig(RCC_HSE_ON);

	if(RCC_WaitForHSEStartUp() == SUCCESS)
	{
	    RCC_HCLKConfig(RCC_SYSCLK_Div1);

	    RCC_PCLK2Config(RCC_HCLK_Div1);
	    RCC_PCLK1Config(RCC_HCLK_Div2);

	    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	    FLASH_SetLatency(FLASH_Latency_2);
	    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

	    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	    RCC_PLLCmd(ENABLE);
	
	    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	
	    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	
	    while (RCC_GetSYSCLKSource() != 0x08);
	}
   
    FLASH_Unlock();
    EE_Init();	     /* EEPROM Init */
	  BSP_IWDG_Init();   
    BSP_ADC_Init();
    //I2C_Configuration();
	BSP_SPI_Init();
	BSP_GPIO_Init();
#if (DEBUG_MODULE == DEF_ENABLED)
	Debug_InitTarget();
#endif
}


static  void  BSP_IWDG_Init (void)
{
 IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
 IWDG_SetPrescaler(IWDG_Prescaler_32);
 IWDG_SetReload(0X0FFF);
 IWDG_ReloadCounter();
 IWDG_Enable();
}

void BSP_GPIO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

	  /* 使能各端口时钟 */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
	                         RCC_APB2Periph_GPIOB | 
							             RCC_APB2Periph_GPIOC |
	                         RCC_APB2Periph_GPIOD | 
							             RCC_APB2Periph_GPIOE , ENABLE);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4| GPIO_Pin_8; //Flash_CS	/ TP_CS
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_9;	//
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_13;	//LED1 / GT20-CS 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;	// 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOD, &GPIO_InitStructure);

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	//
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);

	  //4个按键(上、下、左、右)
  	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;	//带上拉输入
	  GPIO_Init(GPIOE, &GPIO_InitStructure);
		
		//GSM-RING GSM-STATUS
  	GPIO_InitStructure.GPIO_Pin   =GPIO_Pin_5 ;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;	//带上拉输入
	  GPIO_Init(GPIOB, &GPIO_InitStructure);		
		
		//GSM-RING GSM-STATUS
  	GPIO_InitStructure.GPIO_Pin   =GPIO_Pin_6;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;	//带上拉输入
	  GPIO_Init(GPIOD, &GPIO_InitStructure);		
		
		
  	GPIO_InitStructure.GPIO_Pin   =GPIO_Pin_5 | GPIO_Pin_6;
	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;	//带上拉输入
	  GPIO_Init(GPIOE, &GPIO_InitStructure);		
		
	
	  GPIO_SetBits(GPIOA, GPIO_Pin_0);			//Flash_CS置高 
	  GPIO_SetBits(GPIOA, GPIO_Pin_4);			//TP_CS置高 
	  GPIO_SetBits(GPIOC, GPIO_Pin_13);			//GT20_CS置高 
	  GPIO_SetBits(GPIOE, GPIO_Pin_4);			//Gsenser_CS置高 
	  BSP_BEEP_OFF();

// AFIO->MAPR = 0x02000000; //释放出三个JTAG的口(PA15,PB3,PB4),做普通IO口线
//浮空输入_IN_FLOATING ,
//带上拉输入_IPU, 
//带下拉输入_IPD ,
//模拟输入_AIN ,
//开漏输出_Out_OD, 
//推挽输出_Out_PP ,
//复用功能的推挽输出_AF_PP,
//复用功能的开漏输出_AF_OD
}




static void  BSP_ADC_Init (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开外设*/
    BSP_PeriphEn(BSP_PERIPH_ID_IOPC);
    BSP_PeriphEn(BSP_PERIPH_ID_ADC1);
    BSP_PeriphEn(BSP_PERIPH_ID_DMA1);
	
	/*打开外设时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	/*GPIO口配置为模拟输入模式*/
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* DMA1配置*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;						//外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (INT32U)&ADC_RCVTab;	//DMA传输的目标变量组(数组首地址)
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;								//DMA传输方向单向
	DMA_InitStructure.DMA_BufferSize = ADC_RCVTab_SIZE;	//DMA传输的目标变量组的成员数(数组的成员数)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//设置DMA的外设递增模式, 一个外设
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;							//设置DMA的内存递增模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//外设数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;				//内存数据字长
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;									//设置DMA的传输模式:Circular循环模式/Normal普通模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;							//设置DMA的优先级别
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;									//设置DMA的2个memory中的变量互相访问
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
    BSP_IntVectSet(BSP_INT_ID_DMA1_CH1, ADC_RCVTabISRHandler);		//设置DMA1的中断服务程序入口为 ADC_RCVTabISRHandler
	BSP_IntEn(BSP_INT_ID_DMA1_CH1);	  //使能DMA1通道1中断
	
	
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);/* 使能DMA通道1传送完成中断*/ 	 
	DMA_Cmd(DMA1_Channel1, ENABLE);/* 使能DMA1通道1 */
	
	/* ADC1配置*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                   //独立工作模式	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;						 //扫描方式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					 //连续转换关闭
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	 //外部触发禁止
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;				 //数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = ADC_RCVTab_SIZE;				 //用于转换的通道数
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*ADC1配置通道规则*/ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1 , 1, ADC_SampleTime_1Cycles5);//NTC热敏电阻
	
	/*ADC1使能DMA*/
	ADC_DMACmd(ADC1, ENABLE);
	
	/*ADC1使能*/
	ADC_Cmd(ADC1, ENABLE);  
	
	//重置指定的ADC的校准寄存器 ；   
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));//等待重置完成
	
	ADC_StartCalibration(ADC1);// 开始指定ADC的校准程序 ；
	while(ADC_GetCalibrationStatus(ADC1)); //等待校准完成
}

//ADC的DMA中断程序
void  ADC_RCVTabISRHandler (void)
{
    CPU_SR          cpu_sr;

    CPU_CRITICAL_ENTER();                                       
    OSIntNesting++;
    CPU_CRITICAL_EXIT();

    if (DMA_GetITStatus(DMA1_IT_TC1) != RESET) 
	{
		ADC_Result[ADC_TEMP]= ADC_RCVTab[ADC_TEMP];

		DMA_ClearITPendingBit(DMA1_IT_TC1);
    }
    OSIntExit();                                                /* Tell uC/OS-II that we are leaving the ISR            */
}

static void  BSP_SPI_Init (void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  SPI_InitTypeDef   SPI_InitStructure; 

  /* SPI1 时钟使能 */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);

  /* SPI1 SCK(PA5)、MISO(PA6)、MOSI(PA7) 设置 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//口线速度50MHZ
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	        //复用模式
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4; //Flash_CS	 TP_CS
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//LED2
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_13;	//LED1  GT20-CS 
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	//Gsenser_CS
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_SetBits(GPIOA, GPIO_Pin_0);			//Flash_CS置高 
  GPIO_SetBits(GPIOA, GPIO_Pin_4);			//TP_CS置高 
  GPIO_SetBits(GPIOC, GPIO_Pin_13);			//GT20_CS置高 
  GPIO_SetBits(GPIOE, GPIO_Pin_4);			//Gsenser_CS置高 

  
   /* SPI1总线 配置 */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工  
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   //主模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   //8位
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High ;						   //时钟极性 空闲状态时，SCK保持低电平
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						   //时钟相位 数据采样从第一个时钟边沿开始
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   //软件产生NSS
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;  //波特率控制 SYSCLK/64
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //数据高位在前
  SPI_InitStructure.SPI_CRCPolynomial = 7;							   //CRC多项式寄存器初始值为7 
  SPI_Init(SPI1, &SPI_InitStructure);
  
  /* SPI1 使能 */  
  SPI_Cmd(SPI1,ENABLE);  
 }


/*
*********************************************************************************************************
*                                              OSView_TmrInit()
*
* Description : Select & initialize a timer for use with uC/OS-View.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/
#if (OS_VIEW_MODULE   == DEF_ENABLED)
void  OSView_TmrInit (void)
{
    TIM_TimeBaseInitTypeDef  tim_init;


    tim_init.TIM_Period        = 0xFFFF;
    tim_init.TIM_Prescaler     = 0x00;
    tim_init.TIM_ClockDivision = 0x0;
    tim_init.TIM_CounterMode   = TIM_CounterMode_Up;

#if (OS_VIEW_TIMER_SEL == 2)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInit(TIM2, &tim_init);
    TIM_SetCounter(TIM2, 0);
    TIM_PrescalerConfig(TIM2, 256, TIM_PSCReloadMode_Immediate);
    TIM_Cmd(TIM2, ENABLE);
#elif (OS_VIEW_TIMER_SEL == 3)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_TimeBaseInit(TIM3, &tim_init);
    TIM_SetCounter(TIM3, 0);
    TIM_PrescalerConfig(TIM3, 256, TIM_PSCReloadMode_Immediate);
    TIM_Cmd(TIM3, ENABLE);
#elif (OS_VIEW_TIMER_SEL == 4)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseInit(TIM4, &tim_init);
    TIM_SetCounter(TIM4, 0);
    TIM_PrescalerConfig(TIM4, 256, TIM_PSCReloadMode_Immediate);
    TIM_Cmd(TIM4, ENABLE);
#endif
}
#endif
/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);

    return ((CPU_INT32U)rcc_clocks.SYSCLK_Frequency);
}

/*
*********************************************************************************************************
*                                         OS_CPU_SysTickClkFreq()
*
* Description : Get system tick clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : Clock frequency (of system tick).
*
* Caller(s)   : BSP_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/
INT32U  OS_CPU_SysTickClkFreq (void)
{
    INT32U  freq;

    freq = BSP_CPU_ClkFreq();
    return (freq);
}
/*
*********************************************************************************************************
*                                         OSView_TmrRd()
*
* Description : Read the current counts of a 32-bit free running timer.
*
* Argument(s) : none.
*
* Return(s)   : The 32 bit counts of the timer.
*********************************************************************************************************
*/

#if (OS_VIEW_MODULE   == DEF_ENABLED)
CPU_INT32U  OSView_TmrRd (void)
{
#if (OS_VIEW_TIMER_SEL == 2)

    return ((CPU_INT32U)TIM_GetCounter(TIM2));

#elif (OS_VIEW_TIMER_SEL == 3)

    return ((CPU_INT32U)TIM_GetCounter(TIM3));

#elif (OS_VIEW_TIMER_SEL == 4)

    return ((CPU_INT32U)TIM_GetCounter(TIM4));

#endif
}
#endif

/*
*********************************************************************************************************
*                                初始化用于调试的串口
*********************************************************************************************************
*/

#if (DEBUG_MODULE == DEF_ENABLED)
static void  Debug_InitTarget ()
{

    GPIO_InitTypeDef        gpio_init;
    USART_InitTypeDef       usart_init;
    USART_ClockInitTypeDef  usart_clk_init;
                                                                /* ----------------- INIT USART STRUCT ---------------- */
    usart_init.USART_BaudRate            = DEBUG_COMM_Speed;
    usart_init.USART_WordLength          = USART_WordLength_8b;
    usart_init.USART_StopBits            = USART_StopBits_1;
    usart_init.USART_Parity              = USART_Parity_No ;
    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;


    usart_clk_init.USART_Clock           = USART_Clock_Disable;
    usart_clk_init.USART_CPOL            = USART_CPOL_Low;
    usart_clk_init.USART_CPHA            = USART_CPHA_2Edge;
    usart_clk_init.USART_LastBit         = USART_LastBit_Disable;


#if (DEBUG_COMM_SEL == DEBUG_UART_1)
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
#endif

#if (DEBUG_COMM_SEL == DEBUG_UART_2)
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
#endif

#if (DEBUG_COMM_SEL == DEBUG_UART_3)
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
#endif
}

/* Implementation of putchar (also used by printf function to output data)    */
PUTCHAR_PROTOTYPE  	/* Write character to Serial Port     */
{                
    USART_TypeDef  *usart;

#if   (DEBUG_COMM_SEL == DEBUG_UART_1)
    usart = USART1;
#elif (DEBUG_COMM_SEL == DEBUG_UART_2)
    usart = USART2;
#elif (DEBUG_COMM_SEL == DEBUG_UART_3)
    usart = USART3;
#else
    return;
#endif

  while (USART_GetFlagStatus(usart, USART_FLAG_TXE) == RESET);
  USART_SendData(usart, (unsigned char) ch);
  return (ch);
}


int GetKey (void)  {                    /* Read character from Serial Port    */
    USART_TypeDef  *usart;

#if   (DEBUG_COMM_SEL == DEBUG_UART_1)
    usart = USART1;
#elif (DEBUG_COMM_SEL == DEBUG_UART_2)
    usart = USART2;
#elif (DEBUG_COMM_SEL == DEBUG_UART_3)
    usart = USART3;
#else
    return;
#endif

  while (USART_GetITStatus(usart, USART_IT_RXNE) != RESET);
  return (USART_ReceiveData(usart));
}
#endif


