/*
 * buttons.c
 *
 *  Created on: 23 maj 2021
 *      Author: Szymon Kajda
 */

#include "buttons.h"

uint8_t buttons_hold_mode=0;

volatile TBUTTON button = NONE;

TBUTTON buttonState( void ){
	return button;
}

void buttonForceRelease( void ){
	button = NONE;
}

ISR(TIMER2_OVF_vect){
	debounce();
}
