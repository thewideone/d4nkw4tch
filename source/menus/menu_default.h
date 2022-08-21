/*
 * menu_default.h
 *
 *  Created on: 9 Aug 2022
 *      Author: Szymon Kajda
 */

#ifndef SOURCE_MENUS_MENU_DEFAULT_H_
#define SOURCE_MENUS_MENU_DEFAULT_H_

#include "../DS3231_lib/date_time.h"
#include "../menus.h"

void handleMenuDefault( TDATETIME * dt, uint8_t wake_up_time, uint8_t stayUpTime );

#endif /* SOURCE_MENUS_MENU_DEFAULT_H_ */
