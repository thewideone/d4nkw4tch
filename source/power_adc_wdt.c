/*
 * power_adc_wdt.c
 *
 *  Created on: 20 maj 2021
 *      Author: Szymon Kajda
 */

#include <avr/io.h>
#include <util/delay.h>
#include "power_adc_wdt.h"
#include "oled_lib/ssd1306.h"
#include "buzzer.h"
#include "pin_desc.h"

//#define FIRST_BOOT

// Inverted logic (set to 0 to turn on)
#ifdef FIRST_BOOT
//uint8_t bitSettings = 0b11111111;
static uint8_t EEMEM bitSettingsEEMEM = 0b11111111;
#else
static uint8_t EEMEM bitSettingsEEMEM;
#endif

uint8_t battery_lvl;// = BATTERY_80;

/* Read battery level through voltage divider.
 * Its range is ~(0.874V - 1.112V) for ~(3.3V - 4.2V) battery,
 * so reading should be @ 10 bits (uint16_t) ~(812 - 1023)
 * and is @ 8 bits (uint8_t) ~( <=93 @ 3.3V - 255 @ 4.0V )
 */
void readVcc( void ) {
	//uint8_t result;

	BATT_EN_PORT |= (1<<BATT_EN);	// Enable the voltage divider
	//ADCSRA |= (1<<ADEN);			// Enable ADC
	// Input is ADC7 with 1.1V as reference,
	// setting ADLAR shifts ADC value to the left
	// so reading just ADCH gives 8-bit value
	//ADMUX = _BV(REFS1) | _BV(REFS0) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);
	ADMUX = _BV(ADLAR) | _BV(REFS1) | _BV(REFS0) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);
	//ADMUX = _BV(REFS0) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);
	_delay_ms(2);					// Wait for Vref to settle
	ADCSRA |= (1<<ADEN);			// Enable ADC
	ADCSRA |= _BV(ADSC);			// Convert
	while (bit_is_set(ADCSRA,ADSC));// Wait for it wait for it wait for it
	//result = ADCL;
	//result |= ADCH<<8;
	//while (! (ADCSRA & _BV(ADIF)) );	// We can test ADIF
	//result = ADC;
	//result = ADCH;
	battery_lvl = ADCH;
	//ADCSRA |= _BV( ADIF );			// but it has to be reset manually (unless we're handling an interrupt)

	ADCSRA &= ~(1<<ADEN);			// Disable ADC
	BATT_EN_PORT &= ~(1<<BATT_EN);	// Disable the divider

	//return result;
}

uint8_t batteryLvl( void ){
	return battery_lvl;
}

void handlePowerSettings( uint8_t bit_settings ){
	if( battery_lvl <= BATTERY_20 )
		bit_settings &= ~(1<<PWR_SAVING_MODE_BIT);	// Inverted logic (set to 0 to turn on)
	else{
		ssd1306_invertDisplay( !(bit_settings & (1<<INV_DISPLAY_BIT)) );
	}

	// Turn on power-saving mode if needed
	if( bit_is_clear( bit_settings, PWR_SAVING_MODE_BIT ) ){
		/* FOR LOW CONTRAST */
		ssd1306_cmd( SSD1306_SETCONTRAST );
		ssd1306_cmd(0);
		buzzerSetStateOff();
		stayUpTime = 3;		// In seconds
	}
	else{
		/* FOR HIGH CONTRAST */
		ssd1306_cmd( SSD1306_SETCONTRAST );
		ssd1306_cmd(255);
		buzzerSetStateOn();
		stayUpTime = 5;		// In seconds
	}
}

/* Settings stored as bits in one EEPROM byte */
uint8_t readBitSettings( void ){
	return eeprom_read_byte( &bitSettingsEEMEM );
}

void updateBitSettings( uint8_t bitSettings ){
	eeprom_update_byte( &bitSettingsEEMEM, bitSettings );
}

/* Check if battery is charging */
uint8_t isCharging( void ){
	if( isUSBPlugged() ){
		if( getChargerState() == 1 )
			return 1;
	}
	else
		return 0;

//	bit_is_clear( CHRG_STAT_PIN, CHRG_STAT );
}

/* Check if USB is plugged in */
uint8_t isUSBPlugged( void ){
	return bit_is_set( USB_SENSE_PIN, USB_SENSE );
}

uint8_t getChargerState( void ){
	uint8_t result1 = 0, result2 = 0;

	CHRG_STAT_PORT |= (1<<CHRG_STAT);

	if(bit_is_set( CHRG_STAT_PIN, CHRG_STAT ))
		result1 = 1;

	CHRG_STAT_PORT &= ~(1<<CHRG_STAT);
	if( bit_is_set( CHRG_STAT_PIN, CHRG_STAT ) )
		result2 = 1;

	if( (result1 == result2) == 1 )	// charge complete
		return 0;
	if( (result1 == result2) == 0 )	// preconditioning / charging
		return 1;
	if( result1 != result2 )		// shutdown
		return 2;
}

