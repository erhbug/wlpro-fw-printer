/*******************************************************************************
********************************************************************************
*           FABRICANTES DE BASCULAS TORREY           Agosto 2006               *
********************************************************************************
*                                                                              *
*   Proyecto:            Basculas Serie LSQ                                    *
*   Micro:               M16C/80  M30805FGGP                                   *
*   Nombre de archivo:   rtc.c                                                 *
*   Contenido:           Rutinas para manejo de reloj de tiempo real           *
*                                                                              *
*   Realizado por:       IGNACIO HUERTA                                        *
*                                                                              *
********************************************************************************
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "stm32f0xx_hal.h"
//#include "bsp.h"
//#include "dvr_def.h"
#include "dvr_rtc.h"
#include "bsp_printer.h"
#include <stdlib.h>

/* Variables -----------------------------------------------------------------*/

char SII_Buffer[16];
char rRTC_SR[6];
char RTC_Date[8], RTC_Time[8]; 
struct RTC_Format Rtc;

extern I2C_HandleTypeDef hi2c1;
/* functions -----------------------------------------------------------------*/

/* start code ----------------------------------------------------------------*/


//=======================================================================================
//
//   SII_ISL1208StatusReg:
//--------------------------------------------------------------------------------------------
//   Description:
//      bit 0: RTCF	Real time Clock Fail bit [RTCF]
//					1: Battery Bit [BAT]
//					2: Alarm Bit [ALM]
//					3: === Reserved ===
//					4: Write RTC Enable Bit [WRTC]
//					5: === Reserved ===
//					6: Crystal Oscillator Enable Bit [XTOSCB]
//					7: Auto Reset Enable Bit [ARST]
//=======================================================================================
void SII_ISL1208StatusRegWr(unsigned char r_SRTemp)
{
	//SII_Buffer[0] = 0x07;
	char sr = ISL1208_read8(REG_CTL_SR);
	switch (r_SRTemp)
	{
    case C_WRTC_En:
			sr = sr | 0x10;																				// WRTC=1 enable Write RTC b4
			break;
		case C_WRTC_Dis:
			//sr = sr & 0xC7;																				// WRTC=0 Disable Write RTC b4, b3,5: Reserved
		sr = sr & 0xEF;																				// WRTC=0 Disable Write RTC b4, b3,5: Reserved
			break;
	}	
	ISL1208_write8(REG_CTL_SR, sr);
	//HAL_I2C_Master_Transmit(&hi2c1, C_ISL1208_WAddr, (uint8_t*) SII_Buffer, 2, 0xFF);
}

//=======================================================================================*/
//SIIC_User2Wr()
//=======================================================================================*/
void SIIC_User2Wr(void)
{
	SII_Buffer[0] = 0x13;	
	HAL_I2C_Master_Transmit(&hi2c1, C_ISL1208_WAddr, (uint8_t*) SII_Buffer, 1, 0xFF);
	//ISL1208_write8(REG_USR_USR1, rRTC_SR[0]);
}



void SII_ISL1208ReadREGISTERS(){
	//int day,mon,year,hour,min,sec;
	char Registros[0x15];
	char str[50];
	char a=0;
	char N=0X14;//0X14;
	//if ((HAL_I2C_IsDeviceReady(&hi2c1, C_ISL1208_WAddr, 1, 0xFF)) == HAL_OK) 	{
	//	HAL_I2C_Master_Transmit(&hi2c1, C_ISL1208_WAddr, (uint8_t *)aInitValue, 1, 0xFF);
	//leer hora	
	memset(Registros,0,sizeof(Registros));
	
	//SII_Buffer[0]=5;
Registros[0]=0;
	
	HAL_I2C_Master_Transmit(&hi2c1, C_ISL1208_WAddr, (uint8_t *)Registros, 1, 0xFF);
	
	HAL_I2C_Master_Receive(&hi2c1,C_ISL1208_RAddr,(uint8_t*) Registros,N,0xFF);//read hour	
	
	Print(" \n", Impresor.Font);	
	for(a=0;a<N;a++){
		//itoa (Registros[a],STR2,2);
		sprintf(str,"Reg[%02X] = %02X\n",a,Registros[a]);
		Print(str, Impresor.Font);	
		
	}
	Print(" \n", Impresor.Font);	
	Print(" \n", Impresor.Font);	

}

