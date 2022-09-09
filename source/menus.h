/*
 * menus.c
 *
 *  Created on: May 19, 2021
 *      Author: Szymon Kajda
 */
#ifndef MENUS_H
#define MENUS_H

#include <avr/io.h>

#include "button_debounce/buttons.h"
#include "oled_lib/ssd1306.h"
#include "DS3231_lib/ds3231.h"
#include "DS3231_lib/date_time.h"
#include "bluetooth/bluetooth.h"
#include "led.h"

#include "anims/menu_pointers.h"
#include "bitmaps/arrow_cursor_bitmaps.h"
#include "power_adc_wdt.h"
#include "buzzer.h"

#include "menus/menu_main.h"
#include "menus/menu_bt.h"
#include "menus/menu_set_date_time.h"
#include "menus/menu_settings.h"
#include "menus/menu_sound_settings.h"
#include "menus/menu_apps.h"
#include "menus/menu_default.h"

typedef enum {menu_main, menu_apps, menu_settings, menu_sound_settings, menu_volume, menu_set_date_and_time, menu_flashlight, menu_bt, menu_bt_terminal} TMENU;

extern volatile TMENU menu;// = menu_main in "menus.c"

//volatile uint8_t appCursor;
//uint8_t change_value;
//uint8_t change_pos;
//volatile int8_t menuYPos;
extern uint8_t menu_cursor_y;
extern uint8_t menu_cursor_x;

void gotoMenu( TMENU new_menu );
/*
void gotoMenuMain( void );
void gotoMenuApps( void );
void gotoMenuSoundSettings( void );
void gotoMenuVolume( void );
void gotoMenuFlashlight( void );
void gotoMenuSetDateAndTime( void );
void gotoMenuSettings( void );
void gotoMenuBt( void );
void gotoMenuBtTerminal( void );
*/

#endif
