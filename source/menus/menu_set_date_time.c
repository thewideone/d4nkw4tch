/*
 * menu_set_date_time.c
 *
 *  Created on: Jun 19, 2021
 *      Author: Szymon Kajda
 */

#include "menu_set_date_time.h"
#include "../menus.h"

// Moved as globals to "menus.h"
//static uint8_t menu_cursor_y = 0;
//static uint8_t menu_cursor_x = 0;

static uint8_t temp_hh;
static uint8_t temp_mm;
static uint8_t temp_ss;
static uint8_t temp_year;
static uint8_t temp_month;
static uint8_t temp_day;
static uint8_t temp_day_of_week;

void handleMenuSetDateTime( void ){

#ifdef MENU_DATETIME_DEBUG
	ssd1306_drawChar( 1, 9, menu_cursor_x+'0', 1, 0, 1 );
	ssd1306_drawChar( 8, 9, menu_cursor_y+'0', 1, 0, 1 );
#endif	/* MENU_DATETIME_DEBUG */

	//ssd1306_drawBitmap_P(119, menu_arrow_pos_y, arrow_on_left, ARROW_BMP_WIDTH, ARROW_BMP_HEIGHT, 1, 0);
	draw_menu_arrow(119, menu_arrow_anim_dir, 11);

	if( menu_cursor_x == 0 ){	// DEFAULT ( user's not changing any value )
		switch (button) {
			case BOT:
				//menu_cursor_y--;
				//moveArrowCursorInv(-1);
				moveArrowCursor(1);
				break;
			case TOP:
				//menu_cursor_y++;
				//moveArrowCursorInv(1);
				moveArrowCursor(-1);
				break;
		}
		if( sqwStateChanged() ){
			ssd1306_put_int_zeroes(40, 18, datetime.hh, 2, 1, 1, 0);
			ssd1306_put_int_zeroes(58, 18, datetime.mm, 2, 1, 1, 0);
			ssd1306_put_int_zeroes(76, 18, datetime.ss, 2, 1, 1, 0);
			ssd1306_put_int_zeroes(40, 29, datetime.day, 2, 1, 1, 0);
			ssd1306_put_int_zeroes(58, 29, datetime.month, 2, 1, 1, 0);
			ssd1306_put_int_zeroes(76, 29, datetime.year, 2, 1, 1, 0);
			ssd1306_puts(40, 40, days[datetime.dayofweek], 1, 1, 0);
		}
	}

	if( buttonState() == PRESS ){
		switch( menu_cursor_y ){
			case 5:
				gotoMenu( menu_apps );	// go back
				break;
			case 4:
			case 3:
			case 2:
				menu_cursor_x++;

				if( menu_cursor_x == 1 ){
					temp_hh = datetime.hh;
					temp_mm = datetime.mm;
					temp_ss = datetime.ss;
					temp_year = datetime.year;
					temp_month = datetime.month;
					temp_day = datetime.day;
					temp_day_of_week = datetime.dayofweek;
				}
				break;
		}
	}

	switch ( menu_cursor_y ) {
		//case 5:	// pointing on "Back", handled above ^^
		//	break;
		case 4://1:		// pointing on DAY OF WEEK
			switch (menu_cursor_x) {	// CHANGING DAY OF WEEK
				case 1:
					ssd1306_puts(40, 40, days[temp_day_of_week], 1, 0, 1);
					switch (button) {
						case BOT:
							temp_day_of_week--;
							if (temp_day_of_week == 255)
								temp_day_of_week = 6;
							break;
						case TOP:
							temp_day_of_week++;
							if (temp_day_of_week == 7)
								temp_day_of_week = 0;
							break;
					}
					break;
				case 2:
					menu_cursor_x = 0;
					BQ32002_setDate(temp_year, temp_month, temp_day, temp_day_of_week);
					break;
			}
			break;
		case 3://2:		// pointing on DATE
			switch (menu_cursor_x) {	// CHANGING DATE
				case 1:
					drawDateToSet(1, 0, 1, 0, 0, 1);
					switch (button) {
						case BOT:
							temp_year--;
							break;
						case TOP:
							temp_year++;
							break;
					}
					break;
				case 2:
					drawDateToSet(1, 0, 0, 1, 1, 0);
					switch (button) {
						case BOT:
							temp_month--;
							if (temp_month == 0)
								temp_month = 12;
							break;
						case TOP:
							temp_month++;
							if (temp_month == 13)
								temp_month = 1;
							break;
					}
					break;
				case 3:
					drawDateToSet(0, 1, 1, 0, 1, 0);
					switch (button) {
						case BOT:
							temp_day--;
							if (temp_day == 0)
								temp_day = 31;
							break;
						case TOP:
							temp_day++;
							if (temp_day == 32)
								temp_day = 1;
							break;
					}
					break;
				case 4:
					menu_cursor_x = 0;
					BQ32002_setDate(temp_year, temp_month, temp_day, temp_day_of_week);
					break;
			}
			break;
		case 2://3:		// pointing on TIME
			switch (menu_cursor_x) {	// CHANGING TIME
				case 1:
					drawTimeToSet(1, 0, 1, 0, 0, 1);
					switch (button) {
						case BOT:
							temp_ss--;
							if (temp_ss == 0)
								temp_ss = 59;
							break;
						case TOP:
							temp_ss++;
							if (temp_ss == 60)
								temp_ss = 1;
							break;
					}
					break;
				case 2:
					drawTimeToSet(1, 0, 0, 1, 1, 0);
					switch (button) {
						case BOT:
							temp_mm--;
							if (temp_mm == 0)
								temp_mm = 59;
							break;
						case TOP:
							temp_mm++;
							if (temp_mm == 60)
								temp_mm = 1;
							break;
					}
					break;
				case 3:
					drawTimeToSet(0, 1, 1, 0, 1, 0);
					switch (button) {
						case BOT:
							temp_hh--;
							if (temp_hh == 255)
								temp_hh = 23;
							break;
						case TOP:
							temp_hh++;
							if (temp_hh == 24)
								temp_hh = 0;
							break;
					}
					break;
				case 4:
					menu_cursor_x = 0;
					BQ32002_setTime(temp_hh, temp_mm, temp_ss);
					break;
			}
			break;
	}
}

void drawDateToSet( uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f ){
	ssd1306_put_int_zeroes( 40, 29, temp_day, 2, 1, a, b );
	ssd1306_put_int_zeroes( 58, 29, temp_month, 2, 1, c, d );
	ssd1306_put_int_zeroes( 76, 29, temp_year, 2, 1, e, f );
}

void drawTimeToSet( uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f ){
	ssd1306_put_int_zeroes( 40, 18, temp_hh, 2, 1, a, b );
	ssd1306_put_int_zeroes( 58, 18, temp_mm, 2, 1, c, d );
	ssd1306_put_int_zeroes( 76, 18, temp_ss, 2, 1, e, f );
}
