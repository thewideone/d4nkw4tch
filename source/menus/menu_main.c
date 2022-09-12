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

void show_time( datetime_t * dt );

void handleMenuMain( datetime_t * dt ){
	show_time( dt );
	switch( button ){
		case PRESS:
			gotoMenu( menu_apps );
			//gotoMenuApps();
			break;
	}
}

void handleMenuMainAstronauts( datetime_t * dt ){
	if( sqwStateChanged() ){
		readVcc();
		uint8_t batt_lvl = batteryLvl();
//		ssd1306_put_int( 5, 46, batt_lvl, 1, 1, 0 );

//		ssd1306_put_int( 30, 46, charging, 1, 1, 0 );

		// Maybe move this VV to some ISR?
		if (isCharging())
			ssd1306_drawBitmap_P( 34, 57, chrg, CHRG_BMP_WIDTH, CHRG_BMP_HEIGHT, 1, 0 );
		else
			ssd1306_drawFillRect( 34, 57, CHRG_BMP_WIDTH, CHRG_BMP_HEIGHT, 0 );
		if (batt_lvl>BATTERY_80)
			ssd1306_drawBitmap_P( 27, 57, batt_4, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
		else if (batt_lvl>BATTERY_60)
			ssd1306_drawBitmap_P( 27, 57, batt_3, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
		else if (batt_lvl>BATTERY_40)
			ssd1306_drawBitmap_P( 27, 57, batt_2, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
		else if (batt_lvl>BATTERY_20)
			ssd1306_drawBitmap_P( 27, 57, batt_1, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
		else // if (batt_lvl<21)
			ssd1306_drawBitmap_P( 27, 57, batt_0, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
		if (isUSBPlugged())
			ssd1306_drawBitmap_P( 27, 49, usb, USB_BMP_WIDTH, USB_BMP_HEIGHT, 1, 0 );
		else
			ssd1306_drawFillRect( 27, 49, USB_BMP_WIDTH, USB_BMP_HEIGHT, 0 );

		ssd1306_puts( 80, 2, dt->date, 1, 1, 0 );
//		ssd1306_puts( 16, 17, dt->time, 2, 1, 0 ); // x=16,y=17
		ssd1306_drawChar( 66, 14, dt->time[0], 1, 0, 2 );
		ssd1306_drawChar( 78, 14, dt->time[1], 1, 0, 2 );
		ssd1306_drawChar( 93, 14, dt->time[3], 1, 0, 2 );
		ssd1306_drawChar( 105, 14, dt->time[4], 1, 0, 2 );
//		ssd1306_put_int( 66, 14, dt->hh, 2, 1, 0 );
//		ssd1306_put_int( 92, 14, dt->mm, 2, 1, 0 );
		ssd1306_drawChar( 116, 21, dt->time[6], 1, 0, 1 );
		ssd1306_drawChar( 122, 21, dt->time[7], 1, 0, 1 );
//		ssd1306_put_int( 116, 21, dt->ss, 1, 1, 0 );

		ssd1306_puts( 66, 31, days[dt->dayofweek], 1, 1, 0 );
//		ssd1306_put_int( 66, 42, (int)(dt->dayofweek), 1, 1, 0 );
		ssd1306_put_int( 66, 42, dt->dayofweek, 1, 1, 0 );
//		ssd1306_puts( 89, 31, temp->temperature, 1, 1, 0 );
//		ssd1306_puts( 102, 31, "*C", 1, 1, 0 );

//		ssd1306_drawFillRect( 89, 18, 2, 2, 1 );
//		ssd1306_drawFillRect( 89, 22, 2, 2, 1 );
	}

	switch( button ){
			case PRESS:
				gotoMenu( menu_apps );
				//gotoMenuApps();
				break;
	}
}

/* DISPLAY DATE AND TIME */
void show_time( datetime_t * dt ){
//	TCCR0A &= ~(1 << COM0B1);
//	for( uint8_t i=0; i<10; i++ ){
//		ssd1306_drawFillRect( 5, 5, 5, 5, 1 );
//		ssd1306_display();
//		_delay_ms(1000);
//		ssd1306_drawFillRect( 5, 5, 5, 5, 0 );
//		ssd1306_display();
//		_delay_ms(1000);
//	}
	// 3.69V -> 0.97-0.98V -> reading: 161-162
	//

	//uint8_t batt_lvl;
	//batt_lvl = readVcc();
	//ssd1306_put_int( 5, 46, batt_lvl, 1, 1, 0 );
	if( sqwState() )
		ssd1306_drawFillRect( 2, 2, 2, 2, 1 );
	else
		ssd1306_drawFillRect( 2, 2, 2, 2, 0 );

	if( sqwStateChanged() ){
		uint8_t batt_lvl;
		uint8_t charging = isCharging();
		ssd1306_drawFillRect( 2, 2, 2, 2, 1 );
		readVcc();
		batt_lvl = batteryLvl();//readVcc();//readBattLvl();
		//char buf[4];
		//long_to_string( lvl, batt_lvl, 4);
		//uint16_t_to_string( buf, batt_lvl, 4);
		//ssd1306_puts( 5, 48, buf, 1, 1, 0 );
		ssd1306_put_int( 5, 46, batt_lvl, 1, 1, 0 );

		ssd1306_put_int( 30, 46, charging, 1, 1, 0 );
		//char floatBuf[5];
		//float Vbatt = batt_lvl*1.1*(102)/(27621);
		//int Vbatt = batt_lvl*1.1*(102)/(27621)*100;
		//volatile uint16_t Vbatt = 12345;//(uint16_t)(batt_lvl*0.406);
		//gcvt( Vbatt, 4, buf );
		//ftoa_my( Vbatt, floatBuf, 2 );
		//float_to_string( floatBuf, 12345 );
		//ssd1306_put_float( 5, 36, (uint16_t)(batt_lvl*0.406), 1, 1, 0 );
		//ssd1306_put_float( 5, 36, 365, 1, 1, 0 );
		//ssd1306_puts( 5, 36, floatBuf, 1, 1, 0 );
		//volatile uint8_t n = 123;
		//ssd1306_put_int( 5, 36, n, 1, 1, 0 );

		// Maybe move this VV to some ISR?
		if (isCharging())
			ssd1306_drawBitmap_P( 33, 55, chrg, CHRG_BMP_WIDTH, CHRG_BMP_HEIGHT, 1, 0 );
		else
			ssd1306_drawFillRect( 17, 55, CHRG_BMP_WIDTH, CHRG_BMP_HEIGHT, 0 );
		if (batt_lvl>BATTERY_80)
			ssd1306_drawBitmap_P( 2, 55, batt_4, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
		else if (batt_lvl>BATTERY_60)
			ssd1306_drawBitmap_P( 2, 55, batt_3, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
		else if (batt_lvl>BATTERY_40)
			ssd1306_drawBitmap_P( 2, 55, batt_2, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
		else if (batt_lvl>BATTERY_20)
			ssd1306_drawBitmap_P( 2, 55, batt_1, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
		else // if (batt_lvl<21)
			ssd1306_drawBitmap_P( 2, 55, batt_0, BATT_BMP_WIDTH, BATT_BMP_HEIGHT, 1, 0 );
		if (isUSBPlugged())
			ssd1306_drawBitmap_P( 15, 55, usb, USB_BMP_WIDTH, USB_BMP_HEIGHT, 1, 0 );
		else
			ssd1306_drawFillRect( 15, 55, USB_BMP_WIDTH, USB_BMP_HEIGHT, 0 );
		ssd1306_puts( 40, 2, dt->date, 1, 1, 0 );
		/*
		if(displayTimeXPos<127){
				displayTimeXPos+=menuArrowAnimDir;
				switch(menuArrowAnimDir){
					case -1:
						ssd1306_drawFastHLine( displayTimeXPos-1, 16, 14, 0 );
						break;
					case 1:
						ssd1306_drawFastHLine( displayTimeXPos+1, 16, 14, 0 );
						break;
				}
				ssd1306_puts( displayTimeXPos, 17, dt->time, 2, 1, 0 ); // x=16,y=17
				menuArrowStepCount++;
		}
	*/
		ssd1306_puts( 16, 17, dt->time, 2, 1, 0 ); // x=16,y=17
		ssd1306_puts( 55, 39, days[dt->dayofweek], 1, 1, 0 );
//		ssd1306_put_int( 35, 39, (int)(dt->dayofweek), 1, 1, 0 );
		ssd1306_put_int( 35, 39, dt->dayofweek, 1, 1, 0 );
//		ssd1306_puts( 104, 56, temp->temperature, 1, 1, 0 );
//		ssd1306_puts( 116, 56, "*C", 1, 1, 0 );
	/*
		if (gotMsg)
			ssd1306_drawChar(98,56,'M',1,0,1);
		else
			ssd1306_drawChar(98,56,' ',1,0,1);
	*/
		//ssd1306_drawFastVLine(0,0,10,1);
		// Binary clock:
		for(uint8_t i=0;i<8;i++){
			ssd1306_setPixel(i+10,7,(datetime.ss>>i)&1);
			ssd1306_setPixel(i+10,6,(datetime.mm>>i)&1);
			ssd1306_setPixel(i+10,5,(datetime.hh>>i)&1);
		}
	}
	else
		ssd1306_drawFillRect( 2, 2, 2, 2, 0 );
}
