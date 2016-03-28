/*
 * Copyright (C) M. Kisantal
 *
 * This file is part of paparazzi
 *
 */
/**
 * @file "modules/test_traj_update/test_traj_update.h"
 * @author M. Kisantal
 * testing periodic functions to update trajectory. Periodically checking position, then making a decision and a waypoint based upon that.
 */
#include <inttypes.h>

#ifndef TEST_TRAJ_UPDATE_H
#define TEST_TRAJ_UPDATE_H

extern float deltaX;
extern float deltaY;

extern void init_test_traj_update(void);
extern void periodic_test_traj_update();
extern void stop_test_traj_update(void);
extern int setPotentialField(int x);
extern uint8_t moveWaypointForwards(uint8_t waypoint, float distanceMeters);
extern uint8_t moveWaypointForwards2(uint8_t waypoint, float distanceMeters);

#endif

