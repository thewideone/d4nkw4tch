/*
 * buzzer.c
 *
 *  Created on: 20 maj 2021
 *      Author: Szymon Kajda
 */


#include "buzzer.h"

static volatile TBUZZER buzzer_state = ON;

TBUZZER buzzerState( void ){
	return buzzer_state;
}

void buzzerSetStateOn( void ){
	buzzer_state = ON;
}

void buzzerSetStateOff( void ){
	buzzer_state = OFF;
}

void buzzerEnable( void ){
	TCCR0A |= (1 << COM0B1);
}

void buzzerDisable( void ){
	TCCR0A &= ~(1 << COM0B1);
}
