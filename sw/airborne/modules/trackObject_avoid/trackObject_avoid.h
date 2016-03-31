/*
 * Copyright (C) Tigran Mkhoyan
 *
 * This file is part of paparazzi
 *
 */
/**

 */

#ifndef ORANGE_AVOIDER_H
#define ORANGE_AVOIDER_H
#include <inttypes.h>

extern uint8_t safeToGoForwards;
extern int32_t incrementForAvoidance;
extern void trackObject_avoid_set(void); //init 
extern void trackObject_avoid_periodic(void);
extern uint8_t moveWaypointForwards(uint8_t waypoint, float distanceMeters);
extern uint8_t increase_nav_heading(int32_t *heading, int32_t increment);
extern uint8_t chooseIncrementAvoidance(void);

#endif

