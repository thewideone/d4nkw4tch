/*
 * bluetooth.c
 *
 *  Created on: Jun 5, 2021
 *      Author: Szymon Kajda
 */

#include "bluetooth.h"
#include "../date_time.h"
#include "../button_debounce/buttons.h"
#include "../led.h"
#include "../pin_desc.h"
#include "../uart_lib/uart.h"
#include "../oled_lib/ssd1306.h"

char msg_char = 0; // works with uint8_t
uint8_t msg_ready = 0;

char rx_buf[BLUETOOTH_BUF_SIZE]={};
uint8_t rx_buf_idx = 0;

void bluetoothTurnOn( void ){
	for(uint8_t i=0;i<81;i++)
		tx_char('a');
}
void bluetoothTurnOff( void ){
	tx_string("AT+SLEEP");
}

uint8_t rxBufIdx( void ){
	return rx_buf_idx;
}

/*
char btMsgChar( void ){
	return msg_char;
}

void btResetMsgChar( void ){
	msg_char = 0;
}
*/

/* UART RECEIVE INTERRUPT used for BT */
ISR(USART_RX_vect){
	//msg_char = rx_char();//=1
	//wakeUpTime = datetime.ss;	// stay up
	//buttonForceRelease();		// reset button state
	//button = NONE;
	//btCmdDrawChar( gotMsg );

	uint8_t l_rx_buf_idx = rx_buf_idx;
	rx_buf[l_rx_buf_idx++] = UDR0;

	if( l_rx_buf_idx >= BLUETOOTH_BUF_SIZE )
		l_rx_buf_idx = 0;
	rx_buf_idx = l_rx_buf_idx;

	//ssd1306_display();		// I think it's not needed here
//	LED_ON;					// LED turns off probably beacuse of the WDT
	//bt_buf[bt_buf_index] = UDR0;
	//while(!(UCSR0A & (1<<RXC0)));
	//bt_buf_index++;
}
