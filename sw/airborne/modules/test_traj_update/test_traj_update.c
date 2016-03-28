/*
 * Copyright (C) M. Kisantal
 *
 * This file is part of paparazzi
 *
 */
/**
 * @file "modules/test_traj_update/test_traj_update.c"
 * @author M. Kisantal
 * testing periodic functions to update trajectory. Periodically checking position, then making a decision and a waypoint based upon that.
 */

#include "modules/test_traj_update/test_traj_update.h"
#include "modules/periodic_switch/periodic_switch.h"
#include "state.h"
#include "firmwares/rotorcraft/navigation.h"

struct PotentialFieldStruct {
        double lon;
        double lat;
        float dX;
        float dY;      
};

#define CELLNUMX 56 // set the PotentialField grid resolution here
#define CELLNUMY 56
static struct PotentialFieldStruct PotentialField[CELLNUMX][CELLNUMY]; 

static int i,j;

static double LongIncrement;
static double LatIncrement;

float deltaX;
float deltaY;



void init_test_traj_update(void){
        
        

        float SP[5][3]; // array size could be smaller,but now it fixes messed up indexing
        SP[1][1] = 51.990657;    SP[1][2] = 4.376748;
        SP[2][1] = 51.990601;    SP[2][2] = 4.376748;
        SP[3][1] = 51.990601;    SP[3][2] = 4.376845;
        SP[4][1] = 51.990657;    SP[4][2] = 4.376845;

        

 

        
         LongIncrement = (SP[3][2] - SP[2][2]) /CELLNUMX;
         LatIncrement  = (SP[4][1] - SP[3][1]) /CELLNUMY;



// _________________DEFINING PotentialField SECTORS______________________________
        for (j=0; j<CELLNUMY; j++){              // sectors are defined by left bottom corner coordinates
                for (i=0; i<CELLNUMX; i++){
                        PotentialField[i][j].lon  = SP[2][2] + i * LongIncrement;

                        PotentialField[i][j].lat  = SP[2][1] + j * LatIncrement;

                        PotentialField[i][j].dX = 0; 
                        PotentialField[i][j].dY = 0;
                        //PotentialField[i][j].dX = cosf(i*3.14/180); 
                        //PotentialField[i][j].dY = sinf(i*3.14/180);
                }     
        };
// ----------------------------------------------------------------------
printf("------- Initialization done.---------\n");
return;}









void periodic_test_traj_update() {
        

if (trajectory_flag == 0) { return;} // to activate call from flightplan: logger_switch(0,1)
printf("periodic function running...\n");


// ________________PotentialField UPDATE_____________________________


// -----------------------------------------------------------




        int LON_i, LAT_i, FLAG_long, FLAG_lat;
        float LON, LAT; 
//stateCalcPositionLla_i(); 
        // getting current LAT/LON      
        LAT_i = stateGetPositionLla_i()->lat;
        LON_i = stateGetPositionLla_i()->lon;

        LON =  LON_i;
        LAT =  LAT_i;


        LON =  LON / 10000000;
        LAT =  LAT / 10000000;



        //LOCALIZATION LOOPS
        for(i=0; i<CELLNUMX; i++){               
                if (LON < PotentialField[i][1].lon){
                        if (i !=0){FLAG_long = i-1;}
                        else {FLAG_long = 0;}   //zero index if coord is smaller than any sector
                break;                        
                } else {if (i == CELLNUMX - 1){FLAG_long = i;};} //max index if coord is bigger than any sector
        }

        for(j=0; j<CELLNUMY; j++){
                if (LAT < PotentialField[1][j].lat){
                        if(j != 0){FLAG_lat = j-1;}
                        else {FLAG_lat = 0;}
                break;
                } else { if (j == CELLNUMY - 1){FLAG_lat = j;};}
        }

        //update Gradient
        deltaX = PotentialField[FLAG_long][FLAG_lat].dX;
        deltaY = PotentialField[FLAG_long][FLAG_lat].dY;
printf("deltaX = %f\n",deltaX);
printf("deltaY = %f\n ------- \n",deltaY);        

};




void stop_test_traj_update(void) {return;}

int setPotentialField(int x) {
printf("Potential field is set. x= %d\n", x);
      for (j=0; j<CELLNUMY; j++){              // sectors are defined by left bottom corner coordinates
                for (i=0; i<CELLNUMX; i++){
                        PotentialField[i][j].dX = cosf(j*10*3.14/180+i*10*3.14/180); // now it's just a arbitrary initialization
                        PotentialField[i][j].dY = sinf(j*10*3.14/180+i*10*3.14/180);
                }     
        };

return FALSE;}


uint8_t moveWaypointForwards(uint8_t waypoint, float distanceMeters){

printf("HALLO.\n");


        struct EnuCoor_i new_coor;
	struct EnuCoor_i *pos = stateGetPositionEnu_i(); // Get your current position
          
        

        float theta = atan2f(deltaY, deltaX);

        float sin_heading = sinf(theta);
	float cos_heading = cosf(theta);

	/*  // Calculate the sine and cosine of the heading the drone is keeping
	  float sin_heading = sinf(ANGLE_FLOAT_OF_BFP(nav_heading));
	  float cos_heading = cosf(ANGLE_FLOAT_OF_BFP(nav_heading));
        */
          // calculate heading based on dX and dY

	  // Now determine where to place the waypoint you want to go to
	  new_coor.x = pos->x + POS_BFP_OF_REAL(sin_heading * (distanceMeters));
	  new_coor.y = pos->y + POS_BFP_OF_REAL(cos_heading * (distanceMeters));
	  new_coor.z = pos->z; // Keep the height the same

	  // Set the waypoint to the calculated position
	  waypoint_set_xy_i(waypoint, new_coor.x, new_coor.y);
          nav_set_heading_towards(new_coor.x, new_coor.y);

	  return FALSE;
}



