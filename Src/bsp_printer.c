//helo
#include "main.h"
#include "bsp_printer.h"

#include "f_8x16.h"
#include "f_16x16.h"
#include "f_16x24.h"
#include "f_16x32.h"
#include "f_24x24.h"
#include "f_24x32.h"


extern void vSend_String_Usart(char *pDatavoid);
extern uint8_t aRxBuffer[];
char BuferToPrint[SizeOfBuferToPrint];//
char BuferTemp[SizeOfBuferToPrint];

struct strImpresor Impresor;
	

void OnStrobe0(void){
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_STB1_Pin, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_STB3_Pin, GPIO_PIN_SET);
}

void OnStrobe1(void){
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_STB1_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_STB2_Pin, GPIO_PIN_RESET);	
}

void OnStrobe2(void){
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_STB2_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_STB3_Pin, GPIO_PIN_RESET);
}

void OffAllStrobes(void){
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_STB1_Pin, GPIO_PIN_SET);	
				HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_STB2_Pin, GPIO_PIN_SET);	
				HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_STB3_Pin, GPIO_PIN_SET);
	
}

void ClearBufferToPrint(void){
	for(int j=0;j<SizeOfBuferToPrint;j++)
	  BuferToPrint[j]=0x00;	
}

void InitstrImpresor(void){
	int i,j;	
	Impresor.CMD.CMD=CMD_NULL;
	Impresor.CMD.ACTION=ACTION_NULL;	
	OffAllStrobes();
	Impresor.Motor.Status=STOP;	
	Impresor.CountStrobe=0;
	Impresor.TimeStrobeON=5000;
	for(i=0;i<LINES_ARRAY;i++){
		for(j=0;j<BYTES_PER_ROW;j++){
			Impresor.HeaderBufer[i][j]=0x00;		
		}
	}
	Impresor.CountLine=0;
	Impresor.Font=FONT16x24;	
	Impresor.Motor.NumberOfSteps=0;	//1cm = 40 1mm=4  1/4mm =1 step	
	Impresor.Motor.Count=0;
	Impresor.Motor.Speed=255;	
	Impresor.Motor.Activate=DISABLE;	
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_CLK_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_DI_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_LATCH_Pin, GPIO_PIN_SET);	
	ReadSensors();
	SpeedMotorMonitor();	
	BSP_Printer_FillZEROsFirstLine();		
}

void WritePrintHead(void){
	char a =0;	
			for(a=0;a<BYTES_PER_ROW;a++){	
				send_printhead_byte(BmpBufferRS232.BufferPrinterHeader[a]);
				}		
      BmpBufferRS232.PrintHeadDataSent=1;	
			HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_LATCH_Pin, GPIO_PIN_RESET);	
			HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_LATCH_Pin, GPIO_PIN_SET);		
}

void vInterrup_TIM14(void){	
	__HAL_TIM_CLEAR_FLAG(&htim14, TIM_FLAG_UPDATE);	
	if(Impresor.Motor.Status==STOP){
		HAL_GPIO_WritePin(MOTOR_GPIO_Port, MOTOR_INHIBIT_OUT_Pin, GPIO_PIN_SET);
		OffAllStrobes();
		Impresor.Motor.Activate=DISABLE;		
    Impresor.Motor.Count=6;
		return;
	}
	Impresor.Motor.Count++;	
	if(Impresor.Motor.Count>=6)	
		Impresor.Motor.Count=0;	
	if(Impresor.Motor.Count==0){
		HAL_GPIO_WritePin(MOTOR_GPIO_Port, MOTOR_INHIBIT_OUT_Pin, GPIO_PIN_RESET);
		HAL_GPIO_TogglePin(MOTOR_I10_OUT_GPIO_Port,MOTOR_I10_OUT_Pin);
		WritePrintHead();
		OnStrobe0();
		return;
	}
	if(Impresor.Motor.Count==2){
		OnStrobe1();
		return;
	}
	if(Impresor.Motor.Count==3){
		HAL_GPIO_TogglePin(MOTOR_I10_OUT_GPIO_Port,MOTOR_I10_OUT_Pin);
		return;
	}
	if(Impresor.Motor.Count==4){
		OnStrobe2();
		if(Impresor.Motor.NumberOfSteps>0)Impresor.Motor.NumberOfSteps--;		
		if(Impresor.Motor.NumberOfSteps==0 )Impresor.Motor.Status=STOP;
	}
}



