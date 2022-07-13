/*
 * buzzer.h
 *
 *  Created on: 20 maj 2021
 *      Author: Szymon Kajda
 */

#ifndef BUZZER_H
#define BUZZER_H

#include <avr/io.h>
#include "pin_desc.h"

#define BUTTON_CLICK_OCR0B_VALUE 255

typedef enum {ON, OFF} TBUZZER;

static volatile TBUZZER buzzer_state;// = ON;	// defined in "buzzer.c", declared here

// was static, why static?
inline void setupBuzzer( void ){
	BUZZER_DDR |= (1<<BUZZER_PIN);	// BUZZER_PIN as output
	// SETUP TIMER0 for buzzer
	OCR0B = 250;					// startup frequency?
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	TCCR0B |= (1 << CS01);
	TCCR0A &= ~(1 << COM0B1);		// DISABLE BUZZER
}

TBUZZER buzzerState( void );
void buzzerSetStateOn( void );
void buzzerSetStateOff( void );
void buzzerEnable( void );
void buzzerDisable( void );

#endif	/* BUZZER_H */
