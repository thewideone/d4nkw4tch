/*
 * buttons.c
 *
 *  Created on: 23 maj 2021
 *      Author: Szymon Kajda
 */

#include "buttons.h"

uint8_t buttons_hold_mode=0;

volatile button_t button = NONE;

button_t getButtonState( void ){
	return button;
}

void buttonForceRelease( void ){
	button = NONE;
}

void setButtonsHoldOn( void ){
	buttons_hold_mode = 1;
}

void setButtonsHoldOff( void ){
	buttons_hold_mode = 0;
}

uint8_t getButtonsHold( void ){
	return buttons_hold_mode;
}

void updateButtonsHoldModeDelayed( uint8_t rtc_state_change_cnt ){
	static uint8_t btn_hold_delay_cntr;	// initialized to 0

	// If button is pressed, count RTC output "toggles"
	if( isButtonDown( BUTTON1_MASK ) ||
		isButtonDown( BUTTON2_MASK ) ||
		isButtonDown( BUTTON3_MASK ) ){
		if( sqwStateChanged() )
			btn_hold_delay_cntr++;
	}
	// Else, if this count > 0, reset the counter
	// and disable buttons hold mode if it's enabled
	else{
		btn_hold_delay_cntr = 0;

		if( getButtonsHold() )
			setButtonsHoldOff();
	}
	// If the button is still pressed after some time,
	// enable buttons hold mode
	if( btn_hold_delay_cntr == rtc_state_change_cnt ){
		setButtonsHoldOn();
		// Just in case function wouldn't be called
		// after a button release
		btn_hold_delay_cntr = 0;
	}
}

ISR(TIMER2_OVF_vect){
	debounce();
}