/* TIM14 init function */
 void MX_TIM14_Init(uint8_t Speed){//INTERRUPCION 1 MS	
	 switch(Speed){
		case SPEED_40_MMPS: htim14.Init.Period=  7000; break; 
		case SPEED_45_MMPS: htim14.Init.Period=  6000; break; 
		case SPEED_50_MMPS: htim14.Init.Period=  5000; break;  
		case SPEED_55_MMPS: htim14.Init.Period=  4000; break;  
		case SPEED_60_MMPS: htim14.Init.Period=  3500; break;
		case SPEED_65_MMPS: htim14.Init.Period=  3000; break;
		case SPEED_70_MMPS: htim14.Init.Period=  2500; break;
		case SPEED_75_MMPS: htim14.Init.Period=  2000; break;
		case SPEED_80_MMPS: htim14.Init.Period=  1200; break;
		 
		//case SPEED_85_MMPS: htim14.Init.Period=  800; break;//ok
		//case SPEED_85_MMPS: htim14.Init.Period=  700; break;//ok
		case SPEED_85_MMPS: htim14.Init.Period=  580; break;//ok
		 
		case SPEED_90_MMPS: htim14.Init.Period=  500; break;
		case SPEED_95_MMPS: htim14.Init.Period=  450; break;
		case SPEED_100_MMPS: htim14.Init.Period= 400; break;
		case SPEED_105_MMPS: htim14.Init.Period= 350; break;
		 
		case SPEED_110_MMPS: htim14.Init.Period= 120; break;
		default: htim14.Init.Period= 1000; break;
	}
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 47;//47->1ms
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  //htim14.Init.Period = Speed;//999->1ms
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
   // Error_Handler();
  }
}

void SET_HEADER_LAT(char value){
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_LATCH_Pin, (GPIO_PinState)value);	//21901069
}
void SET_HEADER_STB1(char value){
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_STB1_Pin, (GPIO_PinState)value);	
}
void SET_HEADER_STB2(char value){
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_STB2_Pin, (GPIO_PinState)value);	
}
void SET_HEADER_STB3(char value){
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_STB3_Pin, (GPIO_PinState)value);		
}
void SET_HEADER_DI(char value){
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_DI_Pin, (GPIO_PinState)value);	
}
void SET_HEADER_CLK(char value){
	HAL_GPIO_WritePin(PRINT_HEAD_GPIO_Port, PH_CLK_Pin, (GPIO_PinState)value);	
}


void send_printhead_bit(unsigned char DataBit)
	{
	SET_HEADER_DI(DataBit);
	SET_HEADER_CLK(DEF_SET);
	SET_HEADER_CLK(DEF_CLR);
	}

void BSP_Printer_FillZEROsFirstLine(void){
  int i;
  SET_HEADER_LAT(DEF_SET);
	HAL_Delay(5);
	SET_HEADER_STB1(DEF_SET); /* Reverse logic at output */
  SET_HEADER_STB2(DEF_SET);
  SET_HEADER_STB3(DEF_SET);
                
   for(i=0;i<BYTES_PER_ROW;i++) send_printhead_byte(0x00);
	SET_HEADER_LAT(DEF_CLR); 
	SET_HEADER_LAT(DEF_SET); 
}

