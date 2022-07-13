/*
 * menu_settings.c
 *
 *  Created on: Jun 21, 2021
 *      Author: Szymon Kajda
 */

#include "menu_settings.h"
#include "../menus.h"

#include "../power_adc_wdt.h"

void handleMenuSettings( uint8_t * bit_settings ){
	draw_menu_arrow(120, menu_arrow_anim_dir, 11);

	switch (button) {
		case BOT:
			moveArrowCursor(1);
			break;
		case TOP:
			moveArrowCursor(-1);
			break;
		case PRESS:
			/* GO TO SETTING */
			//switch (appCursor) {
			switch( menu_cursor_y ){
				case 0:
					gotoMenu( menu_sound_settings );
					break;
				case 1:
					/* INVERT DISPLAY */
					cli();
					*bit_settings ^= (1 << INV_DISPLAY_BIT);
					updateBitSettings(*bit_settings);
					sei();
					ssd1306_invertDisplay(!(*bit_settings & (1 << INV_DISPLAY_BIT)));
					break;
				case 2:
					/* POWER SAVING MODE */
					cli();
					*bit_settings ^= (1 << PWR_SAVING_MODE_BIT);
					updateBitSettings(*bit_settings);
					sei();
					handlePowerSettings(*bit_settings);
					break;
				case 3:
					/* ASTRONAUT MENU */
					cli();
					*bit_settings ^= (1 << ASTRONAUT_MENU_BIT);
					updateBitSettings(*bit_settings);
					sei();
					break;
				case 4:
					gotoMenu( menu_main );
					break;
			}
			break;
	}
}
