#include "ssd1306.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>


uint8_t ssd1306_buf[ SSD1306_BUF_SIZE ] = {
};

#if USE_SECOND_DISPLAY == 1
uint8_t display_nr;
#endif

/* **************************************** SPI **************************************** */

#if USE_SPI_OR_I2C == 1
static void SPIwrite( uint8_t dat ){
	uint8_t i;
	for (i= 0x80;i;i >>= 1){
		SCK_LO;
		if (dat & i) MOSI_HI;
		else MOSI_LO;
		SCK_HI;
	}
}

void ssd1306_InitSpi( void ){
	MOSI_DDR |= MOSI;
	SCK_DDR |= SCK;

#if USE_RST == 1
	RST_DDR |= RST;
	RST_PORT |= RST;
#endif

	DC_DDR |= DC;

#if USE_CS == 1
	CS_DDR |= CS;
	CS_PORT |= CS;
#endif

#if USE_SECOND_DISPLAY == 1
	CS2_DDR |= CS2;
	CS2_PORT |= CS2;
#endif
}
#endif

/* **************************************** I2C **************************************** */

#if USE_SPI_OR_I2C == 0
void i2cSetBitrate(uint16_t bitrateKHz) {
	uint8_t bitrate_div;

	bitrate_div = ((F_CPU/1000l)/bitrateKHz);
	if(bitrate_div >= 16)
		bitrate_div = (bitrate_div-16)/2;

	TWBR = bitrate_div;
}

void ssd1306_TWI_start(void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTA);
	while (!(TWCR&(1<<TWINT)));
}

void ssd1306_TWI_stop(void) {
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while ( (TWCR&(1<<TWSTO)) );
}

void ssd1306_TWI_write(uint8_t bajt) {
	TWDR = bajt;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ( !(TWCR&(1<<TWINT)));
}

uint8_t ssd1306_TWI_read(uint8_t ack) {
	TWCR = (1<<TWINT)|(ack<<TWEA)|(1<<TWEN);
	while ( !(TWCR & (1<<TWINT)));
	return TWDR;
}

void ssd1306_TWI_write_buf( uint8_t SLA, uint8_t adr, uint16_t len, uint8_t *buf ) {

	ssd1306_TWI_start();
	ssd1306_TWI_write(SLA);
	ssd1306_TWI_write(adr);
	while (len--) ssd1306_TWI_write(*buf++);
	ssd1306_TWI_stop();
}

void ssd1306_TWI_read_buf(uint8_t SLA, uint8_t adr, uint8_t len, uint8_t *buf) {

	ssd1306_TWI_start();
	ssd1306_TWI_write(SLA);
	ssd1306_TWI_write(adr);
	ssd1306_TWI_start();
	ssd1306_TWI_write(SLA + 1);
	while (len--) *buf++ = ssd1306_TWI_read( len ? ACK : NACK );
	ssd1306_TWI_stop();
}
#endif

/* **************************************** END OF I2C **************************************** */


void ssd1306_cmd( uint8_t cmd ){
#if USE_SPI_OR_I2C == 1

#if USE_CS == 1
	#if USE_SECOND_DISPLAY == 0
		CS_HI;
	#else
		if ( !display_nr )
			CS_HI;
		else CS2_HI;
	#endif
#endif

	DC_LO;

#if USE_CS == 1
	#if USE_SECOND_DISPLAY == 0
		CS_LO;
	#else
		if ( !display_nr )
			CS_LO;
		else CS2_LO;
	#endif
#endif

	SPIwrite( cmd );

#if USE_CS == 1
	#if USE_SECOND_DISPLAY == 0
		CS_HI;
	#else
		if ( !display_nr )
			CS_HI;
		else CS2_HI;
	#endif
#endif

#else

	uint8_t control = 0x00;
	ssd1306_TWI_start();
	ssd1306_TWI_write( SSD1306_I2C_ADDRESS );
	ssd1306_TWI_write( control );
	ssd1306_TWI_write( cmd );
	ssd1306_TWI_stop();

#endif
}

void ssd1306_data( uint8_t dat ){
#if USE_SPI_OR_I2C == 1

#if USE_CS == 1
	#if USE_SECOND_DISPLAY == 0
		CS_HI;
	#else
		if ( !display_nr )
			CS_HI;
		else CS2_HI;
	#endif
#endif

	DC_HI;

#if USE_CS == 1
	#if USE_SECOND_DISPLAY == 0
		CS_LO;
	#else
		if ( !display_nr )
			CS_LO;
		else CS2_LO;
	#endif
#endif

	SPIwrite( dat );

#if USE_CS == 1
	#if USE_SECOND_DISPLAY == 0
		CS_HI;
	#else
		if ( !display_nr )
			CS_HI;
		else CS2_HI;
	#endif
#endif

#else

	uint8_t control = 0x40;
	ssd1306_TWI_start();
	ssd1306_TWI_write( SSD1306_I2C_ADDRESS );
	ssd1306_TWI_write( control );
	ssd1306_TWI_write( dat );
	ssd1306_TWI_stop();

#endif
}

