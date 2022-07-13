/*
 * misc.h
 *
 *  Created on: 9 May 2022
 *      Author: Szymon Kajda
 */

#ifndef SOURCE_ANIMS_MISC_H_
#define SOURCE_ANIMS_MISC_H_

#include <util/delay.h>

#include "../oled_lib/ssd1306.h"
#include "../led.h"

#define DISPLAY_OFF_TIME_SHORT 1	// in milliseconds
#define DISPLAY_OFF_TIME_LONG 98	// in milliseconds

void anim_display_off( void );

#endif /* SOURCE_ANIMS_MISC_H_ */