void send_printhead_byte(uint8_t DataByte)
	{
	uint32_t i;
		uint32_t RspODR=0;
	for(i=0;i<=7;i++)		{			
			RspODR=PRINT_HEAD_GPIO_Port->ODR;			
			RspODR&=0xfbff;			
			if(DataByte&0x80)
			RspODR|=DEF_BIT_10;			
			RspODR|=DEF_BIT_11;
			PRINT_HEAD_GPIO_Port->ODR=RspODR;
			DataByte<<=1;				
			PRINT_HEAD_GPIO_Port->ODR&=0xf7ff;					
		}
	}
	void send_printhead_byte_oRIG(uint8_t DataByte)
	{
	uint32_t i;
		uint32_t RspODR=0;
	for(i=0;i<=7;i++)		{			
			RspODR=PRINT_HEAD_GPIO_Port->ODR;			
			RspODR&=0xfbff;			
			if(DataByte&0x01)
			RspODR|=DEF_BIT_10;			
			RspODR|=DEF_BIT_11;
			PRINT_HEAD_GPIO_Port->ODR=RspODR;
			DataByte>>=1;				
			PRINT_HEAD_GPIO_Port->ODR&=0xf7ff;					
		}
	}
		
	void GetFontParam(char FontType){
		 if(FontType==FONT8x16){//ok
		Impresor.Motor.NumberOfSteps=16;//NumberOfLines
		Impresor.CaracteresPorLinea=MAX_N_COLS/8;
	}
	if(FontType==FONT16x16){//ok?
		Impresor.Motor.NumberOfSteps=16;
		Impresor.CaracteresPorLinea=MAX_N_COLS/16;
	}
	if(FontType==FONT16x24){
		Impresor.Motor.NumberOfSteps=24;
		Impresor.CaracteresPorLinea=MAX_N_COLS/16;
	}
	if(FontType==FONT16x32){
		Impresor.Motor.NumberOfSteps=32;
		Impresor.CaracteresPorLinea=MAX_N_COLS/16;
	}
	if(FontType==FONT24x24){
		Impresor.Motor.NumberOfSteps=24;
		Impresor.CaracteresPorLinea=MAX_N_COLS/24;
	}
	if(FontType==FONT24x32){
		Impresor.Motor.NumberOfSteps=32;
		Impresor.CaracteresPorLinea=MAX_N_COLS/24;
	}
		
	}
 
void PrintTextLine(char *Bufer,char FontType){
	int Caracter,Fila,i,j;		
	
	if(Impresor.PrinterADC.PaperOn==0)return;
	
	for(i=0;i<LINES_ARRAY;i++){
		for(j=0;j<BYTES_PER_ROW;j++){
			Impresor.HeaderBufer[i][j]=0x00;			
		}
	}

	GetFontParam(FontType); 

	for(Caracter=0;Caracter<Impresor.CaracteresPorLinea;Caracter++){
	for(Fila=0;Fila<Impresor.Motor.NumberOfSteps;Fila++){
		
			if(Bufer[Caracter]=='\0'){Caracter=Impresor.CaracteresPorLinea;Fila=Impresor.Motor.NumberOfSteps;}
			else{
			  switch(FontType){
					
					
					case FONT8x16: //ok
						              Impresor.HeaderBufer[Fila][Caracter]=f8x16[Bufer[Caracter]][Fila]; 
					break;
					
					case FONT16x16: //ok
						              Impresor.HeaderBufer[Fila][Caracter*2]=f16x16[Bufer[Caracter]][Fila*2+1]; 					                
					                Impresor.HeaderBufer[Fila][(Caracter*2)+1]=f16x16[Bufer[Caracter]][Fila*2];  
					break;
					
					case FONT16x24: 
                          Impresor.HeaderBufer[Fila][Caracter*2]=f16x24[Bufer[Caracter]][Fila*2+1];
                          Impresor.HeaderBufer[Fila][Caracter*2+1]=f16x24[Bufer[Caracter]][Fila*2];
					break;
					
					case FONT16x32: Impresor.HeaderBufer[Fila][Caracter*2]=f16x32[Bufer[Caracter]][Fila*2+1];
                          Impresor.HeaderBufer[Fila][Caracter*2+1]=f16x32[Bufer[Caracter]][Fila*2]; 
					break;
					
					case FONT24x24: Impresor.HeaderBufer[Fila][Caracter*3]=f24x24[Bufer[Caracter]][Fila*3+2];
                          Impresor.HeaderBufer[Fila][Caracter*3+1]=f24x24[Bufer[Caracter]][Fila*3+1];
					                Impresor.HeaderBufer[Fila][Caracter*3+2]=f24x24[Bufer[Caracter]][Fila*3];
					break;
					
					case FONT24x32: Impresor.HeaderBufer[Fila][Caracter*3]=f24x32[Bufer[Caracter]][Fila*3+2];
                          Impresor.HeaderBufer[Fila][Caracter*3+1]=f24x32[Bufer[Caracter]][Fila*3+1];
					                Impresor.HeaderBufer[Fila][Caracter*3+2]=f24x32[Bufer[Caracter]][Fila*3];
					break;
														
					default: Impresor.Motor.NumberOfSteps=3000; break;
				}
				
			}
		}			
	}
	
	Impresor.Motor.Status=START;	
uint32_t tickstart = HAL_GetTick(); 	
	while(Impresor.Motor.Status!=STOP){
		tickstart = HAL_GetTick();
		while((HAL_GetTick() - tickstart) < 1 && Impresor.Motor.Status!=STOP )
      ;//IWDG->KR =  0x0000AAAA;
	}
	
	
  
	
	//vSend_String_Usart("End_PrintTextLine");
	
}