//=======================================================================================*/
//   SII_ISL1208WriteDate:
//--------------------------------------------------------------------------------------------
//   Description:      Addr./Reg Name/Description /  Range
//	SII_TxData[0]	: SubAddr--------------------------------
//	SII_TxData[1]	:	03	   DT	  Date			  1-31
//	SII_TxData[2]	:	04	   MO	  Month			  1-12
//	SII_TxData[3]	:	05	   YR	  Year			  0-99
//	SII_TxData[4]	:	06	   DW	  Day of the Week 0-6  [Sunday,Monday,......,Saturday]
//=======================================================================================*/
void SII_ISL1208WriteDate(void)
{
  SII_ISL1208StatusRegWr(C_WRTC_En);																						// Enable Write RTC
	SII_Buffer[0] = 0x03;																													// Init TxData
	RTC_ValidDate();																															// Valid Date, Month, Year and DayW	
	HAL_I2C_Master_Transmit(&hi2c1, C_ISL1208_WAddr, (uint8_t*) SII_Buffer, 5, 0xFF);
	SII_ISL1208StatusRegWr(C_WRTC_Dis);																						// Disable Write RTC
}

//=======================================================================================*/
//   SII_ISL1208WriteTime:
//--------------------------------------------------------------------------------------------
//   Description:      Addr./Reg Name/Description /  Range
//	SII_TxData[0]	: SubAddr--------------------------------
//	SII_TxData[1]	:	00	   SC	  Seconds		  0-59
//	SII_TxData[2]	:	01	   Mn	  Minutes		  0-59
//	SII_TxData[3]	:	02	   HR	  Hours			  0-23
//=======================================================================================*/
void SII_ISL1208WriteTime(void)
{
  SII_ISL1208StatusRegWr(C_WRTC_En);																						// Enable Write RTC
	SII_Buffer[0] = 0x00;																													// Init TxData
	RTC_ValidTime();																															// Valid Sec, Min and Hour	
	HAL_I2C_Master_Transmit(&hi2c1, C_ISL1208_WAddr, (uint8_t*) SII_Buffer, 4, 0xFF);	
	SII_ISL1208StatusRegWr(C_WRTC_Dis);																						// Disable Write RTC
}

//=======================================================================================*/
//RTC_ValidDate();				// Valid Date, Month, Year and DayW
//=======================================================================================*/
void RTC_ValidDate(void)
{
	if (Rtc.day >= 32 || Rtc.day == 0) Rtc.day = 1;	      												// Valid Date
  if (Rtc.month >= 13|| Rtc.month == 0) Rtc.month = 1;													// Valid Month	
	if (Rtc.year >= 100) Rtc.year = 0;																						// Valid Year
	if (Rtc.dayw >= 7) Rtc.dayw = 0;  																						// Valid Day of the Week
	
	SII_Buffer[1] = Rtc.day;																											// set date
	SII_Buffer[2] = Rtc.month;																										// set month
	SII_Buffer[3] = Rtc.year;																											// set day
	SII_Buffer[4] = Rtc.dayw;																											// set day of week

	DecToBCD(&SII_Buffer[1]);																											// Date
	DecToBCD(&SII_Buffer[2]);																											// Month
	DecToBCD(&SII_Buffer[3]);																											// Year
	DecToBCD(&SII_Buffer[4]);																											// Day of the Week
}

//=======================================================================================*/
//RTC_ValidTime();				// Valid Sec, Min and Hour
//=======================================================================================*/
void RTC_ValidTime(void)
{  
	if(Rtc.hour >= 60) Rtc.hour = 0;																							// Valid Sec			
	if(Rtc.min >= 60)  Rtc.min = 0;																								// Valid Min
	if(Rtc.sec >= 24)  Rtc.sec = 0;		 																						// Valid Hour

	SII_Buffer[3] = Rtc.hour;																											// set seconds
	SII_Buffer[2] = Rtc.min;																											// set minutes
	SII_Buffer[1] = Rtc.sec;																											// set hour

	DecToBCD(&SII_Buffer[1]);																											// Sec
	DecToBCD(&SII_Buffer[2]);																											// Min
	DecToBCD(&SII_Buffer[3]);																											// Hour

	SII_Buffer[3] |= 0x80;																												// Set MIL bit (24H military format)
}

/*******************************************************************************
Conviete BCD a decimal
*******************************************************************************/
int BCDToDec(char data)
{
	char tmp;
	tmp = data;
	tmp &= 0x0F;
	data >>= 4;
	data *= 10;
	data += tmp;
	return (data);
}

