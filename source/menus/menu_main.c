/*
 * menu_main.c
 *
 *  Created on: Jun 5, 2021
 *      Author: Szymon Kajda
 */

#include "avr/io.h"
#include "menu_main.h"
#include "../bitmaps/power_bitmaps.h"
#include "../bitmaps/pictures.h"
#include "../date_time.h"
#include "../oled_lib/ssd1306.h"
#include "../power_adc_wdt.h"
#include "../math.h"
#include "../pin_desc.h"
#include "../power_adc_wdt.h"
#include "../button_debounce/buttons.h"
#include "../menus.h"

//#include <stdlib.h> // for abs()
//#define abs(x) x>=0 ? x : -x

// Not used yet
datetime_t last_charging_time;

void showTimeDefault( datetime_t * dt );
void showTimeAstronauts( datetime_t * dt );

void handleMenuMain( datetime_t * dt ){
	uint8_t bit_settings = readBitSettings();

	if( bit_is_clear( bit_settings, ASTRONAUT_MENU_BIT ) )
		showTimeDefault( dt );
	else
		showTimeAstronauts( dt );

	switch( button ){
		case PRESS:
			gotoMenu( menu_apps );
			break;
	}
}

//
// Draw battery level indicator.
// x, y  - coordinates of the start of the bitmap
// level - 0-255 value of battery level
//
void drawBattLvlIndicator( int x, int y, uint8_t level ){
	// Change to smooth transition using drawing lines
	// to reduce number of bitmaps needed
	if ( level>BATTERY_80 )
		ssd1306_drawBitmap_P( x, y, batt_4, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
	else if ( level>BATTERY_60 )
		ssd1306_drawBitmap_P( x, y, batt_3, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
	else if ( level>BATTERY_40 )
		ssd1306_drawBitmap_P( x, y, batt_2, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
	else if ( level>BATTERY_20 )
		ssd1306_drawBitmap_P( x, y, batt_1, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
	else
		ssd1306_drawBitmap_P( x, y, batt_0, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
}

//
// Save time of the last charging of the battery.
// Used by INT1 interrupt vector and in main() in init seciton.
//
void setLastChargingTime( datetime_t * dt ){
	last_charging_time.hh = dt->hh;
	last_charging_time.dayofweek = dt->dayofweek;
}

int abs( int x ){
	if( x >= 0 )
		return x;
	return -1*x;
}

//
// Draw battery level indicator based on time passed since the last charging.
// x, y  - coordinates of the start of the bitmap
//
/*
void drawTimeBattLvlIndicator( int x, int y, datetime_t *dt ){
	// Change to smooth transition using drawing lines
	// to reduce number of bitmaps needed
	int hours_passed =  abs(dt->dayofweek - last_charging_time.dayofweek) * 24 + abs(dt->hh - last_charging_time.hh);

	ssd1306_put_int( x, 35, hours_passed, 1, 1, 0 );
	ssd1306_put_int( x+10, 35, abs(dt->dayofweek - last_charging_time.dayofweek), 1, 1, 0 );
	ssd1306_put_int( x+20, 35, abs(dt->hh - last_charging_time.hh), 1, 1, 0 );

	ssd1306_put_int( x, 10, (int)last_charging_time.dayofweek, 1, 1, 0 );
	ssd1306_put_int( x, 22, (int)last_charging_time.hh, 1, 1, 0 );
	ssd1306_put_int( x+16, 10, (int)dt->dayofweek, 1, 1, 0 );
	ssd1306_put_int( x+16, 22, (int)dt->hh, 1, 1, 0 );


	if ( hours_passed < 15 )
		ssd1306_drawBitmap_P( x, y, batt_4, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
	else if ( hours_passed < 20 )
		ssd1306_drawBitmap_P( x, y, batt_3, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
	else if ( hours_passed < 24 )
		ssd1306_drawBitmap_P( x, y, batt_2, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
	else if ( hours_passed < 27 )
		ssd1306_drawBitmap_P( x, y, batt_1, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
	else
		ssd1306_drawBitmap_P( x, y, batt_0, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
}
*/

//
// Draw battery charging indicator.
// x, y  - coordinates of the start of the bitmap
//
void drawChargingIndicator( int x, int y ){
	if ( isCharging() )
		ssd1306_drawBitmap_P( x, y, chrg, CHRG_BMP_WIDTH, CHRG_BMP_HEIGHT, 1, 0 );
	else
		ssd1306_drawFillRect( x, y, CHRG_BMP_WIDTH, CHRG_BMP_HEIGHT, 0 );
}

//
// Draw 'USB plugged' indicator.
// x, y  - coordinates of the start of the bitmap
//
void drawUSBIndicator( int x, int y ){
	if ( isUSBPlugged() )
		ssd1306_drawBitmap_P( x, y, usb, USB_BMP_WIDTH, USB_BMP_HEIGHT, 1, 0 );
	else
		ssd1306_drawFillRect( x, y, USB_BMP_WIDTH, USB_BMP_HEIGHT, 0 );
}

void showTimeAstronauts( datetime_t * dt ){
	if( sqwStateChanged() ){
		readVcc();
		uint8_t batt_lvl = batteryLvl();

		// Maybe move these VV to some ISR?
		drawBattLvlIndicator( 27, 57, batt_lvl );
//		drawTimeBattLvlIndicator( 27, 57, dt );

		drawChargingIndicator( 34, 41 );
		drawUSBIndicator( 27, 49 );


		ssd1306_puts( 80, 2, dt->date, 1, 1, 0 );
		ssd1306_drawChar( 66, 14, dt->time[0], 1, 0, 2 );
		ssd1306_drawChar( 78, 14, dt->time[1], 1, 0, 2 );
		ssd1306_drawChar( 93, 14, dt->time[3], 1, 0, 2 );
		ssd1306_drawChar( 105, 14, dt->time[4], 1, 0, 2 );
//		ssd1306_put_int( 66, 14, dt->hh, 2, 1, 0 );
//		ssd1306_put_int( 92, 14, dt->mm, 2, 1, 0 );
		ssd1306_drawChar( 116, 21, dt->time[6], 1, 0, 1 );
		ssd1306_drawChar( 122, 21, dt->time[7], 1, 0, 1 );
//		ssd1306_put_int( 116, 21, dt->ss, 1, 1, 0 );

		// RTC counts days of week from 1 to 7!
		ssd1306_puts( 66, 31, days[ dt->dayofweek - 1 ], 1, 1, 0 );
//		ssd1306_put_int( 66, 42, (int)(dt->dayofweek), 1, 1, 0 );
//		ssd1306_put_int( 66, 42, dt->dayofweek, 1, 1, 0 );
//		ssd1306_puts( 89, 31, temp->temperature, 1, 1, 0 );
//		ssd1306_puts( 102, 31, "*C", 1, 1, 0 );

//		ssd1306_drawFillRect( 89, 18, 2, 2, 1 );
//		ssd1306_drawFillRect( 89, 22, 2, 2, 1 );
	}
}

/* DISPLAY DATE AND TIME */
void showTimeDefault( datetime_t * dt ){
	// 3.69V -> 0.97-0.98V -> reading: 161-162

	// Update frame every 0.5s
	if( sqwStateChanged() ){

		if( sqwState() )
			ssd1306_drawFillRect( 2, 2, 2, 2, 1 );
		else
			ssd1306_drawFillRect( 2, 2, 2, 2, 0 );

		uint8_t batt_lvl;
		uint8_t charging = isCharging();
		ssd1306_drawFillRect( 2, 2, 2, 2, 1 );
		readVcc();
		batt_lvl = batteryLvl();
		ssd1306_put_int( 5, 46, (int)batt_lvl, 1, 1, 0 );

		ssd1306_put_int( 30, 46, charging, 1, 1, 0 );

		// Maybe move these VV to some ISR?
		drawBattLvlIndicator( 2, 55, batt_lvl );
		drawChargingIndicator( 33, 55 );
		drawUSBIndicator( 15, 55 );

		ssd1306_puts( 40, 2, dt->date, 1, 1, 0 );

		ssd1306_puts( 16, 17, dt->time, 2, 1, 0 ); // x=16,y=17
		// RTC counts days of week from 1 to 7!
		ssd1306_puts( 55, 39, days[ dt->dayofweek - 1 ], 1, 1, 0 );
//		ssd1306_put_int( 35, 39, (int)(dt->dayofweek), 1, 1, 0 );
//		ssd1306_put_int( 35, 39, dt->dayofweek, 1, 1, 0 );

//		if (gotMsg)
//			ssd1306_drawChar(98,56,'M',1,0,1);
//		else
//			ssd1306_drawChar(98,56,' ',1,0,1);

		// Binary clock:
		for(uint8_t i=0;i<8;i++){
			ssd1306_setPixel(i+10,7,(datetime.ss>>i)&1);
			ssd1306_setPixel(i+10,6,(datetime.mm>>i)&1);
			ssd1306_setPixel(i+10,5,(datetime.hh>>i)&1);
		}
	}
//	else
//		ssd1306_drawFillRect( 2, 2, 2, 2, 0 );
}
