#ifndef SSD1306_H_
#define SSD1306_H_

#include <stdint.h>

#define FONT_WIDTH 5
#define FONT_HEIGHT 7

/* ******************** CONFIG ******************** */

#define SSD1306_128_64
/* #define SSD1306_128_32 */

#define USE_SECOND_DISPLAY 0		/* 1 - two displays, 0 - one display */

#define USE_SPI_OR_I2C 1			/* 1 - SPI, 0 - I2C */

/* ******************** I2C ******************** */
#if USE_SPI_OR_I2C == 0

#define SSD1306_I2C_ADDRESS 0x78	/* albo 0x7A */

#define I2C_DDR DDRC
#define SCL PC5
#define SDA PC4

#define ACK 1
#define NACK 0

#endif

/* ******************** SPI ******************** */
#if USE_SPI_OR_I2C == 1

#define USE_CS	1		/* działa z 0 */
#define USE_RST	1

#define SCK (1<<5)		/* 5 */
#define MOSI (1<<3)		/* 3 */
#define RST (1<<1)		/* 2 */
#define DC (1<<2)		/* 1 */
#define CS (1<<0)		/* 2 */

#if USE_SECOND_DISPLAY == 1
#define CS2 (1<<4)		/* *opcjonalne* dla drugiego wyswietlacza */
#endif

#define SCK_PORT	PORTB
#define SCK_DDR		DDRB

#define MOSI_PORT	PORTB
#define MOSI_DDR	DDRB

#define RST_PORT	PORTB
#define RST_DDR		DDRB

#define DC_PORT		PORTB
#define DC_DDR		DDRB

#define CS_PORT		PORTB
#define CS_DDR		DDRB

#if USE_SECOND_DISPLAY == 1
#define CS2_PORT		PORTB
#define CS2_DDR		DDRB
#endif

#define RST_LO	RST_PORT &= ~RST
#define RST_HI	RST_PORT |= RST

#define CS_LO	CS_PORT &= ~CS
#define CS_HI	CS_PORT |= CS

#if USE_SECOND_DISPLAY == 1
#define CS2_LO	CS2_PORT &= ~CS2
#define CS2_HI	CS2_PORT |= CS2
#endif

#define DC_LO	DC_PORT &= ~DC
#define DC_HI	DC_PORT |= DC

#define SCK_LO	SCK_PORT &= ~SCK
#define SCK_HI	SCK_PORT |= SCK

#define MOSI_LO	MOSI_PORT &= ~MOSI
#define MOSI_HI	MOSI_PORT |= MOSI

#endif

#define SSD1306_WIDTH	128

/* ******************** END OF CONFIG ******************** */

#if defined SSD1306_128_64
#define SSD1306_HEIGHT	64
#endif
#if defined SSD1306_128_32
#define SSD1306_HEIGTH	32
#endif

#define SSD1306_BUF_SIZE ( SSD1306_WIDTH * SSD1306_HEIGHT / 8 )

#define REFRESH_MIN 0x80
#define REFRESH_MID 0xB0
#define REFRESH_MAX 0xF0

typedef enum {page0, page1, page2, page3, page4, page5, page6, page7} TPAGE;

#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

#define swap( a, b ) { uint16_t t = a; a = b; b = t; }

/*	deklaracje zmiennych	*/

#if USE_SECOND_DISPLAY == 1
extern uint8_t display_nr;
#endif

extern const uint8_t font[];
extern int cursor_x, cursor_y;

// Not used, see comment for ssd1306_move() in "graphics.c"
// Added by Szymon Kajda, used by "graphics.c"/ssd1306_move()
//extern uint8_t ssd1306_buf[ SSD1306_BUF_SIZE ];

/*	deklaracje funkcji		*/

#if USE_SECOND_DISPLAY == 1
void ssd1306_init( uint8_t vcc, uint8_t refresh, uint8_t rst_disable );
#else
void ssd1306_init( uint8_t vcc, uint8_t refresh );
#endif
void ssd1306_cmd( uint8_t cmd );
void ssd1306_data( uint8_t dat );

void ssd1306_display( void );
void ssd1306_refresh_pages( uint8_t page_nr, uint8_t pages_cnt, uint8_t col_start, uint8_t col_end );
void ssd1306_setPixel( int x, int y, uint8_t bw );
void ssd1306_cls( void );

void ssd1306_drawBitmap_P( int x, int y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color, uint8_t bg );
void ssd1306_drawChar( int x, int y, char c, uint8_t color, uint8_t bg, uint8_t size );
void ssd1306_puts( int x, int y, char * str, uint8_t txt_size, uint8_t color, uint8_t bg );
void ssd1306_puts_P( int x, int y, const char * str, uint8_t txt_size, uint8_t color, uint8_t bg );
// Added by Szymon Kajda
void ssd1306_put_int( int x, int y, int data, uint8_t txt_size, uint8_t color, uint8_t bg );
void ssd1306_put_int_zeroes( int x, int y, int data, uint8_t number_of_zeroes, uint8_t txt_size, uint8_t color, uint8_t bg );
// "num" has to look like like: (float with 2 dec places)*100
//void ssd1306_put_float( int x, int y, uint16_t num, uint8_t size, uint8_t color, uint8_t bg );
// Not used, see comment for ssd1306_move() in "graphics.c"
//void ssd1306_move( int x0, int y0, uint8_t w, uint8_t h, int dest_x, int dest_y, uint8_t mode );

void ssd1306_drawFastVLine( int x, int y, uint8_t h, uint8_t color );
void ssd1306_drawFastHLine( int x, int y, uint8_t w, uint8_t color );
void ssd1306_drawLine( int x0, int y0, int x1, int y1, uint8_t color );

void ssd1306_drawCircle( int x, int y, uint8_t r, uint8_t bw );
void ssd1306_drawFillCircle( int x, int y, uint8_t r, uint8_t color );
void ssd1306_fillCircleHelper( int x0, int y0, int r, uint8_t cornername, int delta, uint8_t color );

void ssd1306_drawRect( int x, int y, uint8_t w, uint8_t h, uint8_t color );
void ssd1306_drawRoundRect( int x, int y, uint8_t w, uint8_t h, uint8_t r, uint8_t color );
void ssd1306_drawFillRect( int x, int y, int w, int h, uint8_t color );
void ssd1306_drawFillRoundRect( int x, int y, uint8_t w, uint8_t h, uint8_t r, uint8_t color );
void ssd1306_drawCircleHelper( int x0, int y0, uint8_t r, uint8_t cornername, uint8_t color );

void ssd1306_drawTriangle( int x0, int y0, int x1, int y1, int x2, int y2, uint8_t color );
void ssd1306_drawFillTriangle( int x0, int y0, int x1, int y1, int x2, int y2, uint8_t color );

void ssd1306_startScrollRight( uint8_t start, uint8_t stop );
void ssd1306_startScrollLeft( uint8_t start, uint8_t stop );
void ssd1306_startScrollDiaRight( uint8_t start, uint8_t stop );
void ssd1306_startScrollDiaLeft( uint8_t start, uint8_t stop );
void ssd1306_stopScroll( void );
void ssd1306_invertDisplay( uint8_t i );

// Added by Szymon Kajda
void tostring(char str[], uint16_t num);
void tostring_zeroes(char str[], uint16_t num, uint8_t len);

#endif /* SSD1306_H_ */