//extern int Acelerar;
char Flag0=0;
void Print(char *txt, char Font){
	char a=0;
//	char b=0;
	char x;
	char indice=0;
	
		if(Impresor.PrinterADC.PaperOn==0)return;
	GetFontParam(Font);
	
	for(x=0;x<Impresor.CaracteresPorLinea;x++){
		if(BuferToPrint[x]==0){
			a=x;
			x=100;
		}		//	IWDG->KR =  IWDG_KEY_REFRESH;
	}
	
	while(txt[indice]!='\0'){
		
	if(a==0&&txt[indice]=='\n'){				
	/////Aumentar la velocidad////
	if(Flag0==0 && Impresor.Motor.Speed<=SPEED_80_MMPS){
		Flag0=1;
    HAL_TIM_Base_Stop_IT(&htim14);	
	  //if(Impresor.Motor.Speed<=SPEED_80_MMPS)
	  htim14.Init.Period=htim14.Init.Period*0.5;		  
    HAL_TIM_Base_Init(&htim14);	
	  HAL_TIM_Base_Start_IT(&htim14);	
  }
			ClearBufferToPrint();	
		  strcpy(BuferToPrint,"   ");
//	    Acelerar=200;//entrar en modo aceleracion vaya a la funcion de la interrupcion
		  PrintTextLine(BuferToPrint,Font);
		  ClearBufferToPrint();			
		//Reestablecer velocidad///			
		return;
	}
	else{
		if(Flag0==1){
		  HAL_TIM_Base_Stop_IT(&htim14);
		  MX_TIM14_Init(Impresor.Motor.Speed);
		  HAL_TIM_Base_Start_IT(&htim14);
		  Flag0=0;
	 	  //Reestablecer velocidad//		
	  }
	}
	
	if(txt[indice]=='\n'){		
		PrintTextLine(BuferToPrint,Font);
		ClearBufferToPrint();		
		return;
	}
	
	if(a==Impresor.CaracteresPorLinea-1){
		BuferToPrint[a]=txt[indice];
		PrintTextLine(BuferToPrint,Font);
	  ClearBufferToPrint();
		return;
	}
	
	if(a<Impresor.CaracteresPorLinea-1){
		BuferToPrint[a]=txt[indice];
		
	}
	indice++;
	a++;
}
	
	return;
}

