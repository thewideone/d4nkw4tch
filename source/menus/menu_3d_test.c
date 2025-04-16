/*
 * menu_3d_test.c
 *
 *  Created on: 16 Apr 2025
 *      Author: Szymon Kajda
 */


#include "menu_3d_test.h"

#include "../menus.h"

#include "../3d/scenes/scene1.h"
#include "../mcu_graphic_engine_3d/Inc/lib3d_core.h"

void menu_3d_test_init_3d(void) {
	scene1_init();

	l3d_makeProjectionMatrix(&scene1.mat_proj, &scene1.cameras[0]);
	l3d_setupObjects(&scene1, &scene1.mat_proj, &scene1.mat_view);

	l3d_vec4_t n = l3d_getVec4FromFloat(0.0f, 0.0f, 1.0f, 1.0f);
	l3d_rotateOrigin(&scene1, L3D_OBJ_TYPE_OBJ3D, 0, &n, l3d_degToRad(l3d_floatToRational(15.0f)));
}

void handleMenu3dTest(void) {
	switch( button ){
		case PRESS:
			gotoMenu( menu_apps );
			break;
	}

	l3d_flp_t f_elapsed_time = 0.1f;

	l3d_computeViewMatrix(scene1.active_camera, &(scene1.mat_view), f_elapsed_time );

	l3d_rotateGlobalZ(&scene1, L3D_OBJ_TYPE_OBJ3D, &scene1.objects[SCENE1_OBJ_CUBE_TRI_I0_ID], l3d_degToRad(l3d_floatToRational(0.5f)));

	l3d_transformObjectIntoViewSpace(&scene1, L3D_OBJ_TYPE_OBJ3D, SCENE1_OBJ_CUBE_TRI_I0_ID);

	l3d_drawWireframe(&scene1, SCENE1_OBJ_CUBE_TRI_I0_ID);
}
