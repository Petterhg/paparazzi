/*
 * Copyright (C) 2015
 *
 * This file is part of Paparazzi.
 *
 * Paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * Paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * @file modules/computer_vision/colorfilter.c
 */

// Own header
#include "modules/computer_vision/cv.h"
#include "modules/computer_vision/setFilterHSV.h"

#include "subsystems/datalink/telemetry.h"
#include <stdio.h>
#include <string.h>
#include <std.h>


/* ----------SETTINGS SETFILTER.xml----------*/
//initial MIN and MAX HSV filter values
uint8_t H_MIN = 0;
uint8_t H_MAX = 255;
uint8_t S_MIN = 0;
uint8_t S_MAX = 255;
uint8_t V_MIN = 0;
uint8_t V_MAX = 255;

//initializing tracking options {area,tracking on/off,}
uint8_t trackObject=true;
uint16_t tresholdArea= 10000;   //limit [5000-40000] setFilterhsv.xml
int16_t offsetPosx=20;  //limit [0-90] pix
int16_t offsetPosy=20; 

/* -area of image is 272*272 pix however max 
   -area to be processed is 1/5*totarea = 50000 so we need int16
   -x,y offset from center to trigger waring
   -must be signed! defined as min max offset from center
 */
/*------------SETTINGS SETFILTER.xml-------------*/

/* ------TELEMETRY MESSAGES.xml --------*/

uint8_t FilterIsSet = false; // boolean set value sent by message

/* ------TELEMETRY MESSAGES.xml --------*/

//Function to update hsv values 

//bool_t setFilterHSV_func(void);
//bool_t setFilterHSV_func(void){

bool_t setFilterHSV_func(struct image_t* img);
bool_t setFilterHSV_func(struct image_t* img){

// TODO:
//send boolean back
//DOWNLINK_SEND_HSV_FILTER_CHANGE(DefaultChannel, DefaultDevice, &FilterIsSet);

//printf("Global variable: %d\n", H_MIN);	
 return FALSE;

}

//Function 
void setFilterHSV_init(void)
{
  cv_add(setFilterHSV_func);
}

