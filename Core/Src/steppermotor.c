#include "main.h"
#include "steppermotor.h"

extern TIM_HandleTypeDef htim14;

void steppermotor_setspeed(int speed){//INTERRUPCION 1 MS	
	if(speed<0)speed=0;
	if(speed>500)speed=500;
	 
	//speed*=96000.0;//cada mm tiene 8 dots para sacar los dots/s multiplicamos *8
	                 /*para cada dot estamos usando 12 entradas a la interrupcion 14 */	 
	//speed=1.0/speed;/*ahora si sacamos el periodo de cada interrupcion*/
	//speed=speed/(1.0/48000.0);//y lo dividimos entre el tiempo de cada ciclo de 
	                            //reloj conciderando un reloj de 48MHz
	htim14.Init.Period= 480000/(96*speed);// (unsigned int)(speed); 
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
		;
  }
}

void steppermotor_enable(void){
	HAL_GPIO_WritePin(STP_GPIO_Port, STP_STEP_Pin, GPIO_PIN_RESET);		
	HAL_GPIO_WritePin(STP_GPIO_Port, STP_DIR_Pin, GPIO_PIN_RESET);		
	HAL_GPIO_WritePin(STP_GPIO_Port, STP_ENABLE_Pin, GPIO_PIN_RESET);		
}
	
void steppermotor_disable(void){
	HAL_GPIO_WritePin(STP_GPIO_Port, STP_ENABLE_Pin, GPIO_PIN_SET);		
	HAL_GPIO_WritePin(STP_GPIO_Port, STP_STEP_Pin, GPIO_PIN_RESET);		
	HAL_GPIO_WritePin(STP_GPIO_Port, STP_DIR_Pin, GPIO_PIN_RESET);
}

void steppermotor_step(void){
	HAL_GPIO_TogglePin(STP_GPIO_Port,STP_STEP_Pin);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
