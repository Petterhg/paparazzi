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
 * Plugin for trackObject HSV Filter settings 
 Tigran Mkhoyan
 */

#ifndef HSV_FSETTINGS_PLUGIN
#define HSV_FSETTINGS_PLUGIN

#include <stdint.h>

// Module functions
extern void setFilterHSV_init(void);

/* ----------SETTINGS SETFILTERhsv.xml----------*/
//filter settings
extern uint8_t H_MIN;
extern uint8_t H_MAX;

extern uint8_t S_MIN;
extern uint8_t S_MAX;

extern uint8_t V_MIN;
extern uint8_t V_MAX;

//tracking options
extern uint8_t trackObject; //on off
extern uint16_t tresholdArea; //area to trigger warning
extern int16_t offsetPosx; //x,y offset from center to trigger waring
extern int16_t offsetPosy;   
/* ----------SETTINGS SETFILTERhsv.xml----------*/

/* ------TELEMETRY MESSAGES.xml --------*/
extern uint8_t FilterIsSet;
/* ------TELEMETRY MESSAGES.xml --------*/



#endif /* HSV_FSETTINGS_PLUGIN */


