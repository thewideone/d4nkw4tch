/*
 * uart.c
 *
 *  Created on: Feb 7, 2018
 *      Author: szymon
 */

#include <avr/io.h>
#include "uart.h"

void uart_init( unsigned int baud ){
	UBRR0H = (unsigned char)(baud>>8);
	UBRR0L = (unsigned char)baud;
//	__asm__ (
//				"lds __tmp_reg__, lo8(UBRRVAL)	\n\t"
//				"sts %[_TREG],__tmp_reg__							\n\t"
//				:
//				: [_TREG] "M" (_SFR_IO_ADDR(UBRR0L))
//		);
//	__asm__ (
//				"lds __tmp_reg__, hi8(UBRRVAL)	\n\t"
//				"sts %[_TREG2],__tmp_reg__							\n\t"
//				:
//				: [_TREG2] "M" (_SFR_IO_ADDR(UBRR0H))
//		);
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
}

unsigned char rx_char( void ){
	//while(bit_is_clear(UCSR0A,RXC0));
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void tx_char( unsigned char c ){
	//while(bit_is_clear(UCSR0A,UDRE0));
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = c;
}

void tx_string( char string[]){
	int i=0;
	while(string[i] != 0x00){
		tx_char(string[i]);
		i++;
	}
}

void tx_uint8( uint8_t num ){
	uint8_t rem, len = 3;	// 3 digits to print
	char buf[len+1];

	for( uint8_t i=0; i<8; i++ ){
		rem = num % 10;
		num = num / 10;
		buf[len - (i + 1)] = rem + '0';
	}

	buf[len] = '\0';

	tx_string( buf );
}

void tx_uint8Bin( uint8_t num ){
	uint8_t rem, len = 8;	// 8 bits to print
	char buf[len+1];

	for( uint8_t i=0; i<8; i++ ){
		rem = num % 2;
		num = num / 2;
		buf[len - (i + 1)] = rem + '0';
	}

	buf[len] = '\0';

	tx_string( buf );
}
