/*********************************************************************
RTC.h
*********************************************************************/

#ifndef __DVR_RTC
#define __DVR_RTC
#include <stdio.h>
// Constant Definition
//--------------------------------------------------------------
#define C_ISL1208_WAddr   0xDE	 		// RTC I2C Write address
#define C_ISL1208_RAddr   0xDF	 		// RTC I2C Read address
#define C_WRTC_Dis		0							// Disable Write RTC
#define C_WRTC_En		1								// Enable Write RTC

//variables
struct RTC_Format{ 
	int day;
	int month;
	int year;
	int dayw;
	int hour;
	int min;
	int sec;
	int military;																											//1 formato militar de 24h
	int mdy;																													//1 formato mes-dia-año
};

extern struct RTC_Format Rtc;

enum OscillatorMode {
        OSCILLATOR_CRYSTAL=0x00,  /**< The internal crystal oscillator is enabled */
        OSCILLATOR_EXTERNAL  /**< The internal crystal oscillator is disabled, an external 32kHz oscillator is connected to X1 */
    };

enum Register {
        REG_RTC_SC  = 0x00,
        REG_RTC_MN,
        REG_RTC_HR,
        REG_RTC_DT,
        REG_RTC_MO,
        REG_RTC_YR,
        REG_RTC_DW,
        REG_CTL_SR,
        REG_CTL_INT,
        REG_CTL_ATR = 0x0A,
        REG_CTL_DTR,
        REG_ALM_SCA,
        REG_ALM_MNA,
        REG_ALM_HRA,
        REG_ALM_DTA,
        REG_ALM_MOA,
        REG_ALM_DWA,
        REG_USR_USR1,
        REG_USR_USR2
    };
// functions
//--------------------------------------------------------------
void SII_ISL1208WriteDate(void);																		//escribe fecha
void SII_ISL1208WriteHour(void);																		//escribe hora
void SII_ISL1208ReadTime(struct RTC_Format *DATE);//void SII_ISL1208ReadTime(char*);																		//lee hora

void RTC_ValidDate(void);																						//valida fecha
void RTC_ValidTime(void);																						//valida hora
void RTC_Init(void);																								//inicializa variables
int  BCDToDec(char);																								//convierte un byte BCD a decimal
void DecToBCD(char*);																								//convierte un byte decimal a BCD
bool ISL1208_powerFailed(void);
bool ISL1208_open(void);
void ISL1208_oscillatorMode(uint8_t mode);
void ISL1208time(struct RTC_Format DATE);
struct RTC_Format ISL1208_time(void);
void SII_ISL1208ReadREGISTERS(void);
char ISL1208_write8(char reg, char data);
char ISL1208_read8(uint8_t reg);
	#endif

/*********************************************************************
RTC.h
*********************************************************************/
