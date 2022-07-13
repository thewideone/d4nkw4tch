/*
 * menu_bt.h
 *
 *  Created on: Jun 18, 2021
 *      Author: Szymon Kajda
 */

#ifndef SOURCE_MENUS_MENU_BT_H_
#define SOURCE_MENUS_MENU_BT_H_

void btCmdCursorReset(  void );
void btCmdCursorNewLine( void );
void btCmdDrawChar( void );
void handleMenuBt( void );
void handleMenuBtTerminal( void );

#endif /* SOURCE_MENUS_MENU_BT_H_ */
