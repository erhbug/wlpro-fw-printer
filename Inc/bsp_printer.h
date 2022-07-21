#include "stm32f0xx_hal.h"

#ifndef __bspPrinter_h
#define __bspPrinter_h

#include "dvr_rtc.h"

#define START 1u
#define WORK  2u
#define STOP  0u

#define ACELERATE  0
#define NORMAL  1
			
/*#define SPEED_60_MMPS 780
#define SPEED_65_MMPS 832
#define SPEED_70_MMPS 891
#define SPEED_75_MMPS 960
#defineBYTES_PER_ROW SPEED_80_MMPS 1039*/

#define LINES_ARRAY 32

#define SPEED_40_MMPS 1
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
#define SPEED_100_MMPS 13
#define SPEED_105_MMPS 14
#define SPEED_110_MMPS 15


#define SPEED_MAX SPEED_100_MMPS
#define SPEED_MIN SPEED_40_MMPS

//#define         MAX_GROUP_TICKET_PRINT          6                               /* impresion de items continuos sin detener el Driver*/

#define N_STROBES		  	3
#define BYTES_PER_ROW		54//48         //((POINTS_PER_STROBE/8)*N_STROBES)
#define MAX_N_COLS      BYTES_PER_ROW*8
#define MAX_N_ROWS			MAX_GROUP_TICKET_PRINT*20 //todo

#define LF            0x0a              //10
#define VT            0x0b              //11
#define CR            0x0d              //13
#define ESC           0x1b              //27
#define GS            0x1d              //29
#define FF            0x0c              //12
#define BS            0x08              //8
#define TAB           0x09              //9

#define f_8x16dot     0x01
#define f_16x16dot    0x02
#define f_16x24dot    0x03
#define f_24x24dot    0x04
#define f_24x32dot    0x05
#define f_16x32dot    0x06

#define tam_cabezal_mm 48//56
#define num_dots_por_mm 8
#define num_total_dots 384
#define num_bytes_cabezal num_total_dots/num_dots_por_mm

#define largo_espacio_cfg 'C'
#define dist_total_cfg    'D'
#define tempera_cfg       'R'
#define tipo_et_cfg       'c'

#define MAX_LINES_DRV	1024
#define MAX_LINES_RAM	1024

#define FONT8x16  0
#define FONT16x16 1
#define FONT16x24 2
#define FONT16x32 3
#define FONT24x24 4
#define FONT24x32 5

struct strMotor{
	uint16_t NumberOfSteps;	
	uint8_t Status;
	uint8_t SpeedJack;
	uint8_t SpeedBattery;
	uint8_t Speed;
	uint8_t Count; 
	uint8_t Mode;
	uint8_t CountMode;
	uint8_t Activate;
};

#define CMD_NULL 0
#define CMD_FEED 1
#define CMD_DATE 2
#define CMD_CLEAR_BUFFER 3
#define CMD_ASK_SENSOR 4
#define CMD_SPEED 5
#define CMD_NOTIFY 6
#define CMD_DEMO 7

#define ACTION_NULL    0
#define ACTION_START   1
#define ACTION_WORKING 2
#define ACTION_STOP    3
#define ACTION_SET     4
#define ACTION_GET     5
#define ACTION_PRINT   6
#define ACTION_ASK_PAPER 7
#define ACTION_END_OF_PRINT 8
#define ACTION_PRINT_DEMO_0 9

struct strADC{
	float InputVoltaje;
	float TempMCU;
	float TempPrintHead;
	float  PaperOn;
	int CountADC;
};

struct strCMD{
	uint8_t CMD;
	uint8_t ACTION;
};

/*struct strDATE{
	int Year;
	char Month;
	char Day;
	char Hour;
	char Minute;
	char Second;
};*/

struct strImpresor{	
	uint8_t CountStrobe;
	uint8_t CountLine;
	char CaracteresPorLinea;
	char Font;
	uint16_t Indice;
	uint8_t HeaderBufer[LINES_ARRAY][BYTES_PER_ROW];
	uint16_t TimeStrobeON;
	struct strMotor Motor;
	struct strCMD CMD;
	struct strADC PrinterADC;
	struct RTC_Format DATE;
};


extern struct strImpresor Impresor;
extern TIM_HandleTypeDef htim14;

void SpeedMotorMonitor(void);
void InitstrImpresor(void);
void vInterrup_TIM14(void);
void MX_TIM14_Init(uint8_t Speed);
void send_printhead_byte(uint8_t DataByte);
void BSP_Printer_FillZEROsFirstLine(void);
void GetFontParam(char FontType);
void ClearBufferToPrint(void);
char GetCommand(void);
void EjectCMD(void);
void Print(char *txt, char Font);
void ReadSensors(void);
#endif
