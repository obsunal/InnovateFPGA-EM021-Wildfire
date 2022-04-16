/* AD5270.c
 *
 *  Created on: May 27, 2015
 *      Author: BBUSHEY
 */


#include "AD5270.h"

volatile int* pio = (int*) (PIO_BASE);

void writeAd5270 (uint8_t ui8command, uint16_t ui16value)
{
	uint8_t ui8DacUpperCodes = 0;			// Data register read MSB
	uint8_t ui8DacLowerCodes = 0;			// Data register read MSB
	uint16_t ui16DacCodes = 0;

	if (ui8command == 1)
	{
		ui16DacCodes = (ui8command << 10);
		ui16DacCodes = (ui16DacCodes | (ui16value));
		ui8DacLowerCodes = ui16DacCodes & 0xFF;
		ui8DacUpperCodes = (ui16DacCodes >> 8) & 0xFF;
	}
	if (ui8command == 5 || ui8command == 7 || ui8command == 9)
	{
		ui8DacLowerCodes = ui16value & 0xFF;
		ui8DacUpperCodes = (ui8command  << 2);
	}
	if (ui8command == 0xF0)			//SDO High Z prep
	{
		ui8DacLowerCodes = ui16value & 0xFF;
		ui8DacUpperCodes = (ui16value >> 8) & 0xFF;
	}
	if (ui8command == 0x0F)			// SDO High Z
	{
		ui8DacLowerCodes = ui16value & 0xFF;
		ui8DacUpperCodes = (ui16value >> 8) & 0xFF;
	}
	//digitalWrite(AD5270_SS,LOW);
	alt_u8 data_send[2] = {ui8DacUpperCodes, ui8DacLowerCodes};
	alt_u8 data_recv[1]= {DUMMY_DATA};
	*pio = SS_ACTIVE_5270;
	alt_avalon_spi_command(SPI_BASE, AD5270SS, 2, data_send, 0, data_recv, 0);

	*pio = SS_RESET_5270;
	//SPI.transfer(ui8DacUpperCodes);
	//SPI.transfer(ui8DacLowerCodes);
	//digitalWrite(AD5270_SS,HIGH);
	
	/*Serial.print("Digipot Resistor Code = ");			//Debug serial prints
    Serial.println(ui16DacCodes);
	Serial.print("Digipot Upper 8 Bits = ");
    Serial.println(ui8DacUpperCodes);
	Serial.print("Digipot Lower 8 Bits = ");
    Serial.println(ui8DacLowerCodes);*/
}

alt_u16 readAd5270 (uint8_t ui8command)
{
	uint8_t ui8DacUpperCodes = 0;			// Data register read MSB
	uint8_t ui8DacLowerCodes = 0;			// Data register read LSB
	uint8_t ui8DacReadUpper = 0;
	uint8_t ui8DacReadLower = 0;
	uint16_t ui16DacRead = 0;

	if ((ui8command != 1) || (ui8command != 5) || (ui8command != 7) || (ui8command != 9))
	{
		ui8DacLowerCodes = 0;
		ui8DacUpperCodes = (ui8command << 2);
		alt_u8 data_send[2] = {ui8DacUpperCodes, ui8DacLowerCodes};
		alt_u8 data_recv[2] = {DUMMY_DATA, DUMMY_DATA};
		*pio = SS_ACTIVE_5270;
		alt_avalon_spi_command(SPI_BASE, AD5270SS, 2, data_send, 0, data_recv, 0);
		*pio = SS_RESET_5270;
		ui8DacReadUpper = data_recv[0];
		ui8DacReadLower = data_recv[1];
		//digitalWrite(AD5270_SS,LOW);
		//SPI.transfer(ui8DacUpperCodes);
		//SPI.transfer(ui8DacLowerCodes);
		//ui8DacReadUpper = SPI.transfer(0x00);			//Data register read MSB
		//ui8DacReadLower = SPI.transfer(0x00);			//Data register read LSB
		//digitalWrite(AD5270_SS,HIGH);
		
		ui16DacRead = (unsigned int)(((unsigned int)(ui8DacReadUpper << 8) | ui8DacReadLower) & 0x03FF);

	}
	
	return ui16DacRead;
}