/*******************************************************************************
Conviete decimal a BCD
*******************************************************************************/
void DecToBCD(char *pdata)
{
	char tmp;
	tmp = *pdata;
	tmp /= 10;
	tmp *= 0x10;
	*pdata %= 10;
	*pdata += tmp;
}

char DecToBCD2(uint8_t val)
{
	/*char tmp;
	tmp = pdata;
	tmp /= 10;
	tmp = 0x10;
	pdata %= 10;
	pdata += tmp;
	return pdata;*/
	
	 return ((val/10*16) + (val%10));
}


char ISL1208_read8(uint8_t reg)
{ 
	uint8_t Value;
//	IWDG->KR =  0x0000AAAA;
    //Select the register
    HAL_I2C_Master_Transmit(&hi2c1, C_ISL1208_WAddr, &reg, 1, 0xFF);
 //IWDG->KR =  0x0000AAAA;
    //Read the 8-bit register
     HAL_I2C_Master_Receive(&hi2c1,C_ISL1208_RAddr,&Value,1,0xFF);
	
	
	//sprintf(txt,"Read Reg[%02X]= %02X \n",reg,Value);
	//Print(txt, Impresor.Font);
 
    //Return the byte
    return Value;
}

char ISL1208_write8(char reg, char data){
    //Create a temporary buffer
    uint8_t buff[2];
 
    //Load the register address and 8-bit data
    buff[0] = reg;
    buff[1] = data;
 
    //Write the data
//	IWDG->KR =  0x0000AAAA;
	
	if (HAL_I2C_Master_Transmit(&hi2c1, C_ISL1208_WAddr, buff, 2, 0xFF)==HAL_OK) {
		
	/*sprintf(txt,"Write Reg[%02X]= %02X \n",reg,data);
	Print(txt, Impresor.Font);*/
		
		    return 0;
    } else {
        //Return failure
        return 1;
    }
		
}

bool ISL1208_powerFailed(void)
{
    //Read the 8-bit register value
    char value = ISL1208_read8(REG_CTL_SR);	
 
    //Return the status of the RTCF bit
    if (value & (1 << 0)){
			
			value&=~(1 << 0);
			ISL1208_write8(REG_CTL_SR, value );
			
        return true;
		}
    else
        return false;
}

bool ISL1208_open(void){
	uint8_t reg;
    //Probe for the ISL1208 using a Zero Length Transfer	
    if (HAL_I2C_Master_Transmit(&hi2c1, C_ISL1208_WAddr, &reg, 0, 0xFF)==HAL_OK) {
        //Read the current status register
//        SII_ISL1208StatusRegWr(C_WRTC_En);
			
			  uint8_t sr = ISL1208_read8(REG_CTL_SR);/////borrar		
			
 
        //Disable auto reset for BAT and ALM bits
        //sr &= ~(1 << 7);
			
			//Enable auto reset for BAT and ALM bits
        sr |= (1 << 7);
 
        //Write the new status register
        ISL1208_write8(REG_CTL_SR, sr);
 
        //Return success
        return true;
    } else {
        //Return failure
        return false;
    }
}

unsigned int bcd2bin(unsigned char val)
{
    //return (val & 0x0F) + (val >> 4) * 10;
	return ((val/16*10) + (val%16));
}

uint8_t bin2bcd(unsigned int val)
{
  //  return ((val / 10) << 4) + val % 10;
	return   ((val/10*16) + (val%10));
}



void ISL1208_oscillatorMode(uint8_t mode)
{
    //Read the current 8-bit register value
    char value = ISL1208_read8(REG_CTL_SR);
 
    //Set or clear the XTOSCB bit
    if (mode == OSCILLATOR_EXTERNAL)
        value |= (1 << 6);
    else
        value &= ~(1 << 6);
 
    //Write the value back out
    ISL1208_write8(REG_CTL_SR, value);
}

struct RTC_Format ISL1208_time(){
	struct RTC_Format TIME;
    //Setup a tm structure based on the RTC
    //struct tm timeinfo;
    TIME.sec = bcd2bin(ISL1208_read8(REG_RTC_SC));
    TIME.min = bcd2bin(ISL1208_read8(REG_RTC_MN));
 
