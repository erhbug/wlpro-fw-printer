/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp_printer.h"
#include "string.h"
#include "dvr_rtc.h"



/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;

I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim14;


/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

char *FirmwareVersion="PRINTER 21/04/08 R1\r\n";


extern char BuferToPrint[];
extern char BuferTemp[];

/* USER CODE END PV */


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
//static void MX_TIM14_Init(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
void vSend_String_Usart(char *pDatavoid);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/

UART_HandleTypeDef UartHandle;
__IO ITStatus UartReady = RESET;
__IO uint32_t UserButtonStatus = 0;  /* set to 1 after User Button interrupt  */

/* Buffer used for transmission */
//uint8_t aTxBuffer[] = " ****UART_TwoBoards_ComIT****  ****UART_TwoBoards_ComIT****  ****UART_TwoBoards_ComIT**** ";

/* Buffer used for reception */
uint8_t aRxBuffer[SizeOfBuferToPrint];


/* USER CODE BEGIN 0 */


/**
  ******************************************************************************
  * Objective: To obtain the value of the channel ADC.
	******************************************************************************
	*/	#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t)0x1FFFF7B8)) // = 1767
#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t)0x1FFFF7C2)) // = 1310

uint16_t iGet_ADCx_Value( ){
	uint8_t j = 0;
	uint32_t iVectorConvAcc[4]= {0};
	uint8_t iSampleNumber=5;
	
	float RPV1=511;//K
	float RPV2=100;//K
	double Aux;
	
	for(j=0;j<iSampleNumber;j++){		
		
		HAL_ADC_Start(&hadc);
		
				
		while(__HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_EOC) == RESET)
		{
			;
		}		
		iVectorConvAcc[0] += HAL_ADC_GetValue(&hadc);			
		
		while(__HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_EOC) == RESET)
		{
			;
		}		
		iVectorConvAcc[1] += HAL_ADC_GetValue(&hadc);		
		
		while(__HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_EOC) == RESET)
		{
			;
		}		
		iVectorConvAcc[2] += HAL_ADC_GetValue(&hadc);	
		
		while(__HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_EOC) == RESET)
		{
			;	
		}		
		iVectorConvAcc[3] += HAL_ADC_GetValue(&hadc);	
		
	}	
	
	/***************LECTURA DE VOLTAJE DE ENTRADA***********************/
	Aux=((double)(iVectorConvAcc[0]/iSampleNumber))*((double)(3.3/4096));//Obteniendo el voltaje en el pin del MCU	
	Aux=(Aux/RPV2)*(RPV1+RPV2);	//Obtenemos el voltaje de entrada conciderando el divisor de voltaje
	Impresor.PrinterADC.InputVoltaje = Aux+0.2;
	/*******************************************************************/
	
	/***************LECTURA DE VOLTAJE DE TERMISTOR***********************/
	Aux=((double)(iVectorConvAcc[1]/iSampleNumber))*((double)(3.3/4096));//Obteniendo el voltaje 
	//Aux=(Aux/RPV2)*(RPV1+RPV2);	//Obtenemos el voltaje de entrada conciderando el divisor de voltaje
	Impresor.PrinterADC.TempPrintHead = Aux;
	/*******************************************************************/
	
	
	/*******************DETECCION DE PAPEL******************************/
	if(iVectorConvAcc[2]/iSampleNumber<10)	Impresor.PrinterADC.PaperOn = 1;
	else Impresor.PrinterADC.PaperOn = 0;
	Impresor.PrinterADC.PaperOn = 1;// todo b///////////////////////////////////////////////////////
	/*******************************************************************/
	
	
	
	 Aux = iVectorConvAcc[3]/iSampleNumber;//(int32_t)ADC1->DR;
// Compute temperature with a resolution of 0.01°C
Aux = ((Aux * (3.3/4096)) / 3.3) - (double)(*TEMP30_CAL_ADDR);
Aux *= (int32_t)(11000 - 3000);
Aux = Aux / (double)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR);
Aux += 3000;
Impresor.PrinterADC.TempMCU =   Aux/1000    ;		
	
	return 0;
}

void ReadSensors(void){
	
	iGet_ADCx_Value();
	
}

void SendAndPrint(char *txt, char Font){
	
	vSend_String_Usart(txt);//BuferToPrint
	
  Print(txt, Font);
	SpeedMotorMonitor();
}

