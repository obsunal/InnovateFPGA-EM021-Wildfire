/*
  AD7790.h - 
*/
#ifndef AD7790_h
#define AD7790_h

#include "altera_avalon_spi.h"
#include "terasic_includes.h"

/*****************************************************************************/
/************************** ADC Address Definitions **************************/
/*****************************************************************************/

//ADC Write Commands
#define MODE_WRITE			0x10					// Write to the Mode Register
#define FILTER_WRITE		0x20					// Write to the Filter Register

//ADC Read Commands
#define STATUS_READ			0x08					// Read from the Status Register
#define MODE_READ			0x18					// Read from the Mode Register
#define FILTER_READ			0x28					// Read from the Filter Register
#define DATA_READ			0x38					// Read from the Data Register

#define RESET				0xFF					// Resets the chip to default

#define DUMMY_DATA			0xAA

#define AD7790SS 1
#define SS_ACTIVE_7790 0b01
#define SS_RESET_7790 0b11

//Pins
#define AD7790_SS				8					// AD7790 SPI chip select

void writeAd7790 (uint8_t ui8address, uint8_t ui8value);
alt_u16 readAd7790 (uint8_t ui8address);

#endif
