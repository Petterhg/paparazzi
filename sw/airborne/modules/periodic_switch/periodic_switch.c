/*
 * Copyright (C) M. Kisantal
 *
 * This file is part of paparazzi
 *
 */
/**
 * @file "modules/periodic_switch/periodic_switch.c"
 * @author M. Kisantal
 * switching periodic functions on/off from the flightplan
 */

#include "modules/periodic_switch/periodic_switch.h"

int logger_flag;
int trajectory_flag;

void init_pediodic_switch(void){
logger_flag=0;
}

extern int logger_switch(int x,int y) {
switch(x){
        case 1:
         logger_flag=1;
         break;
        case 0:
         logger_flag=0;
         break;
        default:
         logger_flag=0;
}

switch(y){
        case 1:
         trajectory_flag=1;
         break;
        case 0:
         trajectory_flag=0;
         break;
        default:
         trajectory_flag=0;
}

return 0;
}


