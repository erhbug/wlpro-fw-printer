#include "main.h"
#include "dcmotor.h"


void dcmotor_disable(void){
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_OE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_BRAKE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_ENABLE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_PHASE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_MODE_Pin, GPIO_PIN_SET);
}

/*Fordward Fast Current*/
void dcmotor_FWFC(void){
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_OE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_BRAKE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_ENABLE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_PHASE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_MODE_Pin, GPIO_PIN_SET);
}

/*Fordward Slow Current*/
void dcmotor_FWSC(void){
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_OE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_BRAKE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_ENABLE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_PHASE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_MODE_Pin, GPIO_PIN_RESET);
}

/*Reverse Fast Current*/
void dcmotor_RVFC(void){
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_OE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_BRAKE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_ENABLE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_PHASE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_MODE_Pin, GPIO_PIN_SET);
}

/*Reverse Slow Current*/
void dcmotor_RVSC(void){
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_OE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_BRAKE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_ENABLE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_PHASE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(MCD_GPIO_Port, MCD_MODE_Pin, GPIO_PIN_RESET);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
