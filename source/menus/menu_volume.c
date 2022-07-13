/*
 * menu_volume.c
 *
 *  Created on: 13 Jul 2022
 *      Author: Szymon Kajda
 */

#include "../menus.h"
#include "menu_volume.h"

void handleMenuVolume(){
	if( buttonState() == PRESS ){
		gotoMenu( menu_sound_settings );
	}
}
