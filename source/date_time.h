/*
 * time_date.h
 *
 *  Created on: 24 maj 2021
 *      Author: Szymon Kajda
 */

#ifndef _TIME_DATE_H_
#define _TIME_DATE_H_

#include "BQ32002_lib/bq32002.h"
#include "pin_desc.h"

extern char days[7][4];		// defined in "time_date.c", declared here

extern datetime_t datetime;	// defined in "time_date.c", declared here
//extern TTEMP temperature;	// defined in "time_date.c", declared here

#define STAY_UP_TIME 5 /* In seconds */
extern volatile uint8_t stayUpTime;// = STAY_UP_TIME;
volatile uint8_t wakeUpTime;

//static volatile uint8_t last_sqw_state = 0;	// defined in "time_date.c", declared here

uint8_t sqwState( void );
uint8_t sqwStateChanged( void );
void updateSqwState( void );		// handled at the end of "main.c"
//void readSqwState( void );
void getDateTime( datetime_t * dt );
//void getTemp( TTEMP * temp );

#endif /* _TIME_DATE_H_ */
