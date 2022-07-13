/*
 * misc.c
 *
 *  Created on: 9 May 2022
 *      Author: Szymon Kajda
 */

#include "misc.h"

#include <avr/wdt.h>

void anim_display_off( void ){
	// Frame 1 (all white)
	ssd1306_cls();
	ssd1306_invertDisplay(1);
	ssd1306_display();

//	_delay_ms(DISPLAY_OFF_TIME_SHORT-2);

	ssd1306_invertDisplay(0);

	// Frames 2-11
	for( uint8_t i=3; i<=30; i+=3 ){
		ssd1306_cls();
		ssd1306_drawFillRect( i*2, i, 128-2*i*2, 64-2*i, 1 );
		ssd1306_display();

//		_delay_ms(DISPLAY_OFF_TIME_SHORT-2);
	}

	wdt_reset();

	// Frames 12-13
	ssd1306_cls();
	ssd1306_drawFillRect( 62, 30, 4, 4, 1 );
	ssd1306_display();

	_delay_ms(DISPLAY_OFF_TIME_SHORT*2);

	// Frames 14-21 (horizontal line <>)
	uint8_t x = 57, w = 14;
	for( uint8_t i=0; i<8; i++ ){
		ssd1306_drawFastHLine( x, 31, w, 1 );
		ssd1306_drawFastHLine( x, 32, w, 1 );
		ssd1306_display();

//		_delay_ms(DISPLAY_OFF_TIME_SHORT);

		x-=8;
		w+=16;
	}

//	for( uint8_t i=57, j=14; i>=1; i-=8, j+=16 ){	// i>=1 cuz the last frame has width step = 9
////		ssd1306_cls();
//		ssd1306_drawFastHLine( i, 31, j, 1 );
//		ssd1306_drawFastHLine( i, 32, j, 1 );
//		ssd1306_display();
//
//		_delay_ms(DISPLAY_OFF_TIME_SHORT);
//	}

	wdt_reset();

	// Frame 21 (continuation)
	_delay_ms(DISPLAY_OFF_TIME_LONG-DISPLAY_OFF_TIME_SHORT);

//	LED2_ON;

	// Frames 22-25 (dimming)
	for( uint8_t i=1; i<=4; i++ ){
		ssd1306_cmd( SSD1306_SETCONTRAST );
		ssd1306_cmd(252-i*63);	// floor(255/4)=63, 4*63=252
		_delay_ms(DISPLAY_OFF_TIME_LONG);
	}

	// Frame 26 (horizontal line ><)
	for( uint8_t i=9, j=110; i<=57; i+=8, j-=16 ){	// i>1 cuz the last frame has width step = 9
		ssd1306_cls();
		ssd1306_drawFillRect( 62, 30, 4, 4, 1 );
		ssd1306_drawFastHLine( i, 31, j, 1 );
		ssd1306_drawFastHLine( i, 32, j, 1 );
		ssd1306_display();

//		_delay_ms(DISPLAY_OFF_TIME_SHORT);
	}


	// Frames 33-34
	ssd1306_cls();
	ssd1306_drawFillRect( 62, 30, 4, 4, 1 );
	ssd1306_display();

	_delay_ms(DISPLAY_OFF_TIME_SHORT*2);


	/* Rectangle:
	 * 6, 3		128-2*6 =116, 64-2*3=58
	 * 12, 6	128-2*12=104, 64-2*6=52
	 * 18, 9	92,  46
	 * 24, 12	80,  40
	 * 30, 15
	 * 36, 18
	 * 42, 21
	 * 48, 24
	 * 54, 27
	 * 60, 30
	 *
	 * HLine <>:
	 * 57, 49, 41, 33, ...
	 * HLine <> width:
	 * 14, 30, 46, ...
	 *
	 * HLine ><:
	 * 9, 17, 25, 33, 41, 49, 57
	 * HLine >< width:
	 * 110, 94, 78, 62, 46, 30, ...
	 *
	 */
}
