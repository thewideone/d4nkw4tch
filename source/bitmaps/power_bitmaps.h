#include <avr/io.h>
#include <avr/pgmspace.h>

#ifndef _POWER_BITMAPS_H_
#define _POWER_BITMAPS_H_

/*	POWER BITMAPS	*/
#define USB_BMP_WIDTH 16
#define USB_BMP_HEIGHT 7
extern const uint8_t usb[] PROGMEM;

#define CHRG_BMP_WIDTH 4
#define CHRG_BMP_HEIGHT 7
extern const uint8_t chrg[] PROGMEM;

#define BATT_BMP_WIDTH 11
#define BATT_BMP_HEIGHT 7
extern const uint8_t batt_0[] PROGMEM;
extern const uint8_t batt_1[] PROGMEM;
extern const uint8_t batt_2[] PROGMEM;
extern const uint8_t batt_3[] PROGMEM;
extern const uint8_t batt_4[] PROGMEM;

#endif	/* _POWER_BITMAPS_H_ */