char GetCommand(void){
	char txt[30];
	int a=0;
	//char Indice=0;
	
	if(FlagSerial0==1){		
		txt[0]=aRxBuffer[Impresor.Indice];  
		txt[1]=aRxBuffer[Impresor.Indice+1];
		txt[2]=aRxBuffer[Impresor.Indice+2];
		txt[3]=aRxBuffer[Impresor.Indice+3];
		txt[4]=aRxBuffer[Impresor.Indice+4];
		txt[5]='\0';						
		
		if(strcmp(txt,"#CMD ")==0){		
			Impresor.Indice+=5;					
			for(a=0;a<10;a++){
				txt[a]=aRxBuffer[Impresor.Indice];
				Impresor.Indice++;
				if(txt[a]==' '){
					txt[a+1]='\0';
					a=100;
				}					
			}
								
			/**********************CMD***************************/
			Impresor.CMD.CMD=CMD_NULL;
			if(strcmp(txt,"FEED ")==0){Impresor.CMD.CMD=CMD_FEED;}//Print("FEED \n", FONT8x16);	}
			if(strcmp(txt,"DATE ")==0){Impresor.CMD.CMD=CMD_DATE;}//Print("DATE \n", FONT8x16);}
			if(strcmp(txt,"CLR_BUF ")==0){Impresor.CMD.CMD=CMD_CLEAR_BUFFER;}//Print("CLR_BUF \n", FONT8x16);}
			if(strcmp(txt,"ASK_SENS ")==0){Impresor.CMD.CMD=CMD_ASK_SENSOR;}//Print("ASK_SENS \n", FONT8x16);}
			if(strcmp(txt,"SPEED ")==0){Impresor.CMD.CMD=CMD_SPEED;}//Print("ASK_SENS \n", FONT8x16);}
			if(strcmp(txt,"NOTIFY ")==0){Impresor.CMD.CMD=CMD_NOTIFY;}//Print("ASK_SENS \n", FONT8x16);}			
			if(strcmp(txt,"DEMO ")==0){Impresor.CMD.CMD=CMD_DEMO;}//Print("ASK_SENS \n", FONT8x16);}			
			/***************************************************/
			
			for(a=0;a<30;a++){
				txt[a]=aRxBuffer[Impresor.Indice];
				Impresor.Indice++;
				if(txt[a]=='\n'){
					txt[a]='\0';
					a=100;
				}//	IWDG->KR =  IWDG_KEY_REFRESH;					
			}		
			
			/********************ACTION*************************/
			Impresor.CMD.ACTION=ACTION_NULL;
			if(strcmp(txt,"START")==0){Impresor.CMD.ACTION=ACTION_START;}//Print("START \n", FONT8x16);}
			if(strcmp(txt,"STOP")==0){Impresor.CMD.ACTION=ACTION_STOP;}//Print("STOP \n", FONT8x16);}
			if(strcmp(txt,"SET")==0){Impresor.CMD.ACTION=ACTION_SET;}//Print("SET \n", FONT8x16);}
			if(strcmp(txt,"GET")==0){Impresor.CMD.ACTION=ACTION_GET;}//Print("GET \n", FONT8x16);}
			if(strcmp(txt,"PRINT")==0){Impresor.CMD.ACTION=ACTION_PRINT;}//Print("PRINT \n", FONT8x16);}
			if(strcmp(txt,"PAPER")==0){Impresor.CMD.ACTION=ACTION_ASK_PAPER;}//Print("PAPER \n", FONT8x16);}
			if(strcmp(txt,"END_OF_PRINT")==0){Impresor.CMD.ACTION=ACTION_END_OF_PRINT;}//Print("PAPER \n", FONT8x16);}			
			if(strcmp(txt,"PRINT_DEMO_0")==0){Impresor.CMD.ACTION=ACTION_PRINT_DEMO_0;}//Print("PAPER \n", FONT8x16);}			
		
			/***************************************************/
			
			if(aRxBuffer[Impresor.Indice]=='\0')
			  LimpiarBuferRecepcion();		
		 }		
	}
	
		
	return Impresor.CMD.CMD;
}

unsigned long SecSince1Jan1970(struct RTC_Format DATE){
	long value=0;
	
	if(DATE.year<1970)
		DATE.year=1970;
	
	DATE.year-=1970;	
	value=(unsigned long)(DATE.year*31556926);
	value+=(unsigned long)(DATE.month*2629743);
	value+=(unsigned long)(DATE.day*86400);
	value+=(unsigned long)(DATE.hour*3600);
	value+=(unsigned long)(DATE.min*60);
	value+=(unsigned long)(DATE.sec);
	return value;
}


