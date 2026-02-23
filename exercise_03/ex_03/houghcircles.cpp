/*
 *
 *  Example by Sam Siewert 
 *
 *  Updated for OpenCV 3.1
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

#define HRES 640
#define VRES 480


int main( int argc, char** argv )
{
    namedWindow("Capture Example", WINDOW_AUTOSIZE);

    int dev=0;
    VideoCapture capture(dev);
    Mat gray, frame;

    vector<Vec3f> circles;

    if(argc > 1)
    {
        sscanf(argv[1], "%d", &dev);
        printf("using %s\n", argv[1]);
    }
    else if(argc == 1)
        printf("using default\n");

    else
    {
        printf("usage: capture [dev]\n");
        exit(-1);
    }

    capture.set(CAP_PROP_FRAME_WIDTH, HRES);
    capture.set(CAP_PROP_FRAME_HEIGHT, VRES);

    while(1)
    {
        capture >> frame;
	if(frame.empty()) break;

        
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        GaussianBlur(gray, gray, Size(9,9), 2, 2);

	// threshold, sensitivity, mindiff maxdiff
        HoughCircles(gray, circles, HOUGH_GRADIENT, 40, gray.rows/16, 100, 30, 1, 30);

        printf("circles.size = %ld\n", circles.size());

        for( size_t i = 0; i < circles.size(); i++ )
        {
          Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
          int radius = cvRound(circles[i][2]);
          
	  // circle center
          circle( frame, center, 3, Scalar(255,0,0), 3, LINE_AA );
          // circle outline
          circle( frame, center, radius, Scalar(0,255,0), 3, LINE_AA );
        }

        imshow("Capture Example", frame);

        char c = waitKey(10);
        if( c == 'q' ) break;
    }

    capture.release();
    destroyWindow("Capture Example");
    return (0);
};
