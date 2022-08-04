//Designed for Eriberto Romero
#ifndef __DCMOTOR_H
#define __DCMOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

typedef struct strsensor{
	uint32_t val_paperpresent;
  uint32_t val_thermistor;
	uint32_t val_papersensor;
  uint32_t val_headersensor;
}strsensor;

void sensor_process_init(void);
void readsensors(void);
void sensor_process(void);


#ifdef __cplusplus
}
#endif

#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
