/*
 * time_date.c
 *
 *  Created on: 24 maj 2021
 *      Author: Szymon Kajda
 */

#include "date_time.h"

#include "pin_desc.h"

datetime_t datetime;
//TTEMP temperature;

char days[7][4] = { //[7][4]
		"Mon","Tue","Wed","Thu","Fri","Sat","Sun"
};

volatile uint8_t stayUpTime = STAY_UP_TIME;
static volatile uint8_t last_sqw_state = 0;
static volatile uint8_t sqw_state = 1;

uint8_t sqwState( void ){
	return sqw_state;
}

uint8_t sqwStateChanged( void ){
	sqw_state = bit_is_set( RTC_INT_PIN, RTC_INT ) ? 1 : 0;
	if( sqw_state != last_sqw_state )
		return 1;
	return 0;
}

void updateSqwState( void ){
	last_sqw_state = sqw_state;
}

void getDateTime( datetime_t * dt ){
	if( sqwStateChanged() )
		BQ32002_getDateTime( dt );
}

//void getTemp( TTEMP * temp ){
//	uint8_t sqw_state = bit_is_set( RTC_INT_PIN, RTC_INT ) ? 1 : 0;
//	if( sqw_state != last_sqw_state ){
//		DS3231_get_temp( temp );
//		last_sqw_state = sqw_state;
//	}
//}

/*
void readSqwState( void ){
	last_sqw_state = bit_is_set( RTC_INT_PIN, RTC_INT ) ? 1 : 0;
}
*/
/*
void getDateTime( TDATETIME * dt ){
	uint8_t sqw_state = bit_is_set( RTC_INT_PIN, RTC_INT ) ? 1 : 0;
	if( sqw_state != last_sqw_state ){
		DS3231_get_datetime( dt );	// &dt
		last_sqw_state = sqw_state;
	}
}
*/
