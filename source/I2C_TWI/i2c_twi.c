/*
 * i2c_twi.c
 *
 *  Created on: 2010-09-07
 *       Autor: Miros�aw Karda�
 */
#include "i2c_twi.h"

#include <avr/io.h>

#define I2C_BITRATE	100
#define I2C_TWBR_BITRATE ((F_CPU/1000l)/I2C_BITRATE)
#if I2C_TWBR_BITRATE >= 16
#define I2C_TWBR_BITRATE (((F_CPU/1000l)/I2C_BITRATE)-16)/2;
//#define I2C_TWBR_BITRATE (I2C_TWBR_BITRATE-16)/2;
#endif

void i2cSetBitrate( void ) {
//	uint8_t bitrate_div;
//
//	bitrate_div = ((F_CPU/1000l)/bitrateKHz);
//	if(bitrate_div >= 16)
//		bitrate_div = (bitrate_div-16)/2;

//	TWBR = bitrate_div;
	TWBR = I2C_TWBR_BITRATE;
}

void TWI_start(void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
//	__asm__ __volatile__(
//			".equ a, 0 \n\t"
//			"lds __tmp_reg__,(1<<a)|(1<<1) \n\t"
//			"out %[_PIND],__tmp_reg__ \n\t"
//			:
//			: [_PIND] "I" (_SFR_IO_ADDR(PORTD))
//	);
//	__asm__ __volatile__ (
//			".equ TWINT, 7 	\n\t"
//			".equ TWEN, 2 	\n\t"
//			".equ TWSTA, 5 	\n\t"
//			"lds __tmp_reg__, (1<<TWINT)|(1<<TWEN)|(1<<TWSTA)	\n\t"
//			"sts %[_TREG],__tmp_reg__							\n\t"
//			:
//			: [_TREG] "M" (_SFR_IO_ADDR(TWCR))
//	);
	while (!(TWCR&(1<<TWINT)));
}

void TWI_stop(void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while ( (TWCR&(1<<TWSTO)) );
}

void TWI_write(uint8_t bajt) {
	TWDR = bajt;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ( !(TWCR&(1<<TWINT)));
}

uint8_t TWI_read(uint8_t ack) {
	TWCR = (1<<TWINT)|(ack<<TWEA)|(1<<TWEN);
	while ( !(TWCR & (1<<TWINT)));
	return TWDR;
}



void TWI_write_buf( uint8_t SLA, uint8_t adr, uint8_t len, uint8_t *buf ) {

	TWI_start();
	TWI_write(SLA);
	TWI_write(adr);
	while (len--) TWI_write(*buf++);
	TWI_stop();
}



void TWI_read_buf(uint8_t SLA, uint8_t adr, uint8_t len, uint8_t *buf) {

	TWI_start();
	TWI_write(SLA);
	TWI_write(adr);
	TWI_start();
	TWI_write(SLA + 1);
	while (len--) *buf++ = TWI_read( len ? ACK : NACK );
	TWI_stop();
}
