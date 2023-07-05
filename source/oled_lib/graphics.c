#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#include "ssd1306.h"


#define SSD1306_PUT_INT_BUF_SIZE 16 /* 15(16) + 1('0' - koniec stringa) */

// Added by Szymon Kajda, used by ssd1306_move()
extern uint8_t ssd1306_buf[ SSD1306_BUF_SIZE ];

void tostring(char str[], uint16_t num){
    uint16_t i, rem, len = 0, n;
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

void tostring_zeroes(char str[], uint16_t num, uint8_t len){
    uint16_t i, rem;
    //n = num;

    for( uint8_t z = 0; z<len; z++ )
    	str[z] = '0';

    for ( i = 0; i < len; i++ ){
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

// Let's stay with drawing bitmaps for now, this algorithm is too complicated and big to speed up the program.
// Too many cases to handle.
/*
void ssd1306_move( int x0, int y0, uint8_t w, uint8_t h, int dest_x, int dest_y, uint8_t mode ){
	// x_it - iterator in X axis, y_it - iterator in Y axis
	for( uint8_t y_it=0; y_it<h; y_it++ ){			// for every pixel
		for( uint8_t x_it = 0; x_it<w; x_it++ ){
			// pick colour
			uint8_t pixel_colour = bit_is_set( ssd1306_buf[ x0+x_it + (y0+y_it/8) * SSD1306_WIDTH ], (y0+y_it) % 8 );	// taken from ssd1306_setPixel()
			// set the same colour on destination pixel
			ssd1306_setPixel( dest_x+x_it, dest_y+y_it, pixel_colour );

			// ok, but what if destination overlaps the starting pos?
//			switch( mode ){
//				//case 0:	// just move, no additional action needed
//				//	break;
//				case 1:		// move and fill the starting position with black
//					ssd1306_setPixel( x0+x_it, y0+y_it, 0 );
//					break;
//				case 2:		// move and fill the starting position with white
//					ssd1306_setPixel( x0+x_it, y0+y_it, 1 );
//					break;
//			}
		}
	}
	// if( mode > 0 ) and then all "fors"
	// when moving up in Y axis
	for( uint8_t y_it = dest_y+h; y_it < y0+h; y_it++ ){	// for every pixel in the gap
			for( uint8_t x_it = x0; x_it < x0+w; x_it++ ){
				switch( mode ){
					//case 0:	// just move, no additional action needed
					//	break;
					case 1:		// move and fill the gap with black
						ssd1306_setPixel( x_it, y_it, 0 );
						break;
					case 2:		// move and fill the gap with white
						ssd1306_setPixel( x_it, y_it, 1 );
						break;
					}
			}
	}
	// when moving down in Y axis
	for( uint8_t y_it = y0; y_it < dest_y; y_it++ ){	// for every pixel in the gap
		for( uint8_t x_it = x0; x_it < x0+w; x_it++ ){
			switch( mode ){
				//case 0:	// just move, no additional action needed
				//	break;
				case 1:		// move and fill the gap with black
					ssd1306_setPixel( x_it, y_it, 0 );
					break;
				case 2:		// move and fill the gap with white
					ssd1306_setPixel( x_it, y_it, 1 );
					break;
			}
		}
	}
	// when moving right in X axis
	for( uint8_t y_it = y0; y_it < y0+h; y_it++ ){	// for every pixel in the gap
		for( uint8_t x_it = x0; x_it < dest_x; x_it++ ){
			switch( mode ){
				//case 0:	// just move, no additional action needed
				//	break;
				case 1:		// move and fill the gap with black
					ssd1306_setPixel( x_it, y_it, 0 );
					break;
				case 2:		// move and fill the gap with white
					ssd1306_setPixel( x_it, y_it, 1 );
					break;
			}
		}
	}
	// when moving left in X axis
	for( uint8_t y_it = y0; y_it < y0+h; y_it++ ){	// for every pixel in the gap
		for( uint8_t x_it = dest_x; x_it < x0; x_it++ ){
			switch( mode ){
				//case 0:	// just move, no additional action needed
				//	break;
				case 1:		// move and fill the gap with black
					ssd1306_setPixel( x_it, y_it, 0 );
					break;
				case 2:		// move and fill the gap with white
					ssd1306_setPixel( x_it, y_it, 1 );
					break;
			}
		}
	}
}
*/

/* if you don't want to draw out of screen you can type "uint8_t" against "int" in some of function parameters */

int cursor_x, cursor_y;

void ssd1306_drawBitmap_P( int x, int y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color, uint8_t bg ){
	int i, j, byteWidth = ( w + 7 ) / 8;

	for( j=0; j<h; j++ ){
		for( i=0; i<w; i++ ){
			if( pgm_read_byte( bitmap + j * byteWidth + i / 8 ) & ( 128 >> (i & 7) ) )
				ssd1306_setPixel( x+i, y+j, color );
			else ssd1306_setPixel( x+i, y+j, bg );
		}
	}
}

void ssd1306_puts( int x, int y, char * str, uint8_t txt_size, uint8_t color, uint8_t bg ){

	cursor_x = x; cursor_y = y;

	// Character counter to avoid writing
	// garbage data in an infinite loop.
	uint8_t char_cnt = 0;

	while( *str || char_cnt <= SSD1306_STR_MAX_CHAR_CNT ){
		ssd1306_drawChar( cursor_x, cursor_y, *str++, color, bg, txt_size );
		cursor_x += txt_size*6; /* 6 - szerokosc znaku */
		char_cnt++;
	}
}

void ssd1306_puts_P( int x, int y, const char * str, uint8_t txt_size, uint8_t color, uint8_t bg ){

	cursor_x = x; cursor_y = y;

	register char c;
	while( *str ){
		c = pgm_read_byte( str++ );
		ssd1306_drawChar( cursor_x, cursor_y, c, color, bg, txt_size );
		cursor_x += txt_size*6; /* 6 - szerokosc znaku */
	}
}

void ssd1306_put_int( int x, int y, int data, uint8_t txt_size, uint8_t color, uint8_t bg ){
	if( data == 0 )
		ssd1306_puts( x, y, "0\0", txt_size, color, bg );
	else{
		char buf[ SSD1306_PUT_INT_BUF_SIZE ];
		tostring( buf, data );
	//	int_to_string( buf, data, number_of_numbers );
	//	ssd1306_puts( x, y, itoa(data,buf,10), txt_size, color, bg );
		ssd1306_puts( x, y, buf, txt_size, color, bg );
	}
}

void ssd1306_put_int_zeroes( int x, int y, int data, uint8_t number_of_zeroes, uint8_t txt_size, uint8_t color, uint8_t bg ){
	char buf[ SSD1306_PUT_INT_BUF_SIZE ];
	tostring_zeroes( buf, data, number_of_zeroes );
//	int_to_string( buf, data, number_of_numbers );
//	ssd1306_puts( x, y, itoa(data,buf,10), txt_size, color, bg );
	ssd1306_puts( x, y, buf, txt_size, color, bg );
}

// Added by Szymon Kajda
// Two decimal places
/*
void ssd1306_put_float( int x, int y, uint16_t num, uint8_t size, uint8_t color, uint8_t bg ){
	uint8_t ex = num/100;		// Max three exponential(?) places
	uint8_t fr = num-(ex*100);	// Two decimal places

	uint8_t ex_len=0;
	uint8_t tmp_ex=ex;
	while( tmp_ex > 0 ){
		tmp_ex/=10;
		ex_len++;
	}


	ssd1306_put_int( x, y, ex, size, color, bg );
	ssd1306_drawChar( x+ex_len*(5*size+1), y, '.', color, bg, size );
	ssd1306_put_int( x+(ex_len+1)*(5*size+1), y, fr, size, color, bg );
}
*/

void ssd1306_drawChar( int x, int y, char c, uint8_t color, uint8_t bg, uint8_t size ){

	if( (x >= SSD1306_WIDTH) || (y >= SSD1306_HEIGHT) || ( (x + 6 * size - 1) < 0 ) || ( (y + 8 * size - 1) < 0 ) )
		return;

	uint8_t line;

	for( uint8_t i=0; i<6; i++ ){

		if( i == 5 )
			line = 0x0;
		else
			line = pgm_read_byte( font + (c*5) + i );

		for( int8_t j=0; j<8; j++ ){
			if( line & 0x1 ){
				if ( size == 1 )
					ssd1306_setPixel( x+i, y+j, color );
				else
					ssd1306_drawFillRect( x+(i*size), y+(j*size), size, size, color );
			}
			else if ( bg != color ){
				if( size == 1 )
					ssd1306_setPixel( x+i, y+j, bg );
				else
					ssd1306_drawFillRect( x+i*size, y+j*size, size, size, bg );
			}
			line >>= 1;
		}
	}
}

void ssd1306_drawFillRect( int x, int y, int w, int h, uint8_t color ){
	for( int16_t i=x; i<x+w; i++ ){
		ssd1306_drawFastVLine( i, y, h, color );
	}
}

void ssd1306_drawFastVLine( int x, int y, uint8_t h, uint8_t color ){
	for(int i=y;i<y+h;i++)
		ssd1306_setPixel(x,i,color);
	//ssd1306_drawLine( x, y, x, y+h-1, color );
}

void ssd1306_drawFastHLine( int x, int y, uint8_t w, uint8_t color ){
	ssd1306_drawLine( x, y, x+w-1, y, color );
}

/* Bresenham's algorithm */
void ssd1306_drawLine( int x0, int y0, int x1, int y1, uint8_t color ){

	int steep = abs( y1 - y0 ) > abs( x1 - x0 );

	if(steep){
		swap( x0, y0 );
		swap( x1, y1 );
	}

	if( x0 > x1 ){
		swap( x0, x1 );
		swap( y0, y1 );
	}

	int dx, dy;
	dx = x1 - x0;
	dy = abs( y1 - y0 );

	int err = dx / 2;
	int ystep;

	if( y0 < y1 )
		ystep = 1;
	else
		ystep = -1;

	for( ; x0 <= x1; x0++ ){
		if(steep)
			ssd1306_setPixel( y0, x0, color );
		else
			ssd1306_setPixel( x0, y0, color );

		err -= dy;

		if( err < 0 ){
			y0 += ystep;
			err += dx;
		}
	}
}

void ssd1306_drawCircle( int x, int y, uint8_t r, uint8_t bw ){
	int f = 1 - r;
	int ddF_x = 1;
	int ddF_y = -2 * r;
	int xa = 0;
	int ya = r;

	ssd1306_setPixel( x, y + r, bw );
	ssd1306_setPixel( x, y - r, bw );
	ssd1306_setPixel( x + r, y, bw );
	ssd1306_setPixel( x - r, y, bw );

	while ( xa < ya ){
		if ( f >= 0 ){
			ya--;
			ddF_y += 2;
			f += ddF_y;
		}
		xa++;
		ddF_x += 2;
		f += ddF_x;

		ssd1306_setPixel( x + xa, y + ya, bw );
		ssd1306_setPixel( x - xa, y + ya, bw );
		ssd1306_setPixel( x + xa, y - ya, bw );
		ssd1306_setPixel( x - xa, y - ya, bw );
		ssd1306_setPixel( x + ya, y + xa, bw );
		ssd1306_setPixel( x - ya, y + xa, bw );
		ssd1306_setPixel( x + ya, y - xa, bw );
		ssd1306_setPixel( x - ya, y - xa, bw );
	}

}

void ssd1306_drawFillCircle( int x, int y, uint8_t r, uint8_t color ){
	ssd1306_drawFastVLine( x, y-r, 2*r+1, color );
	ssd1306_fillCircleHelper( x, y, r, 3, 0, color );
}

void ssd1306_fillCircleHelper( int x0, int y0, int r, uint8_t cornername, int delta, uint8_t color ){
	int f = 1 - r;
	int ddF_x = 1;
	int ddF_y = -2 * r;
	int x = 0;
	int y = r;

	while( x < y ){
		if( f >= 0 ){
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if( cornername & 0x1 ){
			ssd1306_drawFastVLine( x0+x, y0+y, 2*y+1+delta, color );
			ssd1306_drawFastVLine( x0+x, y0-y, 2*x+1+delta, color );
		}
		if( cornername & 0x2 ){
			ssd1306_drawFastVLine( x0-x, y0-y, 2*y+1+delta, color );
			ssd1306_drawFastVLine( x0-y, y0-x, 2*x+1+delta, color );
		}
	}
}

void ssd1306_drawRect( int x, int y, uint8_t w, uint8_t h, uint8_t color ){
	ssd1306_drawFastHLine( x, y, w, color );
	ssd1306_drawFastHLine( x, y+h-1, w, color );
	ssd1306_drawFastVLine( x, y, h, color );
	ssd1306_drawFastVLine( x+w-1, y, h, color );
}

void ssd1306_drawRoundRect( int x, int y, uint8_t w, uint8_t h, uint8_t r, uint8_t color ){
	ssd1306_drawFastHLine( x+r, y, w-2*r, color );
	ssd1306_drawFastHLine( x+r, y+h-1, w-2*r, color );
	ssd1306_drawFastVLine( x, y+r, h-2*r, color );
	ssd1306_drawFastVLine( x+w-1, y+r, h-2*r, color );

	ssd1306_drawCircleHelper( x+r, y+r, r, 1, color );
	ssd1306_drawCircleHelper( x+w-r-1, y+r, r, 2, color );
	ssd1306_drawCircleHelper( x+w-r-1, y+r, y+h-r-1, 4, color );
	ssd1306_drawCircleHelper( x+r, y+r, y+h-1-1, 8, color );
}

void ssd1306_drawFillRoundRect( int x, int y, uint8_t w, uint8_t h, uint8_t r, uint8_t color ){
	ssd1306_drawFillRect( x+r, y, w-2*r, h, color );

	ssd1306_fillCircleHelper( x+w-r-1, y+r, r, 1, h-2*r-1, color );
	ssd1306_fillCircleHelper( x+r, y+r, r, 2, h-2*r-1, color );
}

void ssd1306_drawCircleHelper( int x0, int y0, uint8_t r, uint8_t cornername, uint8_t color ){
	int f = 1 - r;
	int ddF_x = 1;
	int ddF_y = -2 * r;
	int x = 0;
	int y = r;

	while( x < y ){
		if( f >= 0 ){
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if( cornername & 0x4 ){
			ssd1306_setPixel( x0 + x, y0 + y, color );
			ssd1306_setPixel( x0 + y, y0 + x, color );
		}
		if( cornername & 0x2 ){
			ssd1306_setPixel( x0 + x, y0 - y, color );
			ssd1306_setPixel( x0 + y, y0 - x, color );
		}
		if( cornername & 0x8 ){
			ssd1306_setPixel( x0 - y, y0 + x, color );
			ssd1306_setPixel( x0 - x, y0 + y, color );
		}
		if( cornername & 0x1 ){
			ssd1306_setPixel( x0 - y, y0 - x, color );
			ssd1306_setPixel( x0 - x, y0 - y, color );
		}
	}
}

void ssd1306_drawTriangle( int x0, int y0, int x1, int y1, int x2, int y2, uint8_t color ){
	ssd1306_drawLine( x0, y0, x1, y1, color );
	ssd1306_drawLine( x1, y1, x2, y2, color );
	ssd1306_drawLine( x2, y2, x0, y0, color );
}

void ssd1306_drawFillTriangle( int x0, int y0, int x1, int y1, int x2, int y2, uint8_t color ){
	int16_t a, b, y, last;

	    // Sort coordinates by Y order (y2 >= y1 >= y0)
	    if (y0 > y1) {
	        swap(y0, y1); swap(x0, x1);
	    }
	    if (y1 > y2) {
	        swap(y2, y1); swap(x2, x1);
	    }
	    if (y0 > y1) {
	        swap(y0, y1); swap(x0, x1);
	    }

	    if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
	        a = b = x0;
	        if(x1 < a)      a = x1;
	        else if(x1 > b) b = x1;
	        if(x2 < a)      a = x2;
	        else if(x2 > b) b = x2;
	        ssd1306_drawFastHLine(a, y0, b-a+1, color);
	        return;
	    }

	    int16_t
	    dx01 = x1 - x0,
	    dy01 = y1 - y0,
	    dx02 = x2 - x0,
	    dy02 = y2 - y0,
	    dx12 = x2 - x1,
	    dy12 = y2 - y1;
	    int32_t
	    sa   = 0,
	    sb   = 0;

	    // For upper part of triangle, find scanline crossings for segments
	    // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	    // is included here (and second loop will be skipped, avoiding a /0
	    // error there), otherwise scanline y1 is skipped here and handled
	    // in the second loop...which also avoids a /0 error here if y0=y1
	    // (flat-topped triangle).
	    if(y1 == y2) last = y1;   // Include y1 scanline
	    else         last = y1-1; // Skip it

	    for(y=y0; y<=last; y++) {
	        a   = x0 + sa / dy01;
	        b   = x0 + sb / dy02;
	        sa += dx01;
	        sb += dx02;
	        /* longhand:
	        a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
	        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
	        */
	        if(a > b) swap(a,b);
	        ssd1306_drawFastHLine(a, y, b-a+1, color);
	    }

	    // For lower part of triangle, find scanline crossings for segments
	    // 0-2 and 1-2.  This loop is skipped if y1=y2.
	    sa = dx12 * (y - y1);
	    sb = dx02 * (y - y0);
	    for(; y<=y2; y++) {
	        a   = x1 + sa / dy12;
	        b   = x0 + sb / dy02;
	        sa += dx12;
	        sb += dx02;
	        /* longhand:
	        a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
	        b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
	        */
	        if(a > b) swap(a,b);
	        ssd1306_drawFastHLine(a, y, b-a+1, color);
	    }
}
