/*
 * Copyright (C) M. Kisantal
 *
 * This file is part of paparazzi
 *
 */
/**
 * @file "modules/sonar_mapping/sonar_mapping.c"
 * @author M. Kisantal
 * Module for processing logged data into information about cyberzoo sectors.
 */


#include <stdio.h>
#include "std.h"
#include "modules/sonar_mapping/sonar_mapping.h"
#include "filters/sonar_median_filter.h"

#ifndef FILE_LOGGER_PATH
#define FILE_LOGGER_PATH /data/ftp/internal_000
#endif



struct MapZoneStruct {
        double lng;
        double lat;
        int safe;
        int checked;        
};



int process_log(void) {

        FILE *logged = fopen("/data/ftp/internal_000/map.csv","r");
        FILE *nozero = fopen("/data/ftp/internal_000/map_nozero.csv","w");

        char header[200];
        int counter;
        float sonar, filtered_sonar;
        float unused1, unused2, unused3, unused4, gpsx, gpsy, alt;
        
        
        
        int linecounter = 0;
        int i = 0;
        int j = 0;
        int k = 0;
        struct MedianFilter myfilter;

        // defining Obstacle Zone and SP points
        float SP[5][3]; // array size could be smaller,but now it fixes messed up indexing
        SP[1][1] = 51.990657;    SP[1][2] = 4.376748;
        SP[2][1] = 51.990601;    SP[2][2] = 4.376748;
        SP[3][1] = 51.990601;    SP[3][2] = 4.376845;
        SP[4][1] = 51.990657;    SP[4][2] = 4.376845;


        fscanf(logged,"%199[^\n]\n",header);

        while( fscanf(logged,
                                "%d, %f, %f, %f, %f, %f, %f, %f, %f", 
                                &counter, &unused1, &sonar, &unused2, &unused3, &unused4, &gpsx, &gpsy, &alt) >= 1)
        {
                if ((sonar != 0)&&(counter % 35 ==0)){
                        linecounter++;
                        alt = alt / 1000;               // OptiTrack alt to meters
                        sonar = -(sonar - (alt-0.25));  // removing altitude variations
                        gpsx = gpsx / 10000000;         // obtaining float coordinates
                        gpsy = gpsy / 10000000;         // obtaining float coordinates
                        
                        // saving subsampled measurements without zero sonar entries                        
                        fprintf(nozero,"%d, %f, %.7f, %.7f\n", counter, sonar, gpsx, gpsy);                
                }
        
        }

        fclose(logged);
        fclose(nozero);

        FILE *nozero2 = fopen("/data/ftp/internal_000/map_nozero.csv","r");
        float sonar_array[linecounter];
        float gpsx_array[linecounter];
        float gpsy_array[linecounter];

        // reading sonar measurements into array
        while( fscanf(nozero2,"%d, %f, %f, %f", &counter, &sonar_array[i], &gpsx_array[i], &gpsy_array[i]) >= 1){i++;}      

        fclose(nozero2);
        FILE *filtered = fopen("/data/ftp/internal_000/map_filtered_sonar.csv","w"); 



        init_median_filter(&myfilter);

        // filtering
        for (i=0; i<3; i++){
                filtered_sonar = update_median_filter(&myfilter,sonar_array[i]);
                filtered_sonar = sonar_array[i]; // overwrt zeros with unfiltrd val
                //fprintf(filtered,"%f, %f\n",sonar_array[i], filtered_sonar);                   
        }
        for (i=3; i<linecounter; i++){
                filtered_sonar = update_median_filter(&myfilter,sonar_array[i]);
                fprintf(filtered,"%f, %f, %.7f, %.7f\n",sonar_array[i-2], filtered_sonar, gpsx_array[i-2], gpsy_array[i-2]);                   
        }



        // close  file "map_filtered_sonar.csv"
        fclose(filtered);

// -------------- Filtered values processed to MAP --------------------


        //defining the grid
        int CellNumX = 14;
        int CellNumY = 14;
 
        struct MapZoneStruct MapZone[CellNumY][CellNumX]; // cell num other order?
        
        double LongIncrement = (SP[3][2] - SP[2][2]) /CellNumX;
        double LatIncrement  = (SP[4][1] - SP[3][1]) /CellNumY;

        // DEFINING THE SECTORS ON MAP
        for (j=0; j<CellNumY; j++){              // blocks are defined by left bottom corner coordinates
                for (i=0; i<CellNumX; i++){
                        MapZone[i][j].lng  = SP[2][2] + i * LongIncrement;
                        MapZone[i][j].lat  = SP[2][1] + j * LatIncrement;
                        MapZone[i][j].safe = 1; // blocks initialized as safe
                        MapZone[i][j].checked = 0; // blocks initialized as unchecked
                }
        
        };

        
        // sorting the measurements to SECTORS
        int FLAG_long = 0;
        int FLAG_lat = 0;

        int sonarTreshold = 1.5; // treshold is 2 meters
        float gpsxC, gpsyC;
        float sonarC, filtered_sonarC;

        FILE *read_filtered = fopen("/data/ftp/internal_000/map_filtered_sonar.csv","r");

        // OBSTACLE LOCALIZATION LOOP
        for (k=0; k<linecounter; k++){
                // read a line from map_filtered_sonar.csv
                
                fscanf(read_filtered,"%f, %f, %f, %f", &sonarC, &filtered_sonarC, &gpsyC, &gpsxC); // LAT LON SWITCHED HERE!!!!
                
                // find cell longitude for measurement k
                for(i=0; i<CellNumX; i++){
                        //printf("map = %f\n", MapZone[i][1].lng);
                        if (gpsxC < MapZone[i][1].lng){
                                if (i !=0){FLAG_long = i-1;}
                                else {FLAG_long = 0;};
                                break;
                        
                        } else {if (i == CellNumX - 1){FLAG_long = i;};}
                }
                // find cell lattitude for measurement k
                for(j=0; j<CellNumY; j++){
                        if (gpsyC < MapZone[1][j].lat){
                                if(j != 0){FLAG_lat = j-1;}
                                else {FLAG_lat = 0;}
                                break;
                        } else {if (j == CellNumY - 1){FLAG_lat = j;};}
                }
                
                MapZone[FLAG_long][FLAG_lat].checked = 1; // set cell to checked

                if (filtered_sonarC > sonarTreshold){      // if there is an obstacle, cell is unsafe
                        MapZone[FLAG_long][FLAG_lat].safe = 0;
                }      
        };  // end file loop 

        fclose(read_filtered);

        FILE *final_map = fopen("/data/ftp/internal_000/000_final_map.csv","w");

         for (j=0; j<CellNumY; j++){              // blocks are defined by left bottom corner coordinates
                for (i=0; i<CellNumX; i++){
                        fprintf(final_map,"%f, %f, %d, %d\n", 
                        MapZone[i][j].lng, 
                        MapZone[i][j].lat, 
                        MapZone[i][j].safe, 
                        MapZone[i][j].checked);
                }
        
        }; 
        fclose(final_map);


// visualization

FILE *visualization = fopen("/data/ftp/internal_000/map_visualization.csv","w");

for (j=CellNumY-1; j>0; j--){
                for (i=0; i<CellNumX; i++){
                        
                        if(MapZone[i][j].checked == 1){
                                if(MapZone[i][j].safe == 1){
                                        fprintf(visualization, "   ");  //checked and safe
                                } else {fprintf(visualization, " x ");} // checked and obstacle
                        } else {fprintf(visualization, " . ");};        // unchecked
                }
        fprintf(visualization, "\n");
        }; 


        fprintf(visualization, "\n\n\n checked and safe: [  ]   checked and obstacle [ x ]   unchecked [ . ]\n");
        fclose(visualization);
return 0;


}


