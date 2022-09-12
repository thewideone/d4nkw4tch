#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include <stdint.h>

#include <stdlib.h>
#include <avr/wdt.h>

#include "pin_desc.h"
#include "scale8.h"

#include "bitmaps/power_bitmaps.h"
#include "button_debounce/debounce.h"
#include "BQ32002_lib/bq32002.h"
#include "I2C_TWI/i2c_twi.h"
#include "oled_lib/ssd1306.h"
#include "uart_lib/uart.h"

//#include "ws2812b_lib/ws2812b.h"
#include "led.h"
#include "button_debounce/buttons.h"
#include "buzzer.h"
#include "date_time.h"
#include "power_adc_wdt.h"
#include "anims/menu_pointers.h"
#include "anims/misc.h"
#include "menus.h"

//#define STAY_UP_CONDITIONS ( (datetime.ss-wakeUpTime) < stayUpTime || \
//							bit_is_set( PCIFR,PCIF1 ) || \
//							menu == menu_flashlight || \
//							menu == menu_bt_teminal || \
//							menu == menu_set_date_and_time)

//extern int8_t menuArrowAnimDir;

extern volatile button_t button;	// 1-BOT 2-CLICK 3-TOP
extern uint8_t buttons_hold_mode;
//extern volatile TBUZZER buzzer; // = ON;
//volatile TBUZZER buzer_state;
extern volatile TMENU menu;		// = menu_main;

extern volatile uint8_t stayUpTime;// = STAY_UP_TIME;

extern volatile uint8_t wakeUpTime;
//extern volatile uint8_t charging;
//volatile uint8_t charging;
//extern volatile long batt_lvl;
//volatile uint16_t batt_lvl;
//volatile uint8_t batt_lvl;

uint8_t bit_settings;	// description in power_adc_wdt.h

//extern int8_t menuArrowAnimDir;
//extern uint8_t menuArrowStepCount;
//extern uint8_t menuArrowMoving;


int displayTimeXPos=16;



//char bt_buf[BT_BUF_LENGTH];
//uint8_t bt_buf_index = 0;
//extern uint8_t bt_terminal_cursor_x=65;
//extern uint8_t bt_terminal_cursor_y=0;

//extern uint8_t bitSettings;
//extern uint8_t bitSettingsEEMEM EEMEM;

/*
struct small_int{
	unsigned var : 2;
};
struct small_int appCursor = {.var = 0};
*/
/*
	TODO:
	- dodac wyswietlanie zera w tostring() w ssd1306_put_int()
	- dzien tygodnia wychodzi poza days[][] w poniedzialki
	- po obudzeniu obraz jest wyświetlany dopiero przy zmianie stanu rtc int - poprawić
	- watchdog wyłącza zegarek gdy się usypia i jest uspany!
	- low power consumption using DIDR0 register https://www.avrfreaks.net/forum/tristate-input-what-use-them
	- add some flexibility to arrow animations because for example the arrow doesn't roll in menu_set_date_and_time
	- arrow roll in menu_settings & menu_sound_settings & volume...
	- in menu_set_date_time arrow doesn't point directly on values
	- X change menu_cursor_x's to appCursor etc (and change their name), all the menus have some common variables
	- X auto power saving mode when Vcc low
	- turn off BLE LED option: "AT+PIO11" - 1: unconnected: out low, connected: out high
	- connect RGB led to 5V (3.5V<Vcc<5.3V)
	- 68Ohm resistor for brighter led
	- ((datetime.ss-wakeUpTime) < stayUpTime) in main loop not working when minute changes
	- X poll RTC only if connected to RTC INT pin's state's changed
	- zrobić efekt wyłączania starego TV przed wyłączeniem się ekranu + może trochę glitchu?, w Aseprite
	- funkcja przenoszaca fragmenty obrazu (przenoszenie stralki w menu jest szybsze niz rysowanie od nowa
											+ strzalka nie musi miec ramki z czarnych pixeli)
	-code optimalization
	-ograniczenia kursora
	-OCR0B (i nie tylko) value into EEPROM
	-uint8_t bt_enabled = 0/1 in EEPROM
	-screen sometimes stays up (maybe add cli() in some places), see this: https://www.norwegiancreations.com/2018/10/arduino-tutorial-avoiding-the-overflow-issue-when-using-millis-and-micros/
	- X RTC interrupt handle
	- X new pcbs - change power connections
	- X usb plug sense
	-time in different timezones
	-make bt string transmission (BLE allows for transmission of 20 bytes max at once)
	-make bit and predator clock
	-make starship game
	-make game of life
	- pong with changing player after bounce

	- make a power wire with ammeter
	- the screen seems not to be broken even when it doesn't light up for a while
	- capacitor / resistor values from the website (https://learn.adafruit.com/monochrome-oled-breakouts/downloads)
	- schematic http://wiki.sunfounder.cc/index.php?title=OLED-SSD1306_Module
	- schematic https://wiki.52pi.com/index.php/0.91_OLED_(English)
	- schematic https://wiki.banana-pi.org/BPI_OLED_Display_Module
		- SCK and MOSI 10k pull-ups
	- desolder speaker from 5.0V?
	- desolder battery charger
	- disconnect D3-D7 ?

	Notes:
	Console colour in Eclipse can be changed in Window > Preferences > C/C++ > Build > Console
*/

