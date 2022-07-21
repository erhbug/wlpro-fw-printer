//helo
#include "main.h"

str_BmpBufferRS232 BmpBufferRS232;
//char PrintHeadDataSent=0;

	
//https://javl.github.io/image2cpp/
//Image432x20
void MainPrinterBmpRS232(void){
	int suma=0;
	int velocidad=SPEED_85_MMPS;
	uint32_t tickstart;
	InitstrImpresor();
	InitStrBmpBufferRS232();
	Impresor.Motor.NumberOfSteps=240;
	Impresor.Motor.Status=START;	
	tickstart = HAL_GetTick(); 	
	while(Impresor.Motor.Status!=STOP){
		tickstart = HAL_GetTick();
		while((HAL_GetTick() - tickstart) < 1 && Impresor.Motor.Status!=STOP )
    IWDG->KR =  IWDG_KEY_REFRESH;
	}
	
	while(1){
		IWDG->KR =  IWDG_KEY_REFRESH;	
		if(BmpBufferRS232.CountBufferRS232!=BmpBufferRS232.CountPrinter){
			BmpBufferRS232.CountPrinter++;
			if(BmpBufferRS232.CountPrinter>=SizeOfBufer232)
			  BmpBufferRS232.CountPrinter=0;			
			BmpBufferRS232.BufferPrinterHeader[BmpBufferRS232.CountPrinterHead]=BmpBufferRS232.BufferRS232[BmpBufferRS232.CountPrinter];
			suma+=BmpBufferRS232.BufferPrinterHeader[BmpBufferRS232.CountPrinterHead];
			BmpBufferRS232.CountPrinterHead++;
			if(BmpBufferRS232.CountPrinterHead>=BYTES_PER_ROW){				
				BmpBufferRS232.CountPrinterHead=0;
				BmpBufferRS232.PrintHeadDataSent=0;		 
				Impresor.Motor.NumberOfSteps++;		
				
				/*Si es linea en blanco cambiar velocidad*/ //105/85 230400
				if (suma==0 )velocidad=SPEED_110_MMPS;
				if (suma!=0 )velocidad=SPEED_85_MMPS;									
				if(velocidad!=Impresor.Motor.Speed && Impresor.Motor.Count!=5 && suma==0){					
					tickstart = HAL_GetTick();
		      while( (HAL_GetTick() - tickstart)<200 && Impresor.Motor.Count!=5 && Impresor.Motor.Status!=STOP ){
			      IWDG->KR =  IWDG_KEY_REFRESH;						
		      } 			        
		    }
				suma=0;	
				if(velocidad!=Impresor.Motor.Speed){
					Impresor.Motor.Speed=velocidad;
	        MX_TIM14_Init(Impresor.Motor.Speed);
	        HAL_TIM_Base_Start_IT(&htim14);	
				}
				/***********************/				

				if(Impresor.Motor.Status==STOP)
				  Impresor.Motor.Status=START;		
				
			/*Esperar hasta que la linea se haya enviado al cabezal*/
				tickstart = HAL_GetTick();
		      while( (HAL_GetTick() - tickstart)<106 && BmpBufferRS232.PrintHeadDataSent==0 && Impresor.Motor.Status!=STOP ){
			      IWDG->KR =  IWDG_KEY_REFRESH;						
		      } 
      /***********************/				
			}			
		}	
	}
}

void InitStrBmpBufferRS232(void){
	for(int c=0;c<SizeOfBufer232;c++)
		BmpBufferRS232.BufferRS232[c]=1;
	BmpBufferRS232.CountBufferRS232=-1;
	BmpBufferRS232.CountPrinter=-1;
	BmpBufferRS232.CountPrinterHead=0;
	BmpBufferRS232.PrintHeadDataSent=0;
	//BmpBufferRS232.HandleInterrupt=BmpBufferRS232.BufferRS232;
	//BmpBufferRS232.HandlePrinter=BmpBufferRS232.BufferRS232;	
}
