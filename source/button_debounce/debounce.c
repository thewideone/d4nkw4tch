/*
 * debounce.c
 *
 *  Created on: Aug 24, 2020
 *      Author: szymon
 */

#include "debounce.h"

// Bit is set to one if a debounced press is detected.
volatile uint8_t buttons_down;
volatile uint8_t buttons_down_state;

// Return non-zero if a button matching mask is pressed.
uint8_t isButtonJustPressed( uint8_t button_mask ){
	// ATOMIC_BLOCK is needed if debounce() is called from within an ISR
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		// And with debounced state for a one if they match
		button_mask &= buttons_down;
		// Clear if there was a match
		buttons_down ^= button_mask;
	}
	// Return non-zero if there was a match
	return button_mask;
}

uint8_t isButtonDown( uint8_t button_mask ){
	// ATOMIC_BLOCK is needed if debounce() is called from within an ISR
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		// And with debounced state for a one if they match
		button_mask &= buttons_down_state;
	}
	// Return non-zero if there was a match
	return button_mask;
}

void debounceInit(void){
	// Button pins as input
	BUTTON_DDR &= ~(BUTTON_MASK);
	// Enable pullup on buttons
	BUTTON_PORT |= BUTTON_MASK;
}
