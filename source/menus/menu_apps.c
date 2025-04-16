/*
 * menu_apps.c
 *
 *  Created on: Jun 21, 2021
 *      Author: Szymon Kajda
 */

#include "menu_apps.h"
#include "../menus.h"

static TMENU new_menu = menu_main;

void handleMenuApps( void ){
	draw_menu_arrow(120, menu_arrow_anim_dir, 11);
	switch (button) {
		case BOT:
			moveArrowCursor(1);
			break;
		case TOP:
			moveArrowCursor(-1);
			break;
		case PRESS:
			/* GO TO APP */
			//switch (appCursor) {
			switch ( menu_cursor_y ){
				case 1:
					new_menu = menu_set_date_and_time;
					//gotoMenuSetDateAndTime();
					break;
				case 2:
					new_menu = menu_flashlight;
					//gotoMenuFlashlight();
					break;
				case 3:
					new_menu = menu_settings;
					//gotoMenuSettings();
					break;
				case 4:
					new_menu = menu_bt;
					//gotoMenuBt();
					break;
				case 5:
					new_menu = menu_main;
					//gotoMenuMain();
					break;
			}
			gotoMenu( new_menu );
			break;
	}
}
