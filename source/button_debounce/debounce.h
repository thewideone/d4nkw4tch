/*
 * debounce.h
 *
 *  Created on: Aug 24, 2020
 *      Author: Peter Dannegger
 *      Modified by: Szymon Kajda
 */

/*
debounce.h. Modified version of Peter Dannegger�s debounce routines.
Debounce up to eight buttons on one port. $Rev: 577 $
*/
#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "../pin_desc.h"

// Buttons' masks' definitions in "pin_desc.h"
#define BUTTON_MASK (BUTTON1_MASK | BUTTON2_MASK | BUTTON3_MASK)

// Variables to tell that the buttons are pressed (and debounced).
// buttons_down returns non-zero if a change has been detected.
// buttons_down_state returns non-zero as long as a button is pressed.
// Having two variables allows for triggering one-shot actions.
// buttons_down can be read with isButtonJustPressed() which will clear it.
// buttons_down_state can be read with isButtonDown() which will clear it.
extern volatile uint8_t buttons_down;
extern volatile uint8_t buttons_down_state;

// Return non-zero if a button matching mask is pressed.
uint8_t isButtonJustPressed(uint8_t button_mask);
uint8_t isButtonDown( uint8_t button_mask );

// Make button pins inputs and activate internal pullups.
void debounceInit(void);

// Decrease 2 bit vertical counter where mask = 1.
// Set counters to binary 11 where mask = 0.
#define VC_DEC_OR_SET(high, low, mask) \
		low = ~(low & mask); \
		high = low ^ (high & mask)

// Check button state and set bits in the button_down variable if a
// debounced button down press is detected.
// Call this function every 10 ms or so.
static inline void debounce(void){
	// Eight vertical two bit counters for number of equal states
	static uint8_t vcount_low = 0xFF, vcount_high = 0xFF;

	// Keeps track of current (debounced) state
	static uint8_t button_state = 0;

	// Read buttons (active low so invert with ~). Xor with
	// button_state to see which ones are about to change state
	uint8_t state_changed = ~BUTTON_PIN ^ button_state;

	// Decrease counters where state_changed = 1, set the others to 0b11.
	VC_DEC_OR_SET(vcount_high, vcount_low, state_changed);

	// Update state_changed to have a 1 only if the counter overflowed
	state_changed &= vcount_low & vcount_high;

	// Change button_state for the buttons who�s counters rolled over
	button_state ^= state_changed;

	// Update button_down with buttons who�s counters rolled over
	// and who�s state is 1 (pressed)
	buttons_down |= button_state & state_changed;

	// Save current state in a separate variable to allow safe reading
	buttons_down_state = button_state;
}

#endif