void ssd1306_display( void ){
	cli();
	ssd1306_cmd( SSD1306_SETLOWCOLUMN	| 0x0 );
	ssd1306_cmd( SSD1306_SETHIGHCOLUMN	| 0x0 );
	ssd1306_cmd( SSD1306_SETSTARTLINE	| 0x0 );

#if USE_SPI_OR_I2C == 1

#if USE_CS == 1
	#if USE_SECOND_DISPLAY == 0
		CS_HI;
	#else
		if ( !display_nr )
			CS_HI;
		else CS2_HI;
	#endif
#endif

	DC_HI;

#if USE_CS == 1
	#if USE_SECOND_DISPLAY == 0
		CS_LO;
	#else
		if ( !display_nr )
			CS_LO;
		else CS2_LO;
	#endif
#endif

	for ( uint16_t i=0; i<(SSD1306_WIDTH*SSD1306_HEIGHT/8); i++ ){
		SPIwrite( ssd1306_buf[i] );
	}

#if USE_CS == 1
	#if USE_SECOND_DISPLAY == 0
		CS_HI;
	#else
		if ( !display_nr )
			CS_HI;
		else CS2_HI;
	#endif
#endif
#else

	ssd1306_TWI_write_buf( SSD1306_I2C_ADDRESS, 0x40, SSD1306_BUF_SIZE, ssd1306_buf );

#endif
	sei();
}

void ssd1306_setPixel( int x, int y, uint8_t bw ){
	if ( (x < 0) || (x >= SSD1306_WIDTH) || (y < 0) || (y >= SSD1306_HEIGHT) )
		return;

	if (bw) ssd1306_buf[x + (y/8) * SSD1306_WIDTH] |= (1<<(y%8));
	else ssd1306_buf[x + (y/8) * SSD1306_WIDTH] &= ~(1<<(y%8));
}

void ssd1306_cls( void ){
	for (uint16_t n=0;n<SSD1306_BUF_SIZE;n++){
		ssd1306_buf[n] = 0x00;
	}
	/* memset( ssd1306_buf,0x00, ( SSD1306_BUF_SIZE ) ); */
}

