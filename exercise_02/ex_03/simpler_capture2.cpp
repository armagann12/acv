/*
 *
 *  Example by Sam Siewert 
 *
 *  Created for OpenCV 4.x for Jetson Nano 2g, based upon
 *  https://docs.opencv.org/4.1.1
 *
 *  Tested with JetPack 4.6 which installs OpenCV 4.1.1
 *  (https://developer.nvidia.com/embedded/jetpack)
 *
 *  Based upon earlier simpler-capture examples created
 *  for OpenCV 2.x and 3.x (C and C++ mixed API) which show
 *  how to use OpenCV instead of lower level V4L2 API for the
 *  Linux UVC driver.
 *
 *  Verify your hardware and OS configuration with:
 *  1) lsusb
 *  2) ls -l /dev/video*
 *  3) dmesg | grep UVC
 *
 *  Note that OpenCV 4.x only supports the C++ API
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <thread>
#include <chrono>

using namespace cv;
using namespace std;

// See www.asciitable.com
#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)

int main()
{
   VideoCapture cam0(0);
   namedWindow("video_display");
   char winInput;

   if (!cam0.isOpened())
   {
       exit(SYSTEM_ERROR);
   }

   cam0.set(CAP_PROP_FRAME_WIDTH, 640);
   cam0.set(CAP_PROP_FRAME_HEIGHT, 480);

   struct timespec start, end;
   double diff;
   int frame_count = 0;

   clock_gettime(CLOCK_MONOTONIC, &start);


   while (1)
   {
      Mat frame;

      cam0.read(frame);

      imshow("video_display", frame);
      clock_gettime(CLOCK_MONOTONIC, &end);

      diff = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
      frame_count++;
      cout << diff << "----" << frame_count << endl;

      if (diff >= 60.0)
      {
	      double average_fps = frame_count / diff;
	      cout << "average fps over : " << diff << " seconds : " << average_fps << endl;
	      frame_count = 0;

	      clock_gettime(CLOCK_MONOTONIC, &start);
      }

      if ((winInput = waitKey(1)) == ESCAPE_KEY)
      {
          break;
      }
      else if(winInput == 'n')
      {
	  cout << "input " << winInput << " ignored" << endl;
      }
      
   }

   destroyWindow("video_display"); 
};
