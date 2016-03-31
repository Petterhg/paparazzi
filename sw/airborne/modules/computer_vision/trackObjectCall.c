/*
 * Copyright (C) Tigran Mkhoyan
 *
 * This file is part of paparazzi
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
/**
 * @file "modules/computer_vision/cv_opencvdemo.c"
 * @author Tigran Mkhoyan
 * opencv
 */

#include "modules/computer_vision/cv.h"
#include "setFilterHSV.h"
#include "trackObjectCall.h"
#include "trackObject.h"
#include "subsystems/datalink/telemetry.h"
#include <stdio.h>

//from tarckobject.h
//Initialising the struct values. No need to declare egain in trackobject.cpp they will be visible when header is included 
parameters CollisionParam = {
  .targetPosx =0,
  .targetPosy =0,
  .Area = 0,
  .CollisionDetect = false, //bool
  .objectDetect = false, //bool
  .nObjects = 0,
  //.wheretoGo =(char *){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0}, 
  .wheretoGo ={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0}, 
  //.trackState = (char *){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  .trackState = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},
  //.trackState = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
  .actionFlag = 0
};


// Function
bool_t opencv_func(struct image_t* img);
bool_t opencv_func(struct image_t* img)
{

  if (img->type == IMAGE_YUV422)
  {
    /*start of trackobject.cpp */
    // Call OpenCV (C++ from paparazzi C function)

    tarckobjectMain((char*) img->buf, img->w, img->h);

    /*end of trackObject.cpp  */
    // broadcast parameters populated in trackobject.cpp wheretogo needs to be converted with strlen his returns an adres so no need for &   
    //for char type two arguments are needed array lenght and the address to value containing caracters
    //so arg 1 is strlen(char, dest) = lenght second is &char

    
    DOWNLINK_SEND_TRACKOBJECT(DefaultChannel, DefaultDevice, &CollisionParam.targetPosx,
      &CollisionParam.targetPosy, &CollisionParam.Area, &CollisionParam.CollisionDetect,
      &CollisionParam.objectDetect, &CollisionParam.nObjects, strlen(CollisionParam.wheretoGo),
      &CollisionParam.wheretoGo, strlen(CollisionParam.trackState), &CollisionParam.trackState,
      &CollisionParam.actionFlag);
 // DOWNLINK_SEND_AVOID_ACTION(DefaultChannel, DefaultDevice, strlen(wheretoGo), &wheretoGo);
 // printf("Downlink2 %d \n",32);
  //  printf(" Area EXtern from C: %lf \n", CollisionParam.Area);
  //TODO decide action and send avoid message 
  }

  return FALSE;
}

void trackObject_init(void)
{

/*called only once before trackobject  */
 // printf("Start Call line %d \n",32);


  cv_add(opencv_func);

//end call
//printf("End Call line %d \n",75);

}
