/*
 * Copyright (C) Tigran Mkhoyan
 *
 * This file is part of paparazzi
 *
 */
/**
 * @file "modules/orange_avoider/orange_avoider.c"
 * @author Roland Meertens
 * Example on how to use the colours detected to avoid orange pole in the cyberzoo
 */

#include "modules/trackObject_avoid/trackObject_avoid.h"
#include "modules/computer_vision/trackObject.h"
 #include "modules/computer_vision/setFilterHSV.h"
#include "firmwares/rotorcraft/navigation.h"
#include "state.h"
#include <time.h>
#include <stdlib.h>

uint8_t safeToGoForwards=FALSE;
int32_t incrementForAvoidance;

void trackObject_avoid_set() {

//set HSV filter to certain values:
/*uint8_t H_MIN = 0;
uint8_t H_MAX = 256;
uint8_t S_MIN = 0;
uint8_t S_MAX = 256;
uint8_t V_MIN = 0;
uint8_t V_MAX = 256;
//set tracking options
uint8_t trackObject=true;
uint16_t tresholdArea= 10000;   //limit [5000-40000] setFilterhsv.xml
int16_t offsetPosx=20;  //limit [0-90] pix
int16_t offsetPosy=20; */

}
//parameters & trackdata)
void trackObject_avoid_periodic() {

	/*actions are decided in track object function track filtered objects
	we need to collect this info and perfom action
	*/
	//check action if collision detected:
	if (trackdata.CollisionDetect == TRUE){
		//check what to do
		chooseIncrementAvoidance();
		safeToGoForwards = FALSE;
		}
		else 
		{
			safeToGoForwards= TRUE;
			incrementForAvoidance = 0;
		}

	printf("Checking if this funciton is called: these must be equal %d == %d \n", trackdata.CollisionDetect , safeToGoForwards);
}


/**
 * Increases the NAV heading. Assumes heading is an INT32_ANGLE. It is bound in this function.
 */
uint8_t increase_nav_heading(int32_t *heading, int32_t increment)
{
  *heading = *heading + increment;
  // Check if your turn made it go out of bounds...
  INT32_ANGLE_NORMALIZE(*heading); // HEADING HAS INT32_ANGLE_FRAC....
  return FALSE;
}
uint8_t moveWaypointForwards(uint8_t waypoint, float distanceMeters){
	  struct EnuCoor_i new_coor;
	  struct EnuCoor_i *pos = stateGetPositionEnu_i(); // Get your current position

	  // Calculate the sine and cosine of the heading the drone is keeping
	  float sin_heading = sinf(ANGLE_FLOAT_OF_BFP(nav_heading));
	  float cos_heading = cosf(ANGLE_FLOAT_OF_BFP(nav_heading));

	  // Now determine where to place the waypoint you want to go to
	  new_coor.x = pos->x + POS_BFP_OF_REAL(sin_heading * (distanceMeters));
	  new_coor.y = pos->y + POS_BFP_OF_REAL(cos_heading * (distanceMeters));
	  new_coor.z = pos->z; // Keep the height the same

	  // Set the waypoint to the calculated position
	  waypoint_set_xy_i(waypoint, new_coor.x, new_coor.y);

	  return FALSE;
}

uint8_t chooseIncrementAvoidance(){

		if (trackdata.action == 1){
			//go to right
			incrementForAvoidance=350;
		}
		else if(trackdata.action ==2)  {
			//go to left
			incrementForAvoidance=-350;
		}

		else incrementForAvoidance=0;

	return FALSE;
}

