/*
 * ws2812b.h
 *
 *  Created on: May 9, 2021
 *      Author: szymon
 */
#ifndef WS2812B_H
#define WS2812B_H
/*
 *	1 clock cycle @ 8MHz = 125ns
 *
 *	From datasheet:
 *	T0H: 0.4us	-> 375ns (3 cycles)
 *	T0L: 0.85us	-> 875ns (7 cycles)
 *	T1H: 0.8us	-> 750ns (6 cycles)
 *	T1L: 0.45us	-> 500ns (4 cycles)
 *	Reset time: >50us
 *
 *	TH+TL = 1.25us +-600ns
 *
 *	T0: 1250ns
 *	T1: 1250ns
 *
 *	Working code in assembly: C:\Users\szymon\Dropbox\uSD\Programming\Assembler\Atmega328\ws2812b.asm
 *
 *	Based on: https://github.com/lpodkalicki/blog/blob/master/avr/attiny13/036_circular_ws2812_led_chaser/light_ws2812.c
 *
 */

#include <avr/io.h>
#include "../pin_desc.h"

#define RGB_PIN LED_PIN
#define RGB_PORT LED_PORT
void update_LED( uint8_t GRB[] );

inline void setupLED( void ){
	LED_DDR |= (1<<LED_PIN);	// LED
}

#endif /* WS2812B_H */
