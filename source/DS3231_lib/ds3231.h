#ifndef DS3231_H_
#define DS3231_H_

#include <stdint.h>

#define TIME_AS_STRING 1
#define DATE_AS_STRING 1
#define TEMPERATURE_AS_STRING 1

#define TIME_SEPARATOR ':'
#define DATE_SEPARATOR '.'

#define DS3231_ADDR 0xD0

typedef union {
	uint8_t bytes[7];
	struct {
		uint8_t ss;
		uint8_t mm;
		uint8_t hh;
		uint8_t dayofweek;
		uint8_t day;
		uint8_t month;
		uint8_t year;
#if TIME_AS_STRING == 1
		char time[9];
#endif
#if DATE_AS_STRING == 1
		char date[11];
#endif
	};
} TDATETIME;

typedef struct {
	int8_t cel;
	uint8_t fract;
#if TEMPERATURE_AS_STRING == 1
	char temperature[3];
#endif
} TTEMP;

void DS3231_init( void );
void DS3231_get_datetime( TDATETIME * dt );
void show_time( TDATETIME * dt, TTEMP * temp);
void DS3231_set_time( uint8_t hh, uint8_t mm, uint8_t ss );
void DS3231_set_date( uint8_t year, uint8_t month, uint8_t day, uint8_t dayofweek );
void DS3231_get_temp( TTEMP * tmp );
void int_to_string( char str[], uint8_t num, uint8_t len );	// added by Szymon Kajda
uint8_t dec2bcd( uint8_t dec );
uint8_t bcd2dec( uint8_t bcd );

#endif /*	DS3231_H_	*/
