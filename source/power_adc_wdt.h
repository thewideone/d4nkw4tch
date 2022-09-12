/*
 * power_adc_wdt.h
 *
 *  Created on: 20 maj 2021
 *      Author: Szymon Kajda
 */

#ifndef _POWER_ADC_WDT_H_
#define _POWER_ADC_WDT_H_

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>

#include "pin_desc.h"
#include "date_time.h"

typedef enum { SHUTDOWN_NO_BATT, CHARGING, COMPLETE_STANDBY } ChargerState;

#define BATTERY_80 252
#define BATTERY_60 242
#define BATTERY_40 238
#define BATTERY_20 233

/*set_sleep_mode(SLEEP_MODE_PWR_DOWN);*/
#define power_down SMCR |= (1<<SM1)
/*sleep_enable();*/
#define sleep_enable SMCR |= (1<<SE)
/*sleep_cpu();*/
#define sleep_cpu __asm__ __volatile__ ( "sleep" "\n\t" :: )
/*sleep_disable();*/
#define sleep_disable SMCR &= ~(1<<SE)

#define toggle_bit( byte, bit ) byte ^= (1<<bit)

// 0 - pwr saving mode; 1 - invert display
//uint8_t bitSettings;// = 0b11111111; <- assigned in power_adc_wdt.c on FIRST_BOOT
// 		bitSettingsEEMEM in power_adc_wdt.c
// Inverted logic (set to 0 to turn on)
#define PWR_SAVING_MODE_BIT 0
#define INV_DISPLAY_BIT 1
#define BUTTON_SOUND_BIT 2	// 0 - beep, 1 - click
#define ASTRONAUT_MENU_BIT 3

// Functions:
void readVcc( void );
uint8_t batteryLvl( void );
uint8_t isCharging( void );
uint8_t getChargerState( void );
uint8_t isUSBPlugged( void );
void handlePowerSettings( uint8_t bitSettings );
// Inverted logic (set to 0 to turn on)
uint8_t readBitSettings( void );
void updateBitSettings( uint8_t bitSettings );

// Inline functions only in header file:
inline void setupADC( void ){
	//ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

inline void setupBattAndPlugSense( void ){
	BATT_EN_DDR |= (1<<BATT_EN);
	BATT_EN_PORT &= ~(1<<BATT_EN);		// Turn off "voltage measurer" to save energy
	CHRG_MODE_DDR |= (1<<CHRG_MODE);
	//CHRG_MODE_PORT |= (1<<CHRG_MODE);	// High current default, turn to 0 to select low current charging
	CHRG_STAT_DDR &= ~(1<<CHRG_STAT);	// BATT STAT
	CHRG_STAT_PORT |= (1<<CHRG_STAT);	// Enable pull-up
	USB_SENSE_DDR &= ~(1<<USB_SENSE);	// USB PLUG SENSE
}


#define wdt_disable_mod() \
	__asm__ __volatile__ ( \
	   "in __tmp_reg__, __SREG__" "\n\t" \
	   "cli" "\n\t" \
	   "wdr" "\n\t" \
	   "sts %0, %1" "\n\t" \
	   "sts %0, __zero_reg__" "\n\t" \
	   "out __SREG__,__tmp_reg__" "\n\t" \
	   : /* no outputs */ \
	   : "M" (_SFR_IO_ADDR(_WD_CONTROL_REG)), \
	   "r" ((uint8_t)(_BV(_WD_CHANGE_BIT) | _BV(WDE))) \
	   : "r0" \
	)

#endif /* _POWER_ADC_WDT_H_ */