void SendPrinterInfo(void){
	int a,b;
	char txt[50];
	a=b=0;
	while(a<30){
		HAL_Delay(1);	
	  if(HAL_GPIO_ReadPin(Feed_GPIO_Port, Feed_Pin)==0)b++;
		a++;
	}	
	ReadSensors();
	if(a==b){	
		IWDG->KR =  IWDG_KEY_REFRESH;
		SendAndPrint("\r\n\nTORREY PRINTER.\n\r\n",FONT8x16);		
		
		SendAndPrint("FIRMWARE VERSION: ",FONT8x16);
		SendAndPrint(FirmwareVersion,FONT8x16);				
		
		SendAndPrint("Input Power Suply: ",FONT8x16);		
		sprintf(txt," %2.3f Vdc\r\n",Impresor.PrinterADC.InputVoltaje);		
		SendAndPrint(txt,FONT8x16);		
		
		SendAndPrint("Printer Temperature: ",FONT8x16);		
		sprintf(txt," %2.2f °C\r\n",Impresor.PrinterADC.TempPrintHead);				
		SendAndPrint(txt,FONT8x16);	
		
		SendAndPrint("CPU Temperature: ",FONT8x16);		
		sprintf(txt," %2.2f °C\r\n",Impresor.PrinterADC.TempMCU);				
		SendAndPrint(txt,FONT8x16);	
		
		SendAndPrint("Paper: ",FONT8x16);		
		sprintf(txt," %d\r\n",(int)Impresor.PrinterADC.PaperOn);				
		SendAndPrint(txt,FONT8x16);	
		
		SendAndPrint("Motor Speed: ",FONT8x16);		
		sprintf(txt," %d mm/s\r\n",Impresor.Motor.Speed);				
		SendAndPrint(txt,FONT8x16);			
		
		
		SendAndPrint("Communication Speed: ",FONT8x16);		
		sprintf(txt," %d\r\n",UartHandle.Init.BaudRate);				
		SendAndPrint(txt,FONT8x16);	
		
		
		txt[1]=' ';txt[2]='\0';
		SendAndPrint("\n  FONT8x16\r\n",FONT8x16);		
		for(a=32;a<135;a++){
			txt[0]=a;
			SendAndPrint(txt,FONT8x16);		IWDG->KR =  IWDG_KEY_REFRESH;
		}
		SendAndPrint(" \r\n",FONT8x16);	
		
		SendAndPrint("\n  FONT16x16\r\n",FONT8x16);		
		for(a=32;a<135;a++){
			txt[0]=a;
			SendAndPrint(txt,FONT16x16);		IWDG->KR =  IWDG_KEY_REFRESH;
		}
		SendAndPrint(" \r\n",FONT16x16);	
		
		SendAndPrint("\n  FONT16x24\r\n",FONT16x24);		
		for(a=32;a<135;a++){
			txt[0]=a;
			SendAndPrint(txt,FONT16x24);	IWDG->KR =  IWDG_KEY_REFRESH;	
		}
		SendAndPrint(" \r\n",FONT16x24);	
		
		SendAndPrint("\n  FONT16x32\r\n",FONT16x32);		
		for(a=32;a<135;a++){
			txt[0]=a;
			SendAndPrint(txt,FONT16x32);	IWDG->KR =  IWDG_KEY_REFRESH;	
		}
		SendAndPrint(" \r\n",FONT16x32);
		
		SendAndPrint("\n  FONT24x24\r\n",FONT24x24);		
		for(a=32;a<135;a++){
			txt[0]=a;
			SendAndPrint(txt,FONT24x24);		IWDG->KR =  IWDG_KEY_REFRESH;
		}
		SendAndPrint(" \r\n",FONT24x24);
		
		SendAndPrint("\n  FONT24x32\r\n",FONT24x32);		
		for(a=32;a<135;a++){
			txt[0]=a;
			SendAndPrint(txt,FONT24x32);		IWDG->KR =  IWDG_KEY_REFRESH;
		}
		SendAndPrint(" \r\n",FONT24x24);
SendAndPrint(" \r\n",FONT24x24);
SendAndPrint(" \r\n",FONT24x24);
SendAndPrint(" \r\n",FONT24x24);		
		SendAndPrint(" \r\n",FONT24x24);		
		SendAndPrint(" \r\n",FONT24x24);		
	
 /*
	font=FONT16x32;	
	PrintTextLine("  ",FONT8x16);
	PrintTextLine("Ticket: 00001",font);
	PrintTextLine("  ",FONT8x16);
	PrintTextLine("PLU Kg/Pz  Price  Total",font);
	PrintTextLine("  ",FONT8x16);
	PrintTextLine("************************************************",font);
	//PrintTextLine("--------------------------------------------------",FONT16x24);
	//PrintTextLine("____________________________________________________",FONT16x16);
	PrintTextLine("  ",FONT8x16); 
	PrintTextLine("01 10.000 999.99 9999.99",font);
	PrintTextLine("02 20.000 999.99 9999.99",font);
	PrintTextLine("03 30.000 999.99 9999.99",font);
	PrintTextLine("  ",FONT8x16);
	//PrintTextLine("************************************************",FONT16x16);
	PrintTextLine("--------------------------------------------------",font);
	//PrintTextLine("____________________________________________________",FONT16x16);
	PrintTextLine("  ",FONT8x16);
	PrintTextLine("Items: 3         $ 31.95",font);*/	
	
	}
	
  //PrintTextLine("  ",FONT24x32);
}

