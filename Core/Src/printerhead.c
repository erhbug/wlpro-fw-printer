#include "main.h"
#include "printerhead.h"

//char BuferToPrint[SizeOfBuferToPrint];

char BufferToPrinterHead[BYTES_PER_ROW]={0};
	
void send_printhead_byte (uint8_t DataByte){	
	  uint32_t RspODR=0;
		uint32_t BIT_DI= DEF_BIT_14;
		uint32_t BIT_CLK=DEF_BIT_13;
		
	for(uint32_t i=0;i<=7;i++){			
			RspODR=PH_GPIO_Port->ODR;			
			RspODR&=0XBFFF;//(~BIT_DI);
			if(DataByte&0x80)
				RspODR|=BIT_DI;	//DI		
			RspODR|=BIT_CLK; //CLK
			PH_GPIO_Port->ODR=RspODR;			
			DataByte<<=1;				
			PH_GPIO_Port->ODR&=0XDFFF;//(~BIT_CLK);	
		}
	}

void WritePrintHead(void){
	char a =0;	

	for(a=0;a<BYTES_PER_ROW;a++){	
		send_printhead_byte(0X11);//BufferToPrinterHead[a]);
	}		
  //BmpBufferRS232.PrintHeadDataSent=1;		
	HAL_GPIO_WritePin(PH_GPIO_Port, PHLATCH_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PH_GPIO_Port, PHLATCH_Pin, GPIO_PIN_SET);		
	
//	HAL_GPIO_TogglePin(PH_GPIO_Port,PHLATCH_Pin);
}

void OnStrobe0(void){
	HAL_GPIO_WritePin(PH_GPIO_Port, PHSTB1_Pin, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(PH_GPIO_Port, PHSTB2_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(PH_GPIO_Port, PHSTB3_Pin, GPIO_PIN_SET);	
}

void OnStrobe1(void){
	HAL_GPIO_WritePin(PH_GPIO_Port, PHSTB1_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(PH_GPIO_Port, PHSTB2_Pin, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(PH_GPIO_Port, PHSTB3_Pin, GPIO_PIN_SET);
}

void OnStrobe2(void){
	HAL_GPIO_WritePin(PH_GPIO_Port, PHSTB1_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(PH_GPIO_Port, PHSTB2_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(PH_GPIO_Port, PHSTB3_Pin, GPIO_PIN_RESET);
}

void OffAllStrobes(void){
	HAL_GPIO_WritePin(PH_GPIO_Port, PHSTB1_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(PH_GPIO_Port, PHSTB2_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(PH_GPIO_Port, PHSTB3_Pin, GPIO_PIN_SET);	
}

void printerhead_disable(void){
	OffAllStrobes();
	HAL_GPIO_WritePin(PH_GPIO_Port, PHCLK_Pin, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(PH_GPIO_Port, PHDI_Pin, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(PH_GPIO_Port, PHLATCH_Pin, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(PHOE_GPIO_Port, PHOE_Pin, GPIO_PIN_RESET);	
}

void printerhead_enable(void){
	OffAllStrobes();
	HAL_GPIO_WritePin(PH_GPIO_Port, PHCLK_Pin, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(PH_GPIO_Port, PHDI_Pin, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(PH_GPIO_Port, PHLATCH_Pin, GPIO_PIN_SET);	
	
	HAL_GPIO_WritePin(PHOE_GPIO_Port, PHOE_Pin, GPIO_PIN_SET);	
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
