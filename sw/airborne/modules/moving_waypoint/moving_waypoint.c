/*
 * Copyright (C) M. Kisantal
 *
 * This file is part of paparazzi
 *
 */
/**
 * @file "modules/moving_waypoint/moving_waypoint.c"
 * @author M. Kisantal
 * A first attempt to create a module which is able to do stuff with waypoints. Wish good luck for me!
 */

#include "modules/moving_waypoint/moving_waypoint.h"
#include "firmwares/rotorcraft/navigation.h"
#include "state.h"
#include <time.h>
#include <stdlib.h>

int cubeCounter;

void initMyWaypoint()
{
cubeCounter=0;
}
// void fcn_moving_waypoint {}
extern uint8_t fcn_moving_waypoint(uint8_t waypoint){
	struct EnuCoor_i new_coor;
	struct EnuCoor_i *pos = stateGetPositionEnu_i(); // Get your current position

	/*// Calculate the sine and cosine of the heading the drone is keeping
	float sin_heading = sinf(ANGLE_FLOAT_OF_BFP(nav_heading));
	float cos_heading = cosf(ANGLE_FLOAT_OF_BFP(nav_heading)); */

	// Now determine where to place the waypoint you want to go to
	
	cubeCounter++;
	switch (cubeCounter){
		case 1:
			new_coor.x = pos->x + POS_BFP_OF_REAL(1);
			new_coor.y = pos->y + POS_BFP_OF_REAL(0);
			new_coor.z = pos->z + POS_BFP_OF_REAL(0);
			break;
		case 2:
			new_coor.x = pos->x + POS_BFP_OF_REAL(-1);
			new_coor.y = pos->y + POS_BFP_OF_REAL(-1);
			new_coor.z = pos->z + POS_BFP_OF_REAL(1);
			break;
		case 3:
			new_coor.x = pos->x + POS_BFP_OF_REAL(0);
			new_coor.y = pos->y + POS_BFP_OF_REAL(1);
			new_coor.z = pos->z + POS_BFP_OF_REAL(0);
			break;
		case 4:
			new_coor.x = pos->x + POS_BFP_OF_REAL(1);
			new_coor.y = pos->y + POS_BFP_OF_REAL(-1);
			new_coor.z = pos->z + POS_BFP_OF_REAL(0);
			break;
		case 5:
			new_coor.x = pos->x + POS_BFP_OF_REAL(0);
			new_coor.y = pos->y + POS_BFP_OF_REAL(0);
			new_coor.z = pos->z + POS_BFP_OF_REAL(-1);
			break;
		case 6:
			new_coor.x = pos->x + POS_BFP_OF_REAL(0);
			new_coor.y = pos->y + POS_BFP_OF_REAL(1);
			new_coor.z = pos->z + POS_BFP_OF_REAL(1);
			break;
		case 7:
			new_coor.x = pos->x + POS_BFP_OF_REAL(-1);
			new_coor.y = pos->y + POS_BFP_OF_REAL(-1);
			new_coor.z = pos->z + POS_BFP_OF_REAL(-1);
			break;
		case 8:
			new_coor.x = pos->x + POS_BFP_OF_REAL(0);
			new_coor.y = pos->y + POS_BFP_OF_REAL(1);
			new_coor.z = pos->z + POS_BFP_OF_REAL(0);
			break;

	}

	// Set the waypoint to the calculated position
	waypoint_set_xy_i(waypoint, new_coor.x, new_coor.y);



	return FALSE;
}


