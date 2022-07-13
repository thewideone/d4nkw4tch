/*
 * menu_set_date_time.h
 *
 *  Created on: Jun 19, 2021
 *      Author: Szymon Kajda
 */

#ifndef SOURCE_MENUS_MENU_SET_DATE_TIME_H_
#define SOURCE_MENUS_MENU_SET_DATE_TIME_H_

#include <avr/io.h>

#define MENU_DATETIME_DEBUG

void handleMenuSetDateTime(void);
void drawDateToSet( uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f );
void drawTimeToSet( uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f );

#endif /* SOURCE_MENUS_MENU_SET_DATE_TIME_H_ */
