/*
 * Copyright (C) 2014 Freek van Tienen <freek.v.tienen@gmail.com>
 *
 * This file is part of paparazzi.
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
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

/** @file modules/loggers/file_logger.c
 *  @brief File logger for Linux based autopilots
 */

#include "file_logger.h"

#include <stdio.h>
#include "std.h"

#include "subsystems/imu.h"
#include "firmwares/rotorcraft/stabilization.h"
#include "state.h"
#include "modules/sonar/agl_dist.h"
#include "modules/sonar/sonar_bebop.h"
#include "subsystems/gps/gps_datalink.h"
#include "subsystems/gps.h"
#include "modules/periodic_switch/periodic_switch.h"

/** Set the default File logger path to the USB drive */
#ifndef FILE_LOGGER_PATH
#define FILE_LOGGER_PATH /data/ftp/internal_000 /** /data/video/usb */
#endif

/** The file pointer */
static FILE *file_logger = NULL;

/** Start the file logger and open a new file */
extern int file_logger_start(void) /* void ---> extern int*/
{
  uint32_t counter = 0;
  char filename[512];

  // Check for available files
  sprintf(filename, "%s/%05d.csv", STRINGIFY(FILE_LOGGER_PATH), counter);
  while ((file_logger = fopen(filename, "r"))) {
    fclose(file_logger);

    counter++;
    sprintf(filename, "%s/%05d.csv", STRINGIFY(FILE_LOGGER_PATH), counter);
  }
  sprintf(filename, "%s/map.csv", STRINGIFY(FILE_LOGGER_PATH)); // working???
  file_logger = fopen(filename, "w");

  if (file_logger != NULL) {
    fprintf(
      file_logger,
	  /**"counter, gyro_unscaled_p,gyro_unscaled_q,gyro_unscaled_r,accel_unscaled_x,accel_unscaled_y,accel_unscaled_z,mag_unscaled_x,mag_unscaled_y,mag_unscaled_z,COMMAND_THRUST,COMMAND_ROLL,COMMAND_PITCH,COMMAND_YAW,qi,qx,qy,qz\n"*/
      "counter, sonar shit, sonar distance, gps.ecef_pos.x, gps.ecef_pos.y"
    );
  }
return 0; /* !!! */
}

/** Stop the logger an nicely close the file */
extern int file_logger_stop(void) /* void ---> extern int*/
{
  if (file_logger != NULL) {
    fclose(file_logger);
    file_logger = NULL;
  }
return 0; /* !!! */
}

/** Log the values to a csv file */
extern int file_logger_periodic(void) /* void ---> extern int*/
{
  if (file_logger == NULL) {
    return;
  }
  if (logger_flag == NULL) {
    return;
  }
  static uint32_t counter;
  struct Int32Quat *quat = stateGetNedToBodyQuat_i();

  fprintf(file_logger, "%d, %d, %d, %d, %d \n", /**  ,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d */
		  counter, sonar_bebop.distance, gps.ecef_pos.x, gps.ecef_pos.y

		  /**imu.gyro_unscaled.p,

		  imu.gyro_unscaled.q,
		  imu.gyro_unscaled.r,
		  imu.accel_unscaled.x,
		  imu.accel_unscaled.y,
		  imu.accel_unscaled.z,
		  imu.mag_unscaled.x,
		  imu.mag_unscaled.y,
		  imu.mag_unscaled.z,
		  stabilization_cmd[COMMAND_THRUST],
		  stabilization_cmd[COMMAND_ROLL],
		            stabilization_cmd[COMMAND_PITCH],
		            stabilization_cmd[COMMAND_YAW],
		            quat->qi,
		            quat->qx,
		            quat->qy,
		            quat->qz
		*/



         );
  counter++;
return 0; /* !!! */
}
