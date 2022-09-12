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
#include "../uart_lib/uart.h"
#include "../date_time.h"
#include "../led.h"

typedef enum {NONE, TOP, PRESS, BOT} button_t;

/*
 * The header is included by the one source file that defines the variable and by all the source files that reference the variable
 */
extern uint8_t buttons_hold_mode;	// = 0 on startup, defined in "buttons.c", declared here
extern volatile button_t button;		// defined in "buttons.c", declared here, 1-BOT 2-CLICK 3-TOP
extern uint8_t button_state;		// from "debounce.h"

// WARNING! look out for button_state as I don't really understand "static" keyword
// REPLACE "button_state" with "buttons_down()"

inline void handleButtons( void ){
//	tx_int8Bin( b_state );
//	tx_char('\t');
//	tx_int8Bin( buttons_down );
//	tx_string( "\r\n" );

	if ( (buttons_hold_mode && isButtonDown(BUTTON1_MASK) ) || ( !buttons_hold_mode && isButtonJustPressed(BUTTON1_MASK) ) )
//	if ( (hold_mode && isButtonDown(BUTTON1_MASK) ) || ( !hold_mode && isButtonDown(BUTTON1_MASK) ) )
		button = BOT;
	else if ( (buttons_hold_mode && isButtonDown(BUTTON3_MASK) ) || ( !buttons_hold_mode && isButtonJustPressed(BUTTON3_MASK) ) )
//	else if ( (hold_mode && isButtonDown(BUTTON3_MASK) ) || ( !hold_mode && isButtonDown(BUTTON3_MASK) ) )
		button = TOP;
	else if ( isButtonJustPressed(BUTTON2_MASK) )
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

button_t getButtonState( void );
void buttonForceRelease( void );

void setButtonsHoldOn( void );
void setButtonsHoldOff( void );
uint8_t getButtonsHold( void );	// returns 0 if disabled, non-zero otherwise
void updateButtonsHoldModeDelayed( uint8_t rtc_state_change_cnt );

#endif /* _BUTTONS_H_ */
