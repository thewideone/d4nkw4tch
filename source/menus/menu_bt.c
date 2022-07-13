/*
 * menu_bt.c
 *
 *  Created on: Jun 18, 2021
 *      Author: Szymon Kajda
 */

#include "menu_bt.h"
#include "../menus.h"

uint8_t bt_terminal_cursor_x;//=65;
uint8_t bt_terminal_cursor_y;//=0;

uint8_t draw_char_idx = 0;

void btCmdCursorReset(  void ){
	bt_terminal_cursor_x=65;
	bt_terminal_cursor_y=0;
}

void btCmdCursorNewLine( void ){
	bt_terminal_cursor_x=65;
	bt_terminal_cursor_y+=10;
}

// needs optimalization,
void btCmdDrawChar( void ){
	//uint8_t rx_buf_idx = rxBufIdx();

	if( rxBufIdx() != draw_char_idx ){
		ssd1306_drawChar( bt_terminal_cursor_x, bt_terminal_cursor_y, rx_buf[draw_char_idx], 1, 0, 1 );	// draw received character
		bt_terminal_cursor_x += FONT_WIDTH+1;	// move cursor after drawing char
		if( bt_terminal_cursor_x > 120 ){	// new line
			//btCmdCursorNewLine();
			bt_terminal_cursor_x=65;
			bt_terminal_cursor_y+=9;//10;
			if( bt_terminal_cursor_y > SSD1306_HEIGHT - FONT_HEIGHT - 2 ){	// (55) if the new line wants to be drawn out of screen,
				//btCmdCursorReset();	// reset cursor to the default location
				bt_terminal_cursor_y=0;
				ssd1306_drawFillRect( 65, 0, 62, 64, 0 );	// and clear half of the screen
			}
		}
		draw_char_idx++;
		if( draw_char_idx >= BLUETOOTH_BUF_SIZE )
			draw_char_idx = 0;
	}

	/*
	// set flag?
	ssd1306_drawChar( bt_terminal_cursor_x, bt_terminal_cursor_y, msg_char, 1, 0, 1 );	// draw received character
	bt_terminal_cursor_x += FONT_WIDTH+1;	// move cursor after drawing char
	if( bt_terminal_cursor_x > 120 ){	// new line
		btCmdCursorNewLine();
		if( bt_terminal_cursor_y > SSD1306_HEIGHT - FONT_HEIGHT - 2 ){	// (55) if the new line wants to be drawn out of screen,
			btCmdCursorReset();	// set cursor to the new line
			//bt_terminal_cursor_x=65;	// reset cursor to default location
			//bt_terminal_cursor_y=0;
			ssd1306_drawFillRect( 65, 0, 64, 64, 0 );	// and clear half of the screen
		}
	}
	//btResetMsgChar();	// clear msg var to avoid indefinitely drawing chars
	*/
}


void handleMenuBt( void ){
	//if( btMsgChar() != 0 )
	//	btCmdDrawChar( btMsgChar() );
	btCmdDrawChar();
	draw_menu_arrow(55, menu_arrow_anim_dir, 11);

	switch( button ){
		case BOT:
			moveArrowCursor(1);
			break;
		case TOP:
			moveArrowCursor(-1);
			break;
		case PRESS:
			//switch(appCursor){
			switch( menu_cursor_y ){
				case 0:
					bluetoothTurnOn();
					break;
				case 1:
					bluetoothTurnOff();
					//ssd1306_init( SSD1306_SWITCHCAPVCC, REFRESH_MAX );	// screen glitches
					//ssd1306_cmd( SSD1306_DISPLAYON );
					break;
				case 3:
					gotoMenu( menu_bt_terminal );
					//gotoMenuBtTerminal();
					break;
				case 5:
					gotoMenu( menu_apps );
					//gotoMenuMain();
					break;
			}
			break;
		}
}

void handleMenuBtTerminal( void ){
	btCmdDrawChar();
	draw_menu_arrow(55, menu_arrow_anim_dir, 11);

	switch (button) {
		case BOT:
			//button = NONE;
			moveArrowCursor(1);
			break;
		case TOP:
			//button = NONE;
			moveArrowCursor(-1);
			break;
		case PRESS:
			//switch (appCursor) {	// select command
			switch( menu_cursor_y ){
				case 0:
					tx_string("Test str\n");
					break;
				case 1:
					tx_string("AT+ROLE0");
					break;
				case 2:
					tx_string("AT+MODE?");
					break;
				case 3:
					tx_string("AT+PASS?");
					break;
				case 4:
					tx_string("AT+RESET");
					break;
				case 5:
					gotoMenu( menu_bt );
					//gotoMenuBt();
					break;
			}
			break;
	}
}
