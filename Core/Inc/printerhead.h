//Designed for Eriberto Romero
#ifndef __PRINTERHEAD_H
#define __PRINTERHEAD_H

#include "stdint.h"

#define DEF_SET 1
#define DEF_CLR 0

#define  DEF_BIT_00                                     0x01u
#define  DEF_BIT_01                                     0x02u
#define  DEF_BIT_02                                     0x04u
#define  DEF_BIT_03                                     0x08u
#define  DEF_BIT_04                                     0x10u
#define  DEF_BIT_05                                     0x20u
#define  DEF_BIT_06                                     0x40u
#define  DEF_BIT_07                                     0x80u

#define  DEF_BIT_08                                   0x0100u
#define  DEF_BIT_09                                   0x0200u
#define  DEF_BIT_10                                   0x0400u
#define  DEF_BIT_11                                   0x0800u
#define  DEF_BIT_12                                   0x1000u
#define  DEF_BIT_13                                   0x2000u
#define  DEF_BIT_14                                   0x4000u
#define  DEF_BIT_15                                   0x8000u

#define BYTES_PER_ROW		56 //((POINTS_PER_STROBE/8)*N_STROBES)

//#define SizeOfBuferToPrint 512//121//64

void OnStrobe0(void);
void OnStrobe1(void);
void OnStrobe2(void);
void OffAllStrobes(void);
void printerhead_disable(void);
void printerhead_enable(void);
void send_printhead_byte(uint8_t DataByte);
void WritePrintHead(void);

#endif /*  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
