/*
 * ws2812b.c
 *
 *  Created on: May 9, 2021
 *      Author: szymon
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ws2812b.h"

void update_LED( uint8_t GRB[] ){

	uint8_t current_byte;
	uint8_t bit_counter;
	uint8_t RGB_mask_hi = RGB_PORT | (1<<RGB_PIN);
	uint8_t RGB_mask_lo = RGB_PORT | (0<<RGB_PIN);
	uint8_t sreg_prev=SREG;

	cli();
	for( uint8_t i=0; i<3; i++ ){
		current_byte = GRB[i];
		asm volatile(
			"	ldi %[bit_cntr], 8			\n\t	"
			"	loop%=:						\n\t	"
			"		out %[port], %[mask_hi]	\n\t	"
			"		sbrs %[current_byte], 7	\n\t	"
			"		out %[port], %[mask_lo]	\n\t	"
			"		lsl %[current_byte]		\n\t	"
			"		out %[port], %[mask_lo]	\n\t	"
			"		dec %[bit_cntr]			\n\t	"
			"		brne loop%=				\n\t	"
			: [bit_cntr]"=&d" (bit_counter)
			: [current_byte] "r" (current_byte), [port] "I" (_SFR_IO_ADDR(RGB_PORT)),
			  [mask_hi] "r" (RGB_mask_hi), [mask_lo] "r" (RGB_mask_lo)
		);
	}

	SREG = sreg_prev;
}
