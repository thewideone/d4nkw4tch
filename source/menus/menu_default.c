/*
 * menu_default.c
 *
 *  Created on: 9 Aug 2022
 *      Author: Szymon Kajda
 */

#include <stdlib.h>

#include "menu_default.h"
#include "../button_debounce/buttons.h"

void handleMenuDefault( TDATETIME * dt, uint8_t wake_up_time, uint8_t stay_up_time ){
	ssd1306_puts( 25, 14, "You shouldn't", 1, 1, 0 );
	ssd1306_puts( 43, 23, "be here", 1, 1, 0 );
	ssd1306_puts( 25, 34, "Entering main", 1, 1, 0 );
	ssd1306_puts( 37, 43, "menu in", 1, 1, 0 );

	uint8_t time_spent_in_menu = abs( dt->ss - wake_up_time );

	ssd1306_put_int( 85, 43, (stay_up_time - time_spent_in_menu), 1, 1, 0 );

	if( time_spent_in_menu == stay_up_time
		|| buttonState() == PRESS)
		gotoMenu( menu_main );
}
