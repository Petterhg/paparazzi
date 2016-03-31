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
 * @file "modules/computer_vision/cv_opencvdemo.h"
 * @author C. De Wagter
 * opencv
 */

#ifndef TRACKOBJECT_H
#define TRACKOBJECT_H
#include <inttypes.h>
#include <stdint.h>




/*
//typedefinition
typedef struct param {
int targetPosx;
int targetPosy;
double Area;
bool CollisionDetect;
bool objectDetect;
int nObjects;
char wheretoGo[10];
} parameters;




//declaring the struct as extern. Definition is done in ONE sourfile
extern parameters *CollisionParamPointer;

*/

/*------------------- GLOBALS--------------------  */
typedef struct param{
uint16_t targetPosx; //need values highet than 255 -- 272 max 
uint16_t targetPosy;
uint32_t Area; //max val 60k approx
uint8_t CollisionDetect; //bool
uint8_t objectDetect; //bool
uint8_t nObjects;
char wheretoGo[15];
char trackState[30];
//typedef enum actionFlag;
uint8_t actionFlag; //{0,1,2,3} = {noaction, right, left , stop}
} parameters;

extern parameters CollisionParam;
extern void trackFilteredObject(parameters CollisionParam);
/*------------------- GLOBALS--------------------  */

/*--------------CONSTANTS*----------------*/
const int whalf =136;
const int yhalf =136;
/*--------------CONSTANTS*----------------*/

#ifdef __cplusplus
extern "C" {
#endif

 //int trackObject(char* img, int width, int height);
int tarckobjectMain(char* img, int width, int height);

#ifdef __cplusplus
}
#endif

#endif
