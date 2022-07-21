#include "main.h"
#include "stm32f0xx_hal.h"
#ifndef __printbmp_h
#define __printbmp_h
#include "bsp_printer.h"

#define SizeOfBufer232 10*1024+512
typedef struct{
	uint8_t BufferRS232[SizeOfBufer232];
	uint8_t BufferPrinterHeader[BYTES_PER_ROW];	
	uint8_t *HandleInterrupt;
	uint8_t *HandlePrinter;
	int CountBufferRS232;
	int CountPrinter;
	int  CountPrinterHead;
	char PrintHeadDataSent;
}str_BmpBufferRS232;

extern str_BmpBufferRS232 BmpBufferRS232;

void MainPrinterBmpRS232(void);
void InitStrBmpBufferRS232(void);
#endif