/*****************************************************************************************************
******************************************************************************************************/
static int isl1208_enable_oscillator( int val)
{
	char b;
	char out;

	
	
	
	b=ISL1208_read8(REG_CTL_SR);	

	if(val == 0) {
		out = b & 0xBF;
	} else {
		out = b | 0x40;
	}

	//b=i2c_smbus_write_byte_data(client, REG_SR, out);
	ISL1208_write8(REG_CTL_SR,out);

	return 0;
}

void RTC_Init(){
	int status;
	SpeedMotorMonitor();
	
	
	status = ISL1208_read8(REG_CTL_SR);// i2c_smbus_read_byte_data(client, REG_SR);
	if(status & 0x01) {
		//Habilita oscilador externo
		isl1208_enable_oscillator(0);
	
		//Print("Time and date has been lost due to battery power failure...\n", Impresor.Font);
		
    /*//Enable RTC writing		
	  SII_ISL1208StatusRegWr(C_WRTC_En);			
		ISL1208_write8(REG_RTC_SC, 0x00);
    ISL1208_write8(REG_RTC_MN, 0x00);
    ISL1208_write8(REG_RTC_HR, 0x00);    //24-hour mode
    ISL1208_write8(REG_RTC_DT, 0x23);
    ISL1208_write8(REG_RTC_MO, 0x05);
		ISL1208_write8(REG_RTC_YR, 0x12);
    ISL1208_write8(REG_RTC_DW, 0x03);
		//Disable RTC writing
		SII_ISL1208StatusRegWr(C_WRTC_Dis);	*/
		
		Impresor.DATE.year=2019;
		Impresor.DATE.month=8;
		Impresor.DATE.day=28;
		Impresor.DATE.dayw=3;
		Impresor.DATE.hour=10;
		Impresor.DATE.min=50;
		Impresor.DATE.sec=10;
    ISL1208time(Impresor.DATE);
				
		status=ISL1208_read8(REG_CTL_SR);
		status&=0xFD;
		status|=0x80;
		ISL1208_write8(REG_CTL_SR,status);
	}



}