// Section .init3 is used in this example, as this ensures the internal
// __zero_reg__ has already been set up. The code generated by the compiler
// might blindly rely on __zero_reg__ being really 0.
// In other words, in C programs .init2 is weakly bound to initialize the stack,
// and to clear zero_reg (r1), so .init3 seems like a safe place to put this code into.
// https://www.nongnu.org/avr-libc/user-manual/mem_sections.html
void __init3( void ) __attribute__ (( section( ".init3" ), naked, used ));
void __init3( void )
{
    /* wyłączenie watchdoga (w tych mikrokontrolerach, w których watchdog
     * ma możliwość generowania przerwania pozostaje on też aktywny po
     * resecie) */

    MCUSR = 0;
    WDTCSR = (1<<WDCE) | (1<<WDE);
    WDTCSR = 0;
}

int main(){

	/* INIT SECTION */
	wdt_enable(WDTO_2S);
	uart_init(UBRRVAL);	// BT off on startup?
//	tx_string( "UART setup done.\r\n" );
	ssd1306_init( SSD1306_SWITCHCAPVCC, REFRESH_MAX );
//	tx_string( "Display setup done.\r\n" );
//	LED_ON;
//	DS3231_init();
//	LED2_ON;

	setupADC();
	// I/O SETUP
	setupLED();
	setupBuzzer();	// uses Timer0
	setupButtons();	// uses Timer2
	setButtonsHoldOff();

	setupBattAndPlugSense();
	//DDRD &= ~(1<<PD2);	// INT0 (RTC interrupt)
	//PORTD |= (1<<PD2);	// INT0
	// end of I/O SETUP

	// INTERRUPTS SETUP
	PCICR |= (1 << PCIE1) | (1 << PCIE2);				// BUTTON & BATT STAT
	PCMSK1 |= (1<<PCINT11) | (1<<PCINT10) | (1<<PCINT9);	// BUTTON
	PCMSK2 |= (1<<PCINT20);//|(1<<PCINT19);	// BATT STAT

	RTC_INT_DDR &= ~(1<<RTC_INT);		//
	RTC_INT_PORT |= (1<<RTC_INT);		// enable pull-up

//	tx_string( "IO setup done.\r\n" );

	//stayUpTime = STAY_UP_TIME;

	BQ32002_init();
	BQ32002_disableOsc();
	BQ32002_enableOsc();
	BQ32002_setCalFreq( 1 );	// 1Hz output
	BQ32002_enableFreqTest();
//	tx_string( "RTC setup done.\r\n" );

	sei();

	while(1){
		/*	Sleep	*/
//		tx_string( "Going into sleep mode.\r\n" );
		//tx_string("AT+SLEEP");
		//BATT_EN_PORT &= ~(1<<BATT_EN);	// Disable the divider
		ssd1306_cmd( SSD1306_DISPLAYOFF );
		//LED_OFF;
		//wdt_disable_mod();			// Disable Watchdog, so it won't reset MCU when it's asleep
		power_down;
		sleep_enable;
		sei();
		sleep_cpu;
		sleep_disable;

		/* Wake up */
//		tx_string( "Waking up.\r\n" );
		//wdt_enable(WDTO_4S);	// Enable Watchdog
		ssd1306_init( SSD1306_SWITCHCAPVCC, REFRESH_MAX );
		ssd1306_cmd( SSD1306_DISPLAYON );

		bit_settings = readBitSettings();

		if( bit_is_set( bit_settings, BUTTON_SOUND_BIT ) )
			OCR0B = BUTTON_CLICK_OCR0B_VALUE;
		else
			OCR0B = 250;	// put that into PROGMEM
		//ssd1306_invertDisplay( !(bitSettings & (1<<invDisplayBit)) );
		// Just for testing commented readVcc()
		// To restore, uncomment readVcc() and remove "= BATTERY_80" from power_adc_wdt.c
		readVcc();
		handlePowerSettings( bit_settings );

		ssd1306_cls();
		menu = menu_main;
		// READ DATA FROM RTC
		// while(temp <= 0):
		//do{

		// working till now

		/// Temporarily added
//		while( buttonState() == NONE ){
//			handleButtons( buttons_hold_mode );
//			if( buttonState() == TOP ){
//				bluetoothTurnOn();
//			}
//			else if( buttonState() == BOT ){
//				bluetoothTurnOff();
//				LED2_ON;
//			}
//		}
//		LED_ON;
		///

//		DS3231_get_temp( &temperature );

		//	_delay_ms(2);
		//}while( temperature.cel <= 0 );
//		DS3231_get_datetime( &datetime );
//		LED_ON;
		//readSqwState();

		BQ32002_getDateTime( &datetime );
		gotoMenu( menu_main );
		wakeUpTime = datetime.ss;

		tx_string( "Time update done, going into the while loop.\r\n" );


		sei();

		/*	Main loop	*/
//		while( (datetime.ss-wakeUpTime) < stayUpTime ){
//		while( (datetime.ss-wakeUpTime) < stayUpTime || bit_is_set( PCIFR,PCIF1 ) ||
//				menu == menu_flashlight || menu == menu_bt_terminal || menu == menu_set_date_and_time ||
//				menu == menu_bt ){
//		while( (datetime.ss-wakeUpTime) < stayUpTime || bit_is_set( PCIFR,PCIF1 ) ||
//			   ( (menu == menu_flashlight || menu == menu_bt_terminal || menu == menu_set_date_and_time ||
//				  menu == menu_bt || menu == menu_apps) && (datetime.ss-wakeUpTime) < stayUpTime*4) ){
		while( abs(datetime.ss-wakeUpTime) < stayUpTime || bit_is_set( PCIFR,PCIF1 ) ||
			   ( (menu != menu_main) && abs(datetime.ss-wakeUpTime) < stayUpTime*3 )    ){
			wdt_reset();
			//DS3231_get_datetime( &datetime );
			getDateTime( &datetime );
//			BQ32002_getDateTime( &datetime );
//			tx_string( "Updated time: " );
//			tx_string( datetime.time );
//			tx_string( "\r\n" );
			//getTemp( &temperature );

			//buttonForceRelease();	// put into handleButtons()
			handleButtons();

			if( buzzerState() == ON && button )
				buzzerEnable();
			else buzzerDisable();

//			tx_string( "Updated buttons and buzzer.\r\n" );

			if( sqwStateChanged() )
				LED2_TOGGLE;
//			if( getButtonState() == TOP )
//				LED_ON;
//			else
//				LED_OFF;

//			tx_string( "Going into menu switch.\r\n" );

			switch(menu){
				default:
					handleMenuDefault( &datetime, wakeUpTime, stayUpTime );
					break;
/* MAIN MENU */	case menu_main:
//					getDateTime( &datetime );
					if( bit_is_clear( bit_settings, ASTRONAUT_MENU_BIT ) )
						handleMenuMain( &datetime );
					else
						handleMenuMainAstronauts( &datetime );
					break;
/* APP MENU */	case menu_apps:
					handleMenuApps();
					break;
/* FLASHLIGHT */case menu_flashlight:
					if( button == PRESS ){
						LED_OFF;
						LED2_OFF;
						//ssd1306_invertDisplay(0);
						handlePowerSettings( bit_settings );
						gotoMenu( menu_apps );
					}
					break;
/*   MENU 	*/	case menu_settings:
/* SETTINGS */		handleMenuSettings( &bit_settings );
/*   MENU 	*/		break;
/* LOUDNESS */	case menu_sound_settings:
					handleMenuSoundSettings();
/*  MENU 	*/		break;
				case menu_volume:
					handleMenuVolume();
					break;
/* SET D&T */	case menu_set_date_and_time:
					getDateTime( &datetime );
					handleMenuSetDateTime();
					break;
/*BT TERMINAL*/	case menu_bt_terminal:
					handleMenuBtTerminal();
					break;
/*  MENU BT  */	case menu_bt:
					handleMenuBt();
					break;
			}
			ssd1306_display();
			updateSqwState();
		}
		// If not in power-saving mode
		if( bit_is_set( bit_settings, PWR_SAVING_MODE_BIT ) ){
			wdt_reset();
			anim_display_off();	// also includes wdt_reset()
		}
	}
}

/*
void debounce_button( void ){
	uint8_t count = 0;
	uint8_t button_state = 0;

	if(button != button_state){
		count++;
		if(count>=4){
			button_state = button;
			if(button != NONE)
				button = BOT; // 4 example bot
			count = 0;
		}
	}
	else count = 0;
}
*/

/* BUTTONS INTERRUPT */ // <- really?? "debounce()" handled in TIMER2_OVF_vect
ISR(PCINT1_vect){
	wakeUpTime = datetime.ss;
	/*
	if (bit_is_clear(PINC,PC0)){		// BOTTOM
		_delay_ms(40);
		if (bit_is_clear(PINC,PC0))
			button = BOT;
	}
	else if (bit_is_clear(PINC,PC1)){	// MIDDLE
		_delay_ms(40);
		if (bit_is_clear(PINC,PC1))
			button = PRESS;
	}
	else if (bit_is_clear(PINC,PC2)){	// TOP
		_delay_ms(40);
		if (bit_is_clear(PINC,PC2))
			button = TOP;
	}
	else button = NONE;
	*/
}

/* CHARGER INTERRUPT */
ISR(PCINT2_vect){
	wakeUpTime = datetime.ss;
}