    //Make sure we get the proper hour regardless of the mode
    char hours = ISL1208_read8(REG_RTC_HR);
    if (hours & (1 << 7)) {
        //RTC is in 24-hour mode
        TIME.hour = bcd2bin(hours & 0x3F);
    } else {
        //RTC is in 12-hour mode
        TIME.hour = bcd2bin(hours & 0x1F);
 
        //Check for the PM flag
        if (hours & (1 << 5))
            TIME.hour += 12;
    }
 
    //Continue reading the registers
    TIME.day = bcd2bin(ISL1208_read8(REG_RTC_DT));
    TIME.month = bcd2bin(ISL1208_read8(REG_RTC_MO)) ;//-1
    //TIME.year = bcd2bin(ISL1208_read8(REG_RTC_YR)) + 100;
		TIME.year = bcd2bin(ISL1208_read8(REG_RTC_YR))+2000;//ISL1208_read8(REG_RTC_YR);//bcd2bin(ISL1208_read8(REG_RTC_YR)) + 100;
    TIME.dayw = bcd2bin(ISL1208_read8(REG_RTC_DW));
 
    //Return as a timestamp
    return TIME;
}

void ISL1208time(struct RTC_Format DATE){
	if (DATE.day >= 32 || DATE.day == 0) DATE.day = 1;	      		// Valid Date
  if (DATE.month >= 13|| DATE.month == 0) DATE.month = 1;				// Valid Month	
	if (DATE.year <= 2019 || DATE.year >= 2100) DATE.year = 2019;	// Valid Year
	if (DATE.dayw >= 7) DATE.dayw = 0;  													// Valid Day of the Week
	if(Rtc.hour >= 60) DATE.hour = 0;															// Valid Sec			
	if(Rtc.min >= 60)  DATE.min = 0;															// Valid Min
	if(Rtc.sec >= 24)  DATE.sec = 0;		 													// set day of week
	
//Write the current time
	/*sprintf(txt,"SC= %02X\n",DecToBCD2(DATE.sec));Print(txt, Impresor.Font);
	sprintf(txt,"MN= %02X\n",DecToBCD2(DATE.min));Print(txt, Impresor.Font);
	sprintf(txt,"HR= %02X\n",DecToBCD2(DATE.hour));Print(txt, Impresor.Font);
	
	sprintf(txt,"DT= %02X\n",DecToBCD2(DATE.day));Print(txt, Impresor.Font);
	sprintf(txt,"MO= %02X\n",DecToBCD2(DATE.month));Print(txt, Impresor.Font);
	sprintf(txt,"YR= %02X\n",DecToBCD2(DATE.year-2000));Print(txt, Impresor.Font);
	sprintf(txt,"DW= %02X\n",DecToBCD2(DATE.dayw));Print(txt, Impresor.Font);*/
	  
    
		//Write the current time
		SII_ISL1208StatusRegWr(C_WRTC_En);
    ISL1208_write8(REG_RTC_SC, DecToBCD2(DATE.sec));
    ISL1208_write8(REG_RTC_MN, DecToBCD2(DATE.min));
    ISL1208_write8(REG_RTC_HR, DecToBCD2(DATE.hour) | (1 << 7));    //24-hour mode
    ISL1208_write8(REG_RTC_DT, DecToBCD2(DATE.day));
    ISL1208_write8(REG_RTC_MO, DecToBCD2(DATE.month));
		ISL1208_write8(REG_RTC_YR, DecToBCD2(DATE.year-2000));
    ISL1208_write8(REG_RTC_DW, DecToBCD2(DATE.dayw ));
		SII_ISL1208StatusRegWr(C_WRTC_Dis);   
		
}

/*******************************************************************************
inicializa variables
*******************************************************************************/
/*oid RTC_Init(void)		 																												//inicializa variables
{
memset(&Rtc,0,sizeof(RTC));
Rtc.military = 1;
}

bool ISL1208_open(){
    //Probe for the ISL1208 using a Zero Length Transfer
    if (!m_I2C.write(m_ADDR, NULL, 0)) {
        //Read the current status register
        char sr = read8(REG_CTL_SR);
 
        //Disable auto reset for BAT and ALM bits
        sr &= ~(1 << 7);
 
        //Write the new status register
        write8(REG_CTL_SR, sr);
 
        //Return success
        return true;
    } else {
        //Return failure
        return false;
    }
}*/

void ISL1208_RTC_begin() {


  //set the WRTC (Write RTC Enable Bit) bit to 1 to enable the RTC
  //only then the RTC start counting
	ISL1208_write8(REG_CTL_SR, 0x10);
  
}

/* end code ------------------------------------------------------------------*/
