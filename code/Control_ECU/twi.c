/*******************************
 *  twi.c
 *
 *  Created on: Aug 2, 2024
 * 
 *  Author: Muhannad Abdallah
 ******************************/

#include"twi.h"
#include<avr/io.h>
#include"common_macros.h"

void TWI_init(const TWI_ConfigType* Config_Ptr){
	/* Bit Rate: 400.000 kbps using zero pre-scaler TWPS=00 and F_CPU=8MHZ*/
	/* Assume prescaler of 1 */
	TWBR=((F_CPU/Config_Ptr->bit_rate)-16)/2;
	TWSR=0x00;

	/*Two wire Bus address, my address if any master device want to call me in case this MC is slave*/
	/*General call recognition off because bit 0 is 0 */
	TWAR=Config_Ptr->address&0xFE; // my address

	/* Enable TWI*/
	TWCR=(1<<TWEN);

}

void TWI_start(void){
	/*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * Send the start bit by TWSTA=1
	 * Enable TWI module TWEN=1*/


	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);

	/* Wait for TWINT flag to be set in TWCR register (start bit is sent successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

void TWI_stop(void){
	/*
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO =1
	 * Enable TWI module TWEN=1
	 */

	TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void TWI_writeByte(uint8 data){
	/* put data on TWI data register*/
	TWDR=data;

	/* Clear TWINT flag before sending the data TWINT=1
	 * Enable TWI module TWEN =1 */
	TWCR=(1<<TWINT)|(1<<TWEN);

	/* Wait until TWINT flag is set indicating (stop bit is sent successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

uint8 TWI_readByteWithACK(void){
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
	/* Read Data */
	return TWDR;
}

uint8 TWI_readByteWithNACK(void)
{
	/*
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

uint8 TWI_getStatus(void){
	uint8 status;
	/* masking to eliminate first 3 LSB bits and get the other 5 because first 3 is concerned
	 * with prescaling the clock*/
	status=TWSR&0xF8;
	return status;
}