void Demo0(void){
	char string[30];		
	char txt[2];
	txt[0]='0';
	txt[1]=0;
	Impresor.DATE=ISL1208_time();	
while(1){	//
	
	SpeedMotorMonitor();
	if(Impresor.PrinterADC.TempPrintHead<=1.45){
		Print("\n", Impresor.Font);
		Print("\n", Impresor.Font);
		Print("\n", Impresor.Font);
		Print("Enfriando...", Impresor.Font);
		 Print("\n", Impresor.Font);
		Print("\n", Impresor.Font);
		Print("\n", Impresor.Font);
		Print("\n", Impresor.Font);
		Print("\n", Impresor.Font);
		while(Impresor.PrinterADC.TempPrintHead<=1.55){
			SpeedMotorMonitor();			
		}
	}
	IWDG->KR =  IWDG_KEY_REFRESH;		
	
	if(Impresor.DATE.sec<=5){
		 Print("\n", Impresor.Font);
		Print("\n", Impresor.Font);
		Print("\n", Impresor.Font);
		Print("Espera 30 Segundos", Impresor.Font);
		 Print("\n", Impresor.Font);
		Print("\n", Impresor.Font);
		Print("\n", Impresor.Font);
		Print("\n", Impresor.Font);
		Print("\n", Impresor.Font);
		while(Impresor.DATE.sec<30){
		  Impresor.DATE=ISL1208_time();	
		}		
		
	}
	Print(txt, Impresor.Font);
	txt[0]++;
	if(txt[0]>'z'){
		txt[0]='0';		
	Impresor.DATE=ISL1208_time();		
  sprintf(string,"-T%04.2f,V%04.2f,%02d:%02d-",Impresor.PrinterADC.TempPrintHead,Impresor.PrinterADC.InputVoltaje,Impresor.DATE.min,Impresor.DATE.sec);
	Print(string, Impresor.Font);
		
	}	
	
	
	
	
}

}

