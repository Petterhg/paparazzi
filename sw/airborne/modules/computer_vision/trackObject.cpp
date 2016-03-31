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
 * @file "modules/computer_vision/trackObject.cpp"
 * @author Tigran Mkhoyan
 * opencv
 */


#include "trackObject.h"
#include "setFilterHSV.h"
//#include <datalink/telemetry.h"
//#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/objdetect/objdetect.hpp>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string.h>
# include <stdint.h>
using namespace std;
using namespace cv;

/* HSV parameter are fetched from GUI settings. We need to include setfilter.h
//initial MIN and MAX HSV filter values

//init hardset choose value
int H_MIN = 0;
int H_MAX = 80;
int S_MIN = 61;
int S_MAX = 174;
int V_MIN = 40;
int V_MAX = 242;
*/


//default capture width and height bebob std frame size is 272*272
//alternatively use param w,h
const int FRAME_WIDTH = 272;
const int FRAME_HEIGHT = 272;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;
//minimum and maximum object newArea
const int MIN_OBJECT_AREA = 20*20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH / 1.5;
//const int TRESHOLD_OBJECT_AREA = 15000;//MIN_OBJECT_AREA*1.5;  //is global now

//message action
const string Toleft = "Turn left";
const string Toright= "Turn Right";
const string noaction = "Straight";
//message notify
const string noisy = "Noise!Adjust filter";
const string danger = "Danger: Colision course!";
const string nodanger = "Objects ignored no collision";
const string notfound = "No Objects found";
const string found = "Objects found!";

const bool debugon = true;


//debug


/* TODO: get distortion correction to work.
//These are calibrated camera parameters: cam and distcoeff matrix
   Mat cameraMatrix = (Mat_<double>(3,3) << 1.3076265909084114e+02, 0., 1.3550000000000000e+02, 0.,
    1.3076265909084114e+02, 1.3550000000000000e+02, 0., 0., 1);
    Mat distCoeffs = (Mat_<double>(5,1) <<  -3.9244810249976891e-01, 1.8293610745626351e-01, 0, 0,
    -4.5646307231660514e-02);
*/

//struct is global no need to define an instance. Functions in trackobject.cpp we pass values later on by adres
//parameters CollisionParam;
//define pointer instance    
//parameters *CollisionParamPointer;

 //cout << "Area:" << CollisionParamPointer->Area << endl;

//functions used
void morphsOps(Mat &thresh);
void drawObject(int x, int y, Mat &frame);
string intToString(int number);
void trackFilteredObject(Mat threshold, parameters &outparam, Mat &image);
void debug(const int line);
void putBackImage(char* img,Mat &image, int &width, int &height);
void putString(parameters &outdata, const string &instring); 



/*------START OF MAIN  ------------*/
int tarckobjectMain(char* img, int width, int height)
{
    //Matrix to store each frame of the webcam feed
    Mat image;
    //Matrix storage for Hsv image
    Mat HSV;
    //Matrix for binary threshold image
    Mat threshold;
   
   //settiongs + initializing 

    //bool trackObjects = true; //this is a global now
    bool useMorphOps = true;  //default
   //double refArea = 0;        //global

    //create instance of struct pointer type definition has been declared in header 
  //parameters *CollisionParamPointer;

    //place image in 8 bit container. img is pointer to image buffer.
    Mat M(width,height, CV_8UC2, img);

    //color conversions
    //transform to standard openCV format to start workin on it
    //cvtColor(M, image, CV_YUV2RGB_Y422)  

	cvtColor(M, image, CV_YUV2BGR_Y422);   //BGR is default opencv processing format  
    cvtColor(image, HSV, COLOR_BGR2HSV);   //hsv needed to make treshold image 
    
    //Mat undist; //to hold undistorted image
	//undistort(image, undist, cameraMatrix, distCoeffs);
   
    //filter HSV image between values and store filtered image to threshold matrix
    inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);

       
    //perform morphological ops on threshold image to eliminate noise
    //if (useMorphOps)
        morphsOps(threshold);
      //  CollisionParam.targetPosy = 2;

//cout << "Param targetPosy:" << CollisionParam.targetPosy << endl;

//debug(154);
    //pass in the threshold frame to our object tracking function
    //and reference of struct to be populated with data
//debug(158);
    if (trackObject)  //global setting
      trackFilteredObject(threshold, CollisionParam, image);
//debug(159);

    //test
    //putText(image, "Too much noise!", Point(0, 10), 1, 2, Scalar(0, 255, 0), 2);

    //tranform to grey if needed.
    //cvtColor(image, gray, CV_YUV2GRAY_Y422);
    // Convert back to YUV422, and put it in place of the original image
//debug(167);
    putBackImage(img, threshold, width, height);
 //   debug(169);

    return 0;

}
/*------END OF MAIN  ------------*/


/*------START   MODULE FUNCTIONS   ------------*/

string intToString(int number) {
    stringstream ss;
    ss << number;
    return ss.str();
}


