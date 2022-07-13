/*
 * menu_pointers.c
 *
 *  Created on: 19 maj 2021
 *      Author: Szymon Kajda
 */

#include "menu_pointers.h"
#include "../menus.h"
#include "../oled_lib/ssd1306.h"

uint8_t is_menu_arrow_moving=0;
int8_t menu_arrow_anim_dir=1;
uint8_t menu_arrow_step_cnt=11;

int8_t menu_arrow_pos_y = 0;	// with sign

/*
uint8_t isMenuArrowMoving( void ){
	return is_menu_arrow_moving;
}
*/

/* TRIGGERS POINTER ARROW ANIMATION & MOVES CURSOR */
void moveArrowCursor(uint8_t dir){
	if( !is_menu_arrow_moving ){
		//appCursor+=dir;
		menu_cursor_y += dir;
		menu_arrow_anim_dir = dir;
		menu_arrow_step_cnt = 0;
	}
}

void moveArrowCursorInv(uint8_t dir){
	if( !is_menu_arrow_moving ){
		//appCursor+=dir;
		menu_cursor_y += dir;
		menu_arrow_anim_dir = -dir;
		menu_arrow_step_cnt = 0;
	}
}


/*	DRAWS MENU POINTER ARROW
 *	menu_arrow_step_cnt used as a trigger to start
 *	(set to < distance ('0' recommended) -> starts drawing)
 */
void draw_menu_arrow( volatile uint8_t xPos, int8_t dir, uint8_t distance ){
	// distance - how many pixels we want the arrow to travel
	// menu_arrow_step_cnt - how many pixels it has already traveled
	// menu_arrow_pos_y - actual arrow position in Y axis
	// is_menu_arrow_moving - tells us if the arrow is already moving, used in if statement in "moveArrowCursor()"
	if( menu_arrow_step_cnt < distance ){
		menu_arrow_pos_y += dir;			// increment / decrement arrow position
		ssd1306_drawBitmap_P( xPos, menu_arrow_pos_y, arrow_on_left, ARROW_BMP_WIDTH, ARROW_BMP_HEIGHT, 1, 0 );
		menu_arrow_step_cnt++;				// arrow has just made one step or pixel
		is_menu_arrow_moving = 1;			// and it is moving
	}
	else
		is_menu_arrow_moving = 0;			// the arrow has reached it's destination, use "moveArrowCursor()" to trigger next travel

	if( menu_arrow_pos_y == 0 ){
		menu_arrow_step_cnt = 11;
		ssd1306_drawBitmap_P( xPos, menu_arrow_pos_y, arrow_on_left, ARROW_BMP_WIDTH, ARROW_BMP_HEIGHT, 1, 0 );
	}
	// Arrow roll:
	if( menu_arrow_pos_y < 0 ){					// if the arrow is above the screen
		switch(menu_arrow_pos_y){
			case -ARROW_BMP_HEIGHT-1:			// if every pixel of the arrow is above the screen
				menu_cursor_y = 5;				// roll the cursor and position
				menu_arrow_pos_y = 55;			// we will focus on the new arrow which has just rolled from beneath
				menu_arrow_step_cnt = distance;	// end the rolling
				break;
			default:
				ssd1306_drawBitmap_P( xPos, menu_arrow_pos_y+SSD1306_HEIGHT, arrow_on_left, ARROW_BMP_WIDTH, ARROW_BMP_HEIGHT, 1, 0 );
		}
	}
	// Still arrow roll:
	else if ( menu_arrow_pos_y > 63-ARROW_BMP_HEIGHT ){ // 64
		switch(menu_arrow_pos_y){
			case SSD1306_HEIGHT-ARROW_BMP_HEIGHT://63-ARROW_BMP_HEIGHT+1:
				menu_arrow_step_cnt--;
				break;
			case SSD1306_HEIGHT:
				menu_cursor_y = 0;
				menu_arrow_pos_y = 0;
				menu_arrow_step_cnt = distance;
				break;
		}
		ssd1306_drawBitmap_P( xPos, menu_arrow_pos_y-SSD1306_HEIGHT, arrow_on_left, ARROW_BMP_WIDTH, ARROW_BMP_HEIGHT, 1, 0 );
	}
}