void EjectCMD(void){	
	char txt[50];
	int Count=0;
	if(Impresor.CMD.CMD!=CMD_NULL){
		switch(Impresor.CMD.CMD){
			case CMD_CLEAR_BUFFER: 
			ClearBufferToPrint();	
			LimpiarBuferRecepcion();	
			
			//Print("ERIBERT \n", FONT8x16);	
			Impresor.CMD.CMD=CMD_NULL;
		  Impresor.CMD.ACTION=ACTION_NULL;					
			break;
			//Impresor.PrinterADC.PaperOn = 0;
			
			case CMD_DEMO: 
				switch(Impresor.CMD.ACTION){
					case ACTION_PRINT_DEMO_0:
						
					Demo0();			
									
						break;
					default: break;
					
				}
			break;	
			
			case CMD_ASK_SENSOR: 
				switch(Impresor.CMD.ACTION){
					case ACTION_ASK_PAPER:
						
					 if(Impresor.PrinterADC.PaperOn == 0)
						 vSend_String_Usart("N");
						 else
							 vSend_String_Usart("Y");							 
						
						Impresor.CMD.CMD=CMD_NULL;
		        Impresor.CMD.ACTION=ACTION_NULL;						
						break;
					default: break;
				}
			break;				
				
			case CMD_SPEED: 
				switch(Impresor.CMD.ACTION){
					case ACTION_SET:						
						Impresor.CMD.CMD=CMD_NULL;
		        Impresor.CMD.ACTION=ACTION_NULL;		
				  	LimpiarBuferRecepcion();	
				    while(FlagSerial0==0&&Count<2000){
							Count++;
							HAL_Delay(1);			
						}		
					  if(FlagSerial0==1&&aRxBuffer[0]!='\0'&&aRxBuffer[1]!='\0'){							
							if((aRxBuffer[0]>aRxBuffer[1])&&(aRxBuffer[0]>=SPEED_MIN&&aRxBuffer[0]<=SPEED_MAX)&&(aRxBuffer[1]>=SPEED_MIN&&aRxBuffer[1]<=SPEED_MAX)){								
								  Impresor.Motor.SpeedJack = aRxBuffer[0];	
                  Impresor.Motor.SpeedBattery = aRxBuffer[1];
								/*sprintf(txt,"MAX %02d, MIN %02d",Impresor.Motor.SpeedJack,Impresor.Motor.SpeedBattery);
		         PrintTextLine(txt,Impresor.Font);*/
							}
							Impresor.Indice+=2;
						}
							
						break;
					default: break;
				}
			break;	
				
			case CMD_NOTIFY: 
				switch(Impresor.CMD.ACTION){
					case ACTION_END_OF_PRINT:
						vSend_String_Usart("FINISHED\r\n");	
						Impresor.CMD.CMD=CMD_NULL;
		        Impresor.CMD.ACTION=ACTION_NULL;				
						break;
					default: break;
				}
			  break;	
				
			case CMD_FEED: 
				switch(Impresor.CMD.ACTION){
					case ACTION_START:
						Impresor.CMD.ACTION=ACTION_WORKING;					
						break;
					case ACTION_WORKING:
						Print("\n\n", FONT8x16);	
            //IWDG->KR =  IWDG_KEY_REFRESH;					
						break;
					case ACTION_STOP:
						Impresor.CMD.CMD=CMD_NULL;
		        Impresor.CMD.ACTION=ACTION_NULL;				  
						break;
					default: break;
				}
			  break;
			case CMD_DATE: 
				switch(Impresor.CMD.ACTION){
					case ACTION_SET: 						
            Impresor.CMD.CMD=CMD_NULL;
		        Impresor.CMD.ACTION=ACTION_NULL;	
					  
					LimpiarBuferRecepcion();	
				    while(FlagSerial0==0&&Count<10000){
							Count++;
							HAL_Delay(1);			//				
						}					
						
						if(FlagSerial0==1&&aRxBuffer[14]=='\0'){							
				
              Impresor.DATE.year=(aRxBuffer[0]-'0')*1000;
		          Impresor.DATE.year+=(aRxBuffer[1]-'0')*100;
		          Impresor.DATE.year+=(aRxBuffer[2]-'0')*10;
							Impresor.DATE.year+=(aRxBuffer[3]-'0');
							
							Impresor.DATE.month=(aRxBuffer[4]-'0')*10;
		          Impresor.DATE.month+=(aRxBuffer[5]-'0');
							
							Impresor.DATE.day=(aRxBuffer[6]-'0')*10;
		          Impresor.DATE.day+=(aRxBuffer[7]-'0');
							
							Impresor.DATE.hour=(aRxBuffer[8]-'0')*10;
		          Impresor.DATE.hour+=(aRxBuffer[9]-'0');
							
							Impresor.DATE.min=(aRxBuffer[10]-'0')*10;
		          Impresor.DATE.min+=(aRxBuffer[11]-'0');
							
							Impresor.DATE.sec=(aRxBuffer[12]-'0')*10;
		          Impresor.DATE.sec+=(aRxBuffer[13]-'0');
					
          ISL1208time(Impresor.DATE);		
					
					Impresor.DATE=ISL1208_time();					
					//Print(txt, Impresor.Font);	
					sprintf(txt,"%04d/%02d/%02d  %02d:%02d:%02d\n",Impresor.DATE.year,Impresor.DATE.month,Impresor.DATE.day,Impresor.DATE.hour,Impresor.DATE.min,Impresor.DATE.sec);
					Impresor.Indice+=14;
		
	}
						else{
							Print("Set Time Error!\n", Impresor.Font);	
						}
									
					break;
					case ACTION_GET: 
						Impresor.CMD.CMD=CMD_NULL;
		        Impresor.CMD.ACTION=ACTION_NULL;
					
					
					Impresor.DATE=ISL1208_time();								
					sprintf(txt,"%04d/%02d/%02d-%02d:%02d:%02d\n",Impresor.DATE.year,Impresor.DATE.month,Impresor.DATE.day,Impresor.DATE.hour,Impresor.DATE.min,Impresor.DATE.sec);
					//sprintf(txt,"%lu\n",SecSince1Jan1970(Impresor.DATE));
					//Print(txt, Impresor.Font);	
					vSend_String_Usart(txt);
										
					
					//LimpiarBuferRecepcion();
			
					break;
					case ACTION_PRINT: 
						Impresor.DATE=ISL1208_time();					  
					  sprintf(txt,"%02d/%02d/%04d   %02d:%02d\n",Impresor.DATE.month,Impresor.DATE.day,Impresor.DATE.year,Impresor.DATE.hour,Impresor.DATE.min);
					  Print("\n\n", Impresor.Font);	
						Print(txt, Impresor.Font);	
						Impresor.CMD.CMD=CMD_NULL;
		        Impresor.CMD.ACTION=ACTION_NULL;
					break;
					default: break;
				}
				break;
			default: break;
		}			
		}	
}

		/*		SII_ISL1208StatusRegWr(C_WRTC_En);			
		ISL1208_write8(REG_RTC_SC, 0x10);
    ISL1208_write8(REG_RTC_MN, 0x50);
    ISL1208_write8(REG_RTC_HR, 0x10);    //24-hour mode
    ISL1208_write8(REG_RTC_DT, 0x28);
    ISL1208_write8(REG_RTC_MO, 0x08);
		ISL1208_write8(REG_RTC_YR, 0x19);
    ISL1208_write8(REG_RTC_DW, 0x03);
		//Disable RTC writing
		SII_ISL1208StatusRegWr(C_WRTC_Dis);	*/
					
		/*char status=ISL1208_read8(REG_CTL_SR);
		status&=0xFD;
		status|=0x80;
		ISL1208_write8(REG_CTL_SR,status);*/