void drawObject(int x, int y, Mat &frame) {
    //use openCV drawing functions to draw crosshairs on tracked image
    //using 'if' statements to prevent drawing off the window

    circle(frame, Point(x, y), 20, Scalar(0, 255, 0), 2);

    if (y - 25 > 0)
        line(frame, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
    else line(frame, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);
    if (y + 25 < FRAME_HEIGHT)
        line(frame, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
    else line(frame, Point(x, y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);
    if (x - 25 > 0) 
        line(frame, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
    else line(frame, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);
    if (x + 25 < FRAME_WIDTH)
        line(frame, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
    else line(frame, Point(x, y), Point(FRAME_WIDTH, y), Scalar(0, 255, 0), 2);

    putText(frame, intToString(x) + ", " + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2);
}

void morphsOps(Mat &thresh) {
    //create structuring element that will be used to "dilate" and "erode" image
    //the element chose here is a 3px by 3px rectangle
    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

    erode(thresh, thresh, erodeElement);
    erode(thresh, thresh, erodeElement);

    dilate(thresh, thresh, dilateElement);
    dilate(thresh, thresh, dilateElement);
}

void trackFilteredObject(Mat threshold, parameters &outparam, Mat &image) {
    Mat temp;
    int x = 0;
    int y = 0;
 //   char* msgaction;
 //  char* msgdebug;
    double newArea ;
    CollisionParam.Area = 0;
    putString(CollisionParam,"************");
    putString(CollisionParam,"********");
     //cout << outparam.wheretoGo << endl;
      //cout << outparam.trackState << endl;

    //outparam.CollisionDetect = false; this is set false as last statement collision to false.
    debug(227);
    threshold.copyTo(temp);
    vector< vector<Point> > contours; //these two vectors needed to output of findContours
    vector<Vec4i> hierarchy;
    //cout << outparam.wheretoGo << endl;
    //find contours of filtered image using openCV findContours function
    findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    //use moments method to find our filtered object
  
    if (hierarchy.size() > 0) 
    {
       outparam.nObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if (outparam.nObjects < MAX_NUM_OBJECTS) 
        {
            for (int index = 0; index >=0; index = hierarchy[index][0]) 
            {
                Moments moment = moments((cv::Mat)contours[index]);
                double newArea = moment.m00;

                /*if the newArea is less than 20px by 20px, then it's probably just noise
                if the newArea is the same as the 3/2 of the image size, then it's probably the bad filter
                we only want the larges newArea therefore newnewArea is compared to previous Area that we get as global
                from previous iteration*/
                if (newArea > MIN_OBJECT_AREA && newArea < MAX_OBJECT_AREA && newArea > outparam.Area) 
                {
                    x = moment.m10/newArea;
                    y = moment.m01/newArea;
                    outparam.objectDetect = true;
                    outparam.Area = newArea;
                } 
                else 
                {
                    outparam.objectDetect= false;
                    putString(CollisionParam, notfound);
                    debug(262);
                }

            }
            //notify we found an object
            if (outparam.objectDetect = true) 
            {
                //strcpy(outparam.trackState, found);
                //putText(image, "Tracking object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
                //draw object location on image
                drawObject(x, y, threshold);
            
        
                //check the size and offset from center and trigger warning
                if (outparam.Area>tresholdArea && abs(whalf-x)<=offsetPosx && abs(yhalf-y)<=offsetPosy) 
                { 
                //putText(image ,"Colision!" , Point(0, 10), 1, 1, Scalar(0, 0, 255), 2); 
                    outparam.CollisionDetect= true;
                    putString(CollisionParam,danger);
                    debug(291);
                    //decide tactics
                   if ((whalf-x)< 0)
                   {
                    cout << (whalf-x) << endl;
                    putString(CollisionParam,Toleft); //if the number is <0 object is in the RH so turn left
                    CollisionParam.actionFlag =2;
                  debug(294);
                   }
                   else {
                    putString(CollisionParam,Toright);
                    CollisionParam.actionFlag =1;
                   debug(296);
               }

                } 
               // else if () 
                else 
                {
                    outparam.CollisionDetect = false ; 
                    debug(303);
                    putString(CollisionParam,noaction);
                    putString(CollisionParam,nodanger);
                    CollisionParam.actionFlag =0;
                    debug(305);
                }
            }
        }
        else 
        {
             debug(312);
        putString(CollisionParam,noisy);
        CollisionParam.actionFlag =0;
        }

    }


  
    outparam.targetPosx = x;
    outparam.targetPosy = y;
      /* struct content 
    outparam.Area = refArea;
    outparam.CollisionDetect = Collision;
    outparam.objectDetect = objectFound;
    outparam.nObjects = numObjects;
    outparam.wheretoGo[10] 
    outparam.trackState[30];
    */
  
}


void putBackImage(char* img, Mat &image, int &width, int &height){

/*
    for (int row=0; row <height; row++){
        for (int col=0; col <width; col++){
           // img[(row*width+col)] = image.at<uint8_t>(row,col);
          //  img[(row*width+col)*2] = 127;
    }
}
}
    */

        for (int row=0; row <height; row++){
        for (int col=0; col <width; col++){
            img[(row*width+col)*2+1] = image.at<uint8_t>(row,col);
          //     debug(336);
            img[(row*width+col)*2] = 127;
          //  debug(336);
        }

    //debug
    //printf("trackObjectLOOP\n");
    }

}

void debug(const int line) {

if(debugon==true)
{
//ine = "Passed line: " + line;
cout << "Passed line: " <<  intToString(line) << endl;
}

}

void putString(parameters &outdata, const string &instring){

    if(instring.size()<=10)
    { //if size <= 10 char its is a action message, otherwise status message
        sprintf(outdata.wheretoGo, "%.10s", instring.c_str()); //protect buffer with 10 char max
    }
    else 
    {
        sprintf(outdata.trackState,"%.30s", instring.c_str()); 
    }

}




/*------END  MODULE FUNCTIONS   ------------*/