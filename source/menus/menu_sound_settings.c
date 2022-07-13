/*
 * menu_sound_settings.c
 *
 *  Created on: Jun 21, 2021
 *      Author: Szymon Kajda
 */

#include "menu_sound_settings.h"
#include "../menus.h"

#include "../power_adc_wdt.h"

//static uint8_t menu_cursor_y = 0;
//static uint8_t menu_cursor_x = 0;

//static uint8_t button_sound = OCR0B;

#define MENU_SOUND_DEBUG

void handleMenuSoundSettings(void){

#ifdef MENU_SOUND_DEBUG
	ssd1306_drawChar( 1, 1, menu_cursor_x+'0', 1, 0, 1 );
	ssd1306_drawChar( 8, 1, menu_cursor_y+'0', 1, 0, 1 );
#endif	/* MENU_DATETIME_DEBUG */

	draw_menu_arrow(120, menu_arrow_anim_dir, 11);
	uint8_t bit_settings = readBitSettings();

	switch( button ){
		case TOP:
			//menu_cursor_y--;
			moveArrowCursor(-1);
			break;
		case BOT:
			//menu_cursor_y++;
			moveArrowCursor(1);
			break;
		case PRESS:
			switch( menu_cursor_y ){
				case 0:
					gotoMenu( menu_volume );
					break;
				case 1:

					toggle_bit( bit_settings, BUTTON_SOUND_BIT );	// toggle button sound type

					if( bit_is_set( bit_settings, BUTTON_SOUND_BIT ) ){
						ssd1306_puts( 85, 23, "click", 1, 1, 0 );
						OCR0B = BUTTON_CLICK_OCR0B_VALUE;
					}
					else{
						ssd1306_puts( 85, 23, "beep ", 1, 1, 0 );
						OCR0B = 250;
					}

					updateBitSettings( bit_settings );
					break;
				case 4:
					// reset cursors
					//menu_cursor_y = 0;		// or maybe use globals?
					//menu_cursor_x = 0;
					gotoMenu( menu_apps );
					break;
			}
			break;
	}
}