/*#define SPEED_40_MMPS 1
#define SPEED_45_MMPS 2
#define SPEED_50_MMPS 3
#define SPEED_55_MMPS 4
#define SPEED_60_MMPS 5
#define SPEED_65_MMPS 6
#define SPEED_70_MMPS 7
#define SPEED_75_MMPS 8
#define SPEED_80_MMPS 9
#define SPEED_85_MMPS 10
#define SPEED_90_MMPS 11
#define SPEED_95_MMPS 12
#define SPEED_100_MMPS 13*/
void SpeedMotorMonitor(void){
//	float Delta=0.15;
	char Conf=0;
//	char txt[30];
		
	if(Impresor.Motor.Speed<SPEED_50_MMPS||Impresor.Motor.Speed>SPEED_100_MMPS){
		Impresor.Motor.Speed=SPEED_75_MMPS;
		Conf=1;
	}
	
	ReadSensors();
	
	
	if(Impresor.PrinterADC.InputVoltaje>6.9 && Impresor.Motor.Speed!=Impresor.Motor.SpeedJack){
		Impresor.Motor.Speed=Impresor.Motor.SpeedJack;
		Conf=1;
	}
	
	
	if(Impresor.PrinterADC.InputVoltaje<7.5 && Impresor.Motor.Speed!=Impresor.Motor.SpeedBattery && Impresor.CMD.ACTION!=ACTION_PRINT_DEMO_0){
		Impresor.Motor.Speed=Impresor.Motor.SpeedBattery;
		Conf=1;		
	}
	
	/////////////////////////PARA EL DEMO DE IMPRESION//////////////////////////////////////////////
	if(Impresor.PrinterADC.InputVoltaje<7.5&&Impresor.CMD.ACTION==ACTION_PRINT_DEMO_0){
		
		
	/*if(Impresor.PrinterADC.TempPrintHead<1.5 &&Impresor.Motor.Speed!=SPEED_85_MMPS){
		Impresor.Motor.Speed=SPEED_85_MMPS;
		Conf=1;
	}		
	
	if(Impresor.PrinterADC.TempPrintHead>1.53 &&Impresor.Motor.Speed!=SPEED_80_MMPS){
		Impresor.Motor.Speed=SPEED_80_MMPS;
		Conf=1;
	}*/
	
	//if(Impresor.PrinterADC.TempPrintHead>1.8 &&Impresor.Motor.Speed!=SPEED_75_MMPS){
	if(Impresor.Motor.Speed!=SPEED_70_MMPS){
		Impresor.Motor.Speed=SPEED_70_MMPS;
		Conf=1;
	}
		
	}	
	///////////////////////////////////////////////////////////////////////
	
	


	
	if(Conf==1){
		Impresor.Motor.Speed=SPEED_85_MMPS;
		//Impresor.Motor.Speed=SPEED_85_MMPS;
		MX_TIM14_Init(Impresor.Motor.Speed);
		HAL_TIM_Base_Start_IT(&htim14);
		//sprintf(txt,"Max %02d, Min %02d",Impresor.Motor.SpeedJack,Impresor.Motor.SpeedBattery);
		//PrintTextLine(txt,Impresor.Font);
	}	
	
}		
