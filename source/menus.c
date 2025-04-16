/*
 * menus.c
 *
 *  Created on: 20 maj 2021
 *      Author: Szymon Kajda
 */

#include "menus.h"
#include "bitmaps/pictures.h"	// for astronauts
#include "power_adc_wdt.h"	// for bit settings for astronauts

volatile TMENU menu = menu_main;

uint8_t menu_cursor_y = 0;
uint8_t menu_cursor_x = 0;

void gotoMenu( TMENU new_menu ){
	ssd1306_cls();		// Clear screen
	menu = new_menu;	// Change to new menu

	// Reset globals:
	menu_arrow_pos_y = 0;	// from anims/"menu_pointers.c"
	menu_cursor_y = 0;
	menu_cursor_x = 0;

	uint8_t bit_settings = readBitSettings();

	// Draw new menu:
	switch( new_menu ){
		case menu_main:
			BQ32002_getDateTime( &datetime );
			wakeUpTime = datetime.ss;

			uint8_t bit_setts = readBitSettings();
			if( bit_is_set( bit_setts, ASTRONAUT_MENU_BIT ) ){
				ssd1306_drawBitmap_P( 0, 0, bmp_astronauts, BMP_ASTRONAUTS_W, BMP_ASTRONAUTS_H, 1, 0 );
				ssd1306_puts( 25, 2, "Wait,it's", 1, 1, 0 );
				ssd1306_puts( 84, 31, "?", 1, 1, 0 );	// x was 114
				ssd1306_puts( 68, 45, "Always has", 1, 1, 0 );
				ssd1306_puts( 68, 53, "been...", 1, 1, 0 );
				ssd1306_drawFillRect( 89, 18, 2, 2, 1 );
				ssd1306_drawFillRect( 89, 22, 2, 2, 1 );
//				ssd1306_setPixel( 88, 18, 1 );
//				ssd1306_setPixel( 88, 19, 1 );
//				ssd1306_setPixel( 89, 18, 1 );
//				ssd1306_setPixel( 89, 19, 1 );
//
//				ssd1306_setPixel( 88, 22, 1 );
//				ssd1306_setPixel( 88, 23, 1 );
//				ssd1306_setPixel( 89, 22, 1 );
//				ssd1306_setPixel( 89, 23, 1 );

			}
			break;
		case menu_apps:
			ssd1306_puts( 7, 0, "Game", 1, 1, 0 );
			ssd1306_puts( 7, 11, "Set time & date", 1, 1, 0 );
			ssd1306_puts( 7, 22, "Flashlight", 1, 1, 0 );
			ssd1306_puts( 7, 33, "Settings", 1, 1, 0 );
			ssd1306_puts( 7, 44, "Bluetooth", 1, 1, 0 );
			ssd1306_puts( 2, 55, "Back", 1, 1, 0 );
			break;
		case menu_settings:
			menu_arrow_pos_y = 11;
			ssd1306_puts( 37, 1, "Settings:", 1, 1, 0 );
			ssd1306_puts( 7, 11, "Sound settings", 1, 1, 0 );
			ssd1306_puts( 7, 22, "Invert display", 1, 1, 0 );
			ssd1306_puts( 7, 33, "Power-saving mode", 1, 1, 0 );
			ssd1306_puts( 7, 44, "Astronaut menu:", 1, 1, 0 );
			ssd1306_puts( 2, 55, "Back", 1, 1, 0 );

			if( bit_is_set( bit_settings, ASTRONAUT_MENU_BIT ) )
				ssd1306_puts( 97, 44, "on ", 1, 1, 0 );
			else
				ssd1306_puts( 97, 44, "off", 1, 1, 0 );

			draw_menu_arrow(119, menu_arrow_anim_dir, 11);
			break;
		case menu_sound_settings:
			menu_arrow_pos_y = 11;
			ssd1306_puts( 19, 1, "Sound settings:", 1, 1, 0 );
			ssd1306_puts( 7, 12, "Volume", 1, 1, 0 );
			ssd1306_puts( 7, 23, "Button sound:", 1, 1, 0 );
			//ssd1306_puts( 2, 16, "OCR0B:", 1, 1, 0 );
			ssd1306_puts( 2, 55, "Back", 1, 1, 0 );

			if( bit_is_set( bit_settings, BUTTON_SOUND_BIT ) ){
				ssd1306_puts( 85, 23, "click", 1, 1, 0 );
			}
			else{
				ssd1306_puts( 85, 23, "beep ", 1, 1, 0 );
			}
			draw_menu_arrow(119, menu_arrow_anim_dir, 11);
			break;
		case menu_volume:
			break;
		case menu_flashlight:
			LED_ON;
			LED2_ON;
			ssd1306_invertDisplay(1);
			break;
		case menu_set_date_and_time:
			menu_cursor_y = 5;
			menu_arrow_pos_y = 55 ;
			ssd1306_puts( 31, 2, "Date & time:", 1, 1, 0 );
			ssd1306_puts( 2, 55, "Back", 1, 1, 0 );
			ssd1306_puts( 52, 18, ":", 1, 1, 0 );
			ssd1306_puts( 70, 18, ":", 1, 1, 0 );
			ssd1306_puts( 52, 29, ".", 1, 1, 0 );
			ssd1306_puts( 70, 29, ".", 1, 1, 0 );
			draw_menu_arrow(119, menu_arrow_anim_dir, 11);
			break;
		case menu_bt:
			btCmdCursorReset();
			ssd1306_puts( 7, 0, "Turn on", 1, 1, 0 );
			ssd1306_puts( 7, 11, "Turn off", 1, 1, 0 );
			ssd1306_puts( 7, 22, "Settings", 1, 1, 0 );
			ssd1306_puts( 7, 33, "Terminal", 1, 1, 0 );
			//ssd1306_puts( 7, 44, "", 1, 1, 0 );
			ssd1306_puts( 2, 55, "Back", 1, 1, 0 );
			break;
		case menu_bt_terminal:
			btCmdCursorReset();

			ssd1306_drawFastVLine(63,0,64,1);
			ssd1306_puts( 0, 0, "Test str", 1, 1, 0 );
			ssd1306_puts( 0, 11, "AT+ROLE0", 1, 1, 0 );
			ssd1306_puts( 0, 22, "AT+MODE?", 1, 1, 0 );
			ssd1306_puts( 0, 33, "AT+PASS?", 1, 1, 0 );
			ssd1306_puts( 0, 44, "AT+RESET", 1, 1, 0 );
			ssd1306_puts( 2, 55, "Back", 1, 1, 0 );

			tx_string("AT");
			break;
	}
}
