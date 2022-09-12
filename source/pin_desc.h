/*
 * pin_desc.c
 *
 *  Created on: Aug 24, 2020
 *      Author: szymon
 */
#ifndef PIN_DESC_H
#define PIN_DESC_H

#include <avr/io.h>

// Buttons connected to PC0, PC1 and PC2
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define BUTTON_DDR DDRC
#define BUTTON1_MASK (1<<PC1)
#define BUTTON2_MASK (1<<PC2)
#define BUTTON3_MASK (1<<PC3)

#define BATT_EN_DDR DDRC
#define BATT_EN_PORT PORTC
#define BATT_EN PC0

#define BUZZER_DDR DDRD
#define BUZZER_PORT PORTD
#define BUZZER_PIN PD5

// Not used since replacement of DS3231 by BQ32002
#define RTC_32KHZ_DDR DDRB
#define RTC_32KHZ_PORT PORTB
#define RTC_32KHZ_PIN PINB
#define RTC_32KHZ PB6

#define LED2_DDR DDRB
#define LED2_PORT PORTB
#define LED2_PIN PB7

#define LED_DDR DDRD
#define LED_PORT PORTD
#define LED_PIN PD6

#define RTC_INT_DDR DDRD
#define RTC_INT_PORT PORTD
#define RTC_INT_PIN PIND
#define RTC_INT PD2

#define CHRG_MODE_DDR DDRD
#define CHRG_MODE_PORT PORTD
#define CHRG_MODE PD7

#define CHRG_STAT_DDR DDRD
#define CHRG_STAT_PORT PORTD
#define CHRG_STAT_PIN PIND
#define CHRG_STAT PD4

#define USB_SENSE_DDR DDRD
#define USB_SENSE_PORT PORTD
#define USB_SENSE_PIN PIND
#define USB_SENSE PD3

#endif
