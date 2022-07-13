/*
 * math.c
 *
 *  Created on: Jun 5, 2021
 *      Author: Szymon Kajda
 */

#include "math.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "oled_lib/ssd1306.h"

void long_to_string( char str[], long num, uint8_t len ){
	long rem;
	for( uint8_t i=0; i<len; i++ ){
		rem = num % 10;
		num /= 10;
		str[len-(i+1)] = rem + '0';
	}
}

/*
void uint8_t_to_string(char str[], uint8_t num){
    uint8_t i, rem, len = 0, n;
    n = num;
    while ( n != 0 ){
        len++;
        n /= 10;
    }
    for ( i = 0; i < len; i++ ){
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}
*/
/*
void uint8_t_to_string( char str[], uint8_t num, uint8_t len ){
	uint8_t rem;
	for( uint8_t i=0; i<len; i++ ){
		rem = num % 10;
		num /= 10;
		str[len-(i+1)] = rem + '0';
	}
}
*/
void uint16_t_to_string( char str[], uint16_t num, uint8_t len ){
	uint16_t rem;
	for( uint8_t i=0; i<len; i++ ){
		rem = num % 10;
		num /= 10;
		str[len-(i+1)] = rem + '0';
	}
}

/*
uint8_t length( uint8_t num ){
	uint8_t i=0;
	while( num>0 ){
		num/=10;
		i++;
	}
	return i;
}
*/
