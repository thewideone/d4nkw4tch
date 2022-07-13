/*
 * menu_pointers.h
 *
 *  Created on: 20 maj 2021
 *      Author: Szymon Kajda
 */

#ifndef MENU_POINTERS_H
#define MENU_POINTERS_H

#include <avr/io.h>

#include "../bitmaps/arrow_cursor_bitmaps.h"

extern int8_t menu_arrow_anim_dir;

extern int8_t menu_arrow_pos_y;	// with sign

void moveArrowCursor(uint8_t dir);
void moveArrowCursorInv(uint8_t dir);
void draw_menu_arrow(volatile uint8_t xPos, int8_t dir, uint8_t distance);

#endif /* MENU_POINTERS_H */