#if USE_SECOND_DISPLAY == 1
void ssd1306_init( uint8_t vcc, uint8_t refresh, uint8_t rst_disable ){
#else
void ssd1306_init( uint8_t vcc, uint8_t refresh ){
#endif

#if USE_SPI_OR_I2C == 1

	ssd1306_InitSpi();

#if USE_SECOND_DISPLAY == 1
	if ( !rst_disable ){
#endif
	#if USE_RST == 1
		RST_HI;
		_delay_ms(25);
		RST_LO;
		_delay_ms(25);
		RST_HI;
	#endif
#if USE_SECOND_DISPLAY == 1
	}
#endif
#else

	i2cSetBitrate( 400 );
	I2C_DDR |= (1<<SCL)|(1<<SDA);
	_delay_ms(50);

#endif

	ssd1306_cmd( SSD1306_DISPLAYOFF );
	ssd1306_cmd( SSD1306_SETDISPLAYCLOCKDIV );
	ssd1306_cmd( refresh );

	ssd1306_cmd( SSD1306_SETDISPLAYOFFSET );
	ssd1306_cmd( 0x00 );
	ssd1306_cmd( SSD1306_SETSTARTLINE | 0x0 );
	ssd1306_cmd( SSD1306_CHARGEPUMP );

	if ( vcc == SSD1306_EXTERNALVCC ) ssd1306_cmd( 0x10 );
	else ssd1306_cmd( 0x14 );

	ssd1306_cmd( SSD1306_MEMORYMODE );
	ssd1306_cmd( 0x00 );
	ssd1306_cmd( SSD1306_SEGREMAP | 0x1 );
	ssd1306_cmd( SSD1306_COMSCANDEC );

	ssd1306_cmd( SSD1306_SETCONTRAST );

	if ( vcc == SSD1306_EXTERNALVCC ) ssd1306_cmd( 0x9F );
	else ssd1306_cmd( 0xCF );

	ssd1306_cmd( SSD1306_SETPRECHARGE );
	//ssd1306_cmd( 0x22 );	// added on 10_12_2021

#if defined SSD1306_128_32
	ssd1306_cmd( SSD1306_SETMULTIPLEX );
	ssd1306_cmd( 0x1F );

	ssd1306_cmd( SSD1306_SETCOMPINS );
	ssd1306_cmd( 0x02 );
#endif

#if defined SSD1306_128_64
	ssd1306_cmd( SSD1306_SETMULTIPLEX );
	ssd1306_cmd( 0x3F );

	ssd1306_cmd( SSD1306_SETCOMPINS );
	ssd1306_cmd( 0x12 );
#endif

	ssd1306_cmd( SSD1306_SETVCOMDETECT );
	ssd1306_cmd( 0x40 );	// was 0x40 10_12_2021

	ssd1306_cmd( SSD1306_DISPLAYALLON_RESUME );
	ssd1306_cmd( SSD1306_NORMALDISPLAY );

	ssd1306_cmd( SSD1306_DISPLAYON );

	ssd1306_display();

}

void ssd1306_refresh_pages( uint8_t page_nr, uint8_t pages_cnt, uint8_t col_start, uint8_t col_end ){

	uint8_t page_cnt, col_cnt;
	uint8_t * ram_buf_start;

	for( page_cnt=page_nr; page_cnt<(page_nr+pages_cnt); page_cnt++ ){

		ssd1306_cmd( SSD1306_SETLOWCOLUMN  | ( col_start & 0x0F ) );
		ssd1306_cmd( SSD1306_SETHIGHCOLUMN | col_start>>4 );
		ssd1306_cmd( 0xB0 + page_cnt );

		ram_buf_start = &ssd1306_buf[ (page_cnt*128) + col_start ];

		DC_HI;

		#if USE_CS == 1
			#if USE_SECOND_DISPLAY == 0
				CS_LO;
			#else
				if( !display_nr )
					CS_LO;
				else
					CS2_LO;
			#endif
		#endif

		for( col_cnt=col_start; col_cnt<col_end; col_cnt++ ){
			SPIwrite( *ram_buf_start++ );
		}
	}
}

void ssd1306_startScrollRight( uint8_t start, uint8_t stop ){
	ssd1306_cmd( SSD1306_RIGHT_HORIZONTAL_SCROLL );
	ssd1306_cmd( 0x00 );
	ssd1306_cmd( start );
	ssd1306_cmd( 0x00 );
	ssd1306_cmd( stop );
	ssd1306_cmd( 0x01 );
	ssd1306_cmd( 0xFF );
	ssd1306_cmd( SSD1306_ACTIVATE_SCROLL );
}

void ssd1306_startScrollLeft( uint8_t start, uint8_t stop ){
	ssd1306_cmd( SSD1306_LEFT_HORIZONTAL_SCROLL );
	ssd1306_cmd( 0x00 );
	ssd1306_cmd( start );
	ssd1306_cmd( 0x00 );
	ssd1306_cmd( stop );
	ssd1306_cmd( 0x01 );
	ssd1306_cmd( 0xFF );
	ssd1306_cmd( SSD1306_ACTIVATE_SCROLL );
}

void ssd1306_startScrollDiaRight( uint8_t start, uint8_t stop ){
	ssd1306_cmd( SSD1306_SET_VERTICAL_SCROLL_AREA );
	ssd1306_cmd( 0x00 );
	ssd1306_cmd( SSD1306_HEIGHT );
	ssd1306_cmd( SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL );
	ssd1306_cmd( 0x00 );
	ssd1306_cmd( start );
	ssd1306_cmd( 0x00 );
	ssd1306_cmd( stop );
	ssd1306_cmd( 0x01 );
	ssd1306_cmd( SSD1306_ACTIVATE_SCROLL );
}

void ssd1306_startScrollDiaLeft( uint8_t start, uint8_t stop ){
	ssd1306_cmd( SSD1306_SET_VERTICAL_SCROLL_AREA );
	ssd1306_cmd( 0x00 );
	ssd1306_cmd( SSD1306_HEIGHT );
	ssd1306_cmd( SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL );
	ssd1306_cmd( 0x00 );
	ssd1306_cmd( start );
	ssd1306_cmd( 0x00 );
	ssd1306_cmd( stop );
	ssd1306_cmd( 0x01 );
	ssd1306_cmd( SSD1306_ACTIVATE_SCROLL );
}

void ssd1306_stopScroll( void ){
	ssd1306_cmd( SSD1306_DEACTIVATE_SCROLL );
}

void ssd1306_invertDisplay( uint8_t i ){
	if (i) ssd1306_cmd( SSD1306_INVERTDISPLAY );
	else ssd1306_cmd( SSD1306_NORMALDISPLAY );
}
