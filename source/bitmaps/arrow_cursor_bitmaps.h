/*
 * arrow_cursor_bitmaps.h
 *
 *  Created on: Feb 10, 2018
 *      Author: szymon
 */
#include <avr/io.h>
#include <avr/pgmspace.h>

#ifndef _ARROW_CURSOR_BITMAPS_H_
#define _ARROW_CURSOR_BITMAPS_H_

#define ARROW_BMP_WIDTH 8
#define ARROW_BMP_HEIGHT 9
extern const uint8_t arrow_off_left[] PROGMEM;
extern const uint8_t arrow_off_right[] PROGMEM;
extern const uint8_t arrow_on_left[] PROGMEM;
extern const uint8_t arrow_on_right[] PROGMEM;
#define ARROW_UP_DOWN_BMP_WIDTH 9
#define ARROW_UP_DOWN_BMP_HEIGHT 8
extern const uint8_t arrow_off_down[] PROGMEM;
extern const uint8_t arrow_off_up[] PROGMEM;
extern const uint8_t arrow_on_down[] PROGMEM;
extern const uint8_t arrow_on_up[] PROGMEM;

#endif
