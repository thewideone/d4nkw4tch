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
#include "date_time.h"
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
#include "menus/menu_volume.h"
#include "menus/menu_3d_test.h"

typedef enum {menu_main, menu_apps, menu_settings, menu_sound_settings, menu_volume, menu_set_date_and_time, menu_flashlight, menu_bt, menu_bt_terminal, menu_3d_test} TMENU;

extern volatile TMENU menu;// = menu_main in "menus.c"

extern uint8_t menu_cursor_y;
extern uint8_t menu_cursor_x;

void gotoMenu( TMENU new_menu );

#endif
