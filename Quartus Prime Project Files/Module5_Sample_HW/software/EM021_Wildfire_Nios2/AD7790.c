/*
  AD7790.cpp - 
*/

#include "AD7790.h"


volatile int* pio1 = (int*) (PIO_BASE);

uint16_t readAd7790 (uint8_t ui8address)
{
	uint8_t ui8AdcUpperCodes = 0;			// Data register read MSB
	uint8_t ui8AdcLowerCodes = 0;			// Data register read LSB
	uint16_t ui16AdcCodes = 0;

	if (ui8address == DATA_READ)
	{	
		alt_u8 data_send[1] = {ui8address};
		alt_u8 data_recv[2] = {DUMMY_DATA, DUMMY_DATA};
		*pio1 = SS_ACTIVE_7790;
		alt_avalon_spi_command(SPI_BASE, AD7790SS, 1, data_send, 2, data_recv, 0);
		*pio1 = SS_RESET_7790;
		//digitalWrite(AD7790_SS,LOW);
		//SPI.transfer(ui8address);
		//ui8AdcUpperCodes = SPI.transfer(0x00);			//Data register read MSB
		//ui8AdcLowerCodes = SPI.transfer(0x00);			//Data register read LSB
		//digitalWrite(AD7790_SS,HIGH);
		//printf("SPI ADC read: 0x%x 0x%x",data_recv[0], data_recv[1]);
		ui8AdcUpperCodes = data_recv[0];
		ui8AdcLowerCodes = data_recv[1];
		ui16AdcCodes = (unsigned int)((unsigned int)(ui8AdcUpperCodes << 8) | ui8AdcLowerCodes);
		
		/*Serial.print("ADC Data Register Read : ");  //Debug serial prints
		Serial.println(ui16AdcCodes,2);*/
		
		/*Serial.print("ADC Upper 8 Bits : ");		//Debug serial prints
		Serial.println(ui8AdcUpperCodes,2);
		Serial.print("ADC Lower 8 Bits : ");  
		Serial.println(ui8AdcLowerCodes,2);*/
	}
	else
	{
		alt_u8 data_send[1] = {ui8address};
		alt_u8 data_recv[1] = {DUMMY_DATA};
		*pio1 = SS_ACTIVE_7790;
		alt_avalon_spi_command(SPI_BASE, AD7790SS, 1, data_send, 1, data_recv, 0);
		*pio1 = SS_RESET_7790;
		//digitalWrite(AD7790_SS,LOW);
		//SPI.transfer(ui8address);
		//ui8AdcLowerCodes = SPI.transfer(0x00);			// register read
		//digitalWrite(AD7790_SS,HIGH);
		ui8AdcLowerCodes = data_recv[0];
		ui16AdcCodes = ((long)ui8AdcUpperCodes << 8) | ui8AdcLowerCodes;
		
		/*Serial.print("ADC Register Being Read: ");		//Debug serial prints
		Serial.println(ui8address);
		Serial.print(" Reading Register Value : ");
		Serial.println(ui16AdcCodes);*/
	}

	return ui16AdcCodes;
}

void writeAd7790 (uint8_t ui8address, uint8_t ui8value)
{
	
	if (ui8address != RESET)
	{
		alt_u8 data_send[2] = {ui8address, ui8value};
		alt_u8 data_recv[1]= {DUMMY_DATA};
		*pio1 = SS_ACTIVE_7790;
		alt_avalon_spi_command(SPI_BASE, AD7790SS, 2, data_send, 0, data_recv, 0);
		*pio1 = SS_RESET_7790;
		//digitalWrite(AD7790_SS,LOW);
		//SPI.transfer(ui8address);
		//SPI.transfer(ui8value);
		//digitalWrite(AD7790_SS,HIGH);
		//Serial.println("Write Command");		//Debug serial prints
	}
	else
	{
		alt_u8 data_send[4] = {ui8value, ui8value, ui8value, ui8value};
		alt_u8 data_recv[1]= {DUMMY_DATA};
		*pio1 = SS_ACTIVE_7790;
		alt_avalon_spi_command(SPI_BASE, AD7790SS, 4, data_send, 0, data_recv, 0);
		*pio1 = SS_RESET_7790;
		//digitalWrite(AD7790_SS,LOW);
		//SPI.transfer(ui8value);
		//SPI.transfer(ui8value);
		//SPI.transfer(ui8value);
		//SPI.transfer(ui8value);
		//digitalWrite(AD7790_SS,HIGH);
		//Serial.println("Reset Command");			//Debug serial prints
	}
	
	/*Serial.print("ADC Register ");		//Debug serial prints
	Serial.print(ui8address);
	Serial.println("Written");
	Serial.print("With Register Value ");
	Serial.println(ui8value);*/
}
