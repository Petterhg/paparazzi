/*
 * Copyright (C) M. Kisantal
 *
 * This file is part of paparazzi
 *
 */
/**
 * @file "modules/moving_waypoint/moving_waypoint.h"
 * @author M. Kisantal
 * A first attempt to create a module which is able to do stuff with waypoints. Wish good luck for me!
 */

#ifndef MOVING_WAYPOINT_H
#define MOVING_WAYPOINT_H
#include <inttypes.h>

extern int cubeCounter;
// extern void fcn_moving_waypoint;
extern uint8_t fcn_moving_waypoint(uint8_t waypoint);

extern void initMyWaypoint(void);

#endif

