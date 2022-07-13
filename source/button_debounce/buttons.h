/*
 * buttons.h
 *
 *  Created on: 23 maj 2021
 *      Author: Szymon Kajda
 */

/*
 * This file contains all definitions and functions which can be used by other files / functions of smart watch code,
 * as well as ISR(TIMER2_OVF_vect) used by "debounce.h".
 * The "hardware" part handled in "debounce.h" and "debounce.c"
 */

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include "debounce.h"

typedef enum {NONE, TOP, PRESS, BOT} TBUTTON;

/*
 * The header is included by the one source file that defines the variable and by all the source files that reference the variable
 */
extern uint8_t buttons_hold_mode;	// = 0 on startup, defined in "buttons.c", declared here
extern volatile TBUTTON button;		// defined in "buttons.c", declared here, 1-BOT 2-CLICK 3-TOP
extern uint8_t button_state;		// from "debounce.h"

// WARNING! look out for button_state as I don't really understand "static" keyword
// REPLACE "button_state" with "buttons_down()"

inline void handleButtons( uint8_t hold_mode ){
	if ( (hold_mode && (button_state & BUTTON1_MASK) ) || ( !hold_mode && button_down(BUTTON1_MASK) ) )
		button = BOT;
	else if ( (hold_mode && (button_state & BUTTON3_MASK) ) || ( !hold_mode && button_down(BUTTON3_MASK) ) )
		button = TOP;
	else if ( button_down(BUTTON2_MASK) )
		button = PRESS;
	else
		button = NONE;
}

inline void setupButtons( void ){
	DDRC &= ~(1<<PC3) | ~(1<<PC2) | ~(1<<PC1);	// BUTTONS
	PORTC |= (1<<PC3) | (1<<PC2) | (1<<PC1);	// Enable button pull-ups
	// SETUP TIMER2 for button polls
	TCCR2B |= (1 << CS22) | (1 << CS21);// | (1 << CS20);
	TIMSK2 = 1<<TOIE2;
}

TBUTTON buttonState( void );
void buttonForceRelease( void );

#endif /* _BUTTONS_H_ */
