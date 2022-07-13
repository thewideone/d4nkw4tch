/*
 * bluetooth.h
 *
 *  Created on: Jun 5, 2021
 *      Author: Szymon Kajda
 */

#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include <avr/io.h>
#include "../uart_lib/uart.h"

/* Bluetooth */
//#define BT_BUF_LENGTH 20

#define BLUETOOTH_BUF_SIZE 20

extern char rx_buf[BLUETOOTH_BUF_SIZE];

void bluetoothTurnOn( void );
void bluetoothTurnOff( void );
uint8_t rxBufIdx( void );
/*
char btMsgChar( void );
void btResetMsgChar( void );
*/

#endif /* _BLUETOOTH_H_ */
