/*
 * math.h
 *
 *  Created on: Jun 5, 2021
 *      Author: Szymon Kajda
 */

#ifndef _MATH_H_
#define _MATH_H_

#include <avr/io.h>

//uint8_t length( uint8_t num );

void long_to_string( char str[], long num, uint8_t len );
//void uint8_t_to_string(char str[], uint8_t num);
//void uint8_t_to_string(char str[], uint8_t num, uint8_t len);
void uint16_t_to_string( char str[], uint16_t num, uint8_t len );



#endif /* _MATH_H_ */
