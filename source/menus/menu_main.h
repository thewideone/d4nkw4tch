/*
 * menu_main.h
 *
 *  Created on: Jun 5, 2021
 *      Author: Szymon Kajda
 */

#ifndef _MENU_MAIN_H_
#define _MENU_MAIN_H_

#include "../DS3231_lib/date_time.h"

void handleMenuMain( TDATETIME * dt, TTEMP * temp );
void handleMenuMainAstronauts( TDATETIME * dt, TTEMP * temp );

#endif /* _MENU_MAIN_H_ */
