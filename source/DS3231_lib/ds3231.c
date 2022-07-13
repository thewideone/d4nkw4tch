#include "ds3231.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../I2C_TWI/i2c_twi.h"

#include "../pin_desc.h"	// added by Szymon Kajda

void DS3231_init( void ){
	RTC_INT_DDR &= ~(1<<RTC_INT);		// added by Szymon Kajda
	RTC_INT_PORT |= (1<<RTC_INT);		//
	RTC_32KHZ_DDR &= (1<<RTC_32KHZ);	//
	RTC_32KHZ_PORT |= (1<<RTC_32KHZ);	//
	i2cSetBitrate();
	uint8_t ctrl = (1<<6);// | (1<<3);//0;		// set BBSQW bit (6) in Control Register to enable square wave output, changed by Szymon Kajda
												// set EN32kHz (3) bit to enable 32kHz output
	TWI_write_buf( DS3231_ADDR, 0x0E, 1, &ctrl );
}

void DS3231_get_temp( TTEMP * temp ){
	uint8_t buf[2];
	TWI_read_buf( DS3231_ADDR, 0x11, 2, buf );
	temp->cel = buf[0];
	temp->fract = buf[1]>>6;

	//TWI_read_buf( DS3231_ADDR, 0x0F, 1, buf[0] );		// read Status Register
	//TWI_read_buf( DS3231_ADDR, 0x0E, 1, buf );		// read Control Register
	//if( bit_is_clear( buf[0], 2 ) || bit_is_clear( buf[1], 5 ) ){		// if BSY bit is 0 (DS3231 not busy) or conversion is not pending
	//if( bit_is_clear( buf[0], 5 ) ){
	TWI_read_buf( DS3231_ADDR, 0x0E, 1, buf );
	buf[0] |= (1<<5);							// set CONV bit (Convert Temperature)
	TWI_write_buf( DS3231_ADDR, 0x0E, 1, buf );
	//}

#if TEMPERATURE_AS_STRING == 1
	char t[3];
	int_to_string(t,temp->cel,2);
	temp->temperature[0] = t[0];
	temp->temperature[1] = t[1];
	temp->temperature[2] = 0;	// string terminator
#endif
}

void DS3231_get_datetime( TDATETIME * dt ){
	uint8_t i;
	uint8_t buf[7];
	TWI_read_buf( DS3231_ADDR, 0x00, 7, buf );
	for( i=0; i<7; i++ ) dt->bytes[i] = bcd2dec( buf[i] );

#if TIME_AS_STRING == 1 || DATE_AS_STRING == 1
	char s[2];
#endif
#if TIME_AS_STRING == 1
	int_to_string( s,dt->hh,2 );
	dt->time[0] = s[0];
	dt->time[1] = s[1];
	dt->time[2] = TIME_SEPARATOR;
	int_to_string( s,dt->mm,2 );
	dt->time[3] = s[0];
	dt->time[4] = s[1];
	dt->time[5] = TIME_SEPARATOR;
	int_to_string( s,dt->ss,2 );
	dt->time[6] = s[0];
	dt->time[7] = s[1];
	dt->time[8] = 0;	// string terminator
#endif
#if DATE_AS_STRING == 1
	int_to_string( s,dt->day,2 );
	dt->date[0] = s[0];
	dt->date[1] = s[1];
	dt->date[2] = DATE_SEPARATOR;
	int_to_string( s,dt->month,2 );
	dt->date[3] = s[0];
	dt->date[4] = s[1];
	dt->date[5] = DATE_SEPARATOR;
	int_to_string( s,dt->year,2 );
	dt->date[6] = s[0];
	dt->date[7] = s[1];
	dt->date[8] = 0;	// string terminator
#endif
}

void DS3231_set_time( uint8_t hh, uint8_t mm, uint8_t ss ){
	uint8_t buf[3];
	buf[0] = dec2bcd(ss);
	buf[1] = dec2bcd(mm);
	buf[2] = dec2bcd(hh);
	TWI_write_buf( DS3231_ADDR, 0x00, 3, buf );
}

void DS3231_set_date( uint8_t year, uint8_t month, uint8_t day, uint8_t dayofweek ){
	uint8_t buf[4];
	buf[0] = dayofweek;
	buf[1] = dec2bcd(day);
	buf[2] = dec2bcd(month);
	buf[3] = dec2bcd(year);
	TWI_write_buf( DS3231_ADDR, 0x03, 4, buf );
}

// added by Szymon Kajda
void int_to_string( char str[], uint8_t num, uint8_t len ){
	uint8_t rem;
	for( uint8_t i=0; i<len; i++ ){
		rem = num % 10;
		num /= 10;
		str[len-(i+1)] = rem + '0';
	}
}

uint8_t dec2bcd( uint8_t dec ){
	return ((dec / 10)<<4) | ( dec % 10 );
}
uint8_t bcd2dec( uint8_t bcd ){
	return ((((bcd) >> 4) & 0x0F) * 10) + ((bcd) & 0x0F);
}
