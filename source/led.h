/*
 * led.h
 *
 *  Created on: Jun 2, 2021
 *      Author: Szymon Kajda
 */

#ifndef _LED_H_
#define _LED_H_

#include "pin_desc.h"

#define LED_ON LED_PORT |= (1<<LED_PIN)
#define LED_OFF LED_PORT &= ~(1<<LED_PIN)
#define LED_TOGGLE LED_PORT ^= (1<<LED_PIN)

#define LED2_ON LED2_PORT |= (1<<LED2_PIN)
#define LED2_OFF LED2_PORT &= ~(1<<LED2_PIN)
#define LED2_TOGGLE LED2_PORT ^= (1<<LED2_PIN)

inline void setupLED( void ){
	LED_DDR |= (1<<LED_PIN);
	LED2_DDR |= (1<<LED2_PIN);
}

#endif /* _LED_H_ */
