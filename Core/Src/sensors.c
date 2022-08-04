#include "sensors.h"
extern ADC_HandleTypeDef hadc;

/* Private includes ----------------------------------------------------------*/
	uint32_t tickstart = 0;
	int timeout=20;
	strsensor sensores;
	
void sensor_process_init(void){
	tickstart = HAL_GetTick();	
}

//#define TEMP30_CAL_ADDR ((uint16_t*) ((uint32_t)0x1FFFF7B8)) // = 1767
//#define TEMP110_CAL_ADDR ((uint16_t*) ((uint32_t)0x1FFFF7C2)) // = 1310
void readsensors(void){
	uint8_t j = 0;
	uint8_t iSampleNumber=5;
	
	for(j=0;j<iSampleNumber;j++){		
		
		HAL_ADC_Start(&hadc);		
				
		while(__HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_EOC) == RESET){;}
		sensores.val_paperpresent += HAL_ADC_GetValue(&hadc);			
		
		while(__HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_EOC) == RESET){;}
		sensores.val_thermistor += HAL_ADC_GetValue(&hadc);		
		
		while(__HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_EOC) == RESET){;}
		sensores.val_papersensor += HAL_ADC_GetValue(&hadc);	
		
		while(__HAL_ADC_GET_FLAG(&hadc, ADC_FLAG_EOC) == RESET){;}		
		sensores.val_headersensor += HAL_ADC_GetValue(&hadc);	
		
	}
  sensores.val_headersensor/=(uint32_t)iSampleNumber;
	sensores.val_paperpresent/=(uint32_t)iSampleNumber;
	sensores.val_papersensor/=(uint32_t)iSampleNumber;
	sensores.val_thermistor/=(uint32_t)iSampleNumber;
}	

/*En esta funcion se leen los sensores cada 20ms y se envian por serial*/
void sensor_process(void){
	if((HAL_GetTick()-tickstart) >=  timeout ){			
		tickstart = HAL_GetTick();
		readsensors();
		//Pendiente enviar datos por serial
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
