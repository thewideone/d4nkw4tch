/*
 * uart.h
 *
 *  Created on: Feb 7, 2018
 *      Author: szymon
 */

#define FOSC 8000000
#define BAUD 9600
#define UBRRVAL FOSC/16/BAUD-1

void uart_init( unsigned int baud );
unsigned char rx_char();
void tx_char( unsigned char c );
void tx_string( char string[]);
void tx_uint8( uint8_t num );
void tx_uint8Bin( uint8_t num );
