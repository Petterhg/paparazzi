/*
 * Copyright (C) M. Kisantal
 *
 * This file is part of paparazzi
 *
 */
/**
 * @file "modules/periodic_switch/periodic_switch.h"
 * @author M. Kisantal
 * switching periodic functions on/off from the flightplan
 */

#ifndef PERIODIC_SWITCH_H
#define PERIODIC_SWITCH_H

extern int logger_flag;
extern int trajectory_flag;

void init_pediodic_switch(void);
extern int logger_switch(int, int);

#endif