void LimpiarBuferRecepcion(void){
int a;

	FlagSerial0=0;					
  for(a=0;a<SizeOfBuferToPrint;a++)
    aRxBuffer[a]=0;
	Impresor.Indice=0;
}
 
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void){
  /* USER CODE BEGIN 1 */
	uint32_t tickstart = 0;
	char txt[2];
	int count=0;
	
	
  /* USER CODE END 1 */
  
	Impresor.Motor.SpeedJack = SPEED_90_MMPS;
	Impresor.Motor.SpeedBattery =SPEED_75_MMPS;

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
		__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
  MX_TIM14_Init(SPEED_80_MMPS);//MX_TIM14_Init();
	HAL_TIM_Base_Start_IT(&htim14);
	HAL_NVIC_EnableIRQ(TIM14_IRQn);	

	tickstart = HAL_GetTick();		
		IWDG->KR = 0x0000CCCC; // (1)
		IWDG->KR = 0x00005555; // (2)
		IWDG->PR = IWDG_PR_PR_2; // (3)
		IWDG->RLR = 4000;//1250; // (4)
		while (IWDG->SR){ // (5)		
			if(HAL_GetTick()-tickstart > 250)
				break;			
		}
		IWDG->KR =  IWDG_KEY_REFRESH;		
	InitstrImpresor();
	HAL_Delay(20);
		vSend_String_Usart(FirmwareVersion);
	
//SendPrinterInfo();	

RTC_Init();

/*Limpiar Bufer de recepcion*/
		LimpiarBuferRecepcion();
		ClearBufferToPrint();	
txt[1]		=0;	
		//HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET); 		
		MainPrinterBmpRS232();		
while (1) {
	IWDG->KR =  IWDG_KEY_REFRESH;
  count++;
  if (count == 100) {
    SpeedMotorMonitor();
    count = 0;
  }
  if (FlagSerial0 == 1 && GetCommand() == CMD_NULL) {
		//Print("\n\n", Impresor.Font);			
    txt[0] = aRxBuffer[Impresor.Indice];
    Print(txt, Impresor.Font); //HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);  
    Impresor.Indice++;
    if (aRxBuffer[Impresor.Indice] == '\0')
      LimpiarBuferRecepcion();
  }
  if (Impresor.CMD.CMD != CMD_NULL)
    EjectCMD();
	/*Impresor.Motor.Status=START;
	Impresor.Motor.NumberOfSteps=100;*/
	//Print("\n\n", Impresor.Font);	
  if ( FlagSerial0 == 0) {
    if (HAL_GPIO_ReadPin(Feed_GPIO_Port, Feed_Pin) == 0) {
        Print("\n\n", Impresor.Font);			
    }
  }
}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{
  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_8;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_9;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel to be converted. 
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  //hi2c1.Init.Timing = 0x100091A;//0x0000020B;//0x2010091A;
	  //hi2c1.Init.Timing = 0x0FF221A;//100KHZ
	hi2c1.Init.Timing = 0x00F221A;//400KHZ
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void){
  UartHandle.Instance        = USARTx;
  UartHandle.Init.BaudRate   = 230400;//230400;//9600;//115200;//57600;//19200;//9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT; 
  if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }  
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MOTOR_I10_OUT_Pin|MOTOR_I11_OUT_Pin|MOTOR_PHASE1_OUT_Pin|MOTOR_I20_OUT_Pin 
                          |MOTOR_I21_OUT_Pin|MOTOR_PHASE2_OUT_Pin|MOTOR_INHIBIT_OUT_Pin|VT1_OE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, VT2_OE_Pin|PH_DI_Pin|PH_CLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PH_LATCH_Pin|PH_STB1_Pin|PH_STB2_Pin|PH_STB3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : MOTOR_I10_OUT_Pin MOTOR_I11_OUT_Pin MOTOR_PHASE1_OUT_Pin MOTOR_I20_OUT_Pin 
                           MOTOR_I21_OUT_Pin MOTOR_PHASE2_OUT_Pin MOTOR_INHIBIT_OUT_Pin LED_Pin 
                           VT1_OE_Pin */
  GPIO_InitStruct.Pin = MOTOR_I10_OUT_Pin|MOTOR_I11_OUT_Pin|MOTOR_PHASE1_OUT_Pin|MOTOR_I20_OUT_Pin 
                          |MOTOR_I21_OUT_Pin|MOTOR_PHASE2_OUT_Pin|MOTOR_INHIBIT_OUT_Pin|LED_Pin 
                          |VT1_OE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : VT2_OE_Pin PH_DI_Pin PH_CLK_Pin PH_LATCH_Pin 
                           PH_STB1_Pin PH_STB2_Pin PH_STB3_Pin */
  GPIO_InitStruct.Pin = VT2_OE_Pin|PH_DI_Pin|PH_CLK_Pin|PH_LATCH_Pin 
                          |PH_STB1_Pin|PH_STB2_Pin|PH_STB3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Feed_Pin */
  GPIO_InitStruct.Pin = Feed_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Feed_GPIO_Port, &GPIO_InitStruct);
	
	
	HAL_GPIO_WritePin(VT1_OE_GPIO_Port, VT1_OE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(VT2_OE_GPIO_Port, VT2_OE_Pin, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);  			
}

/* USER CODE BEGIN 4 */

void vSend_String_Usart(char *pData){
char a=0;
	char b=0;
	for(a=0;a<64;a++){		
		b++;
		if(pData[a]=='\0')
	     a=100;					
	}
	while(HAL_UART_Transmit(&UartHandle,(uint8_t *)pData,b,0x0FFF) != HAL_OK ){
			;//	IWDG->KR = 0xAAAA;
			}	
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle){
  /* Set transmission flag: transfer complete */
  UartReady = SET;
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){
  /* Set transmission flag: transfer complete */
  UartReady = SET;
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
    Error_Handler();
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
