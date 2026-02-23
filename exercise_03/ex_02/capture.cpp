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
    Mat gray, canny_frame, cdst, frame;
    vector<Vec4i> lines;

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

        Canny(frame, canny_frame, 50, 200, 3);

        cvtColor(canny_frame, cdst, COLOR_GRAY2BGR);
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        HoughLinesP(canny_frame, lines, 1, CV_PI/180, 100, 50, 10);

        for( size_t i = 0; i < lines.size(); i++ )
        {
          Vec4i l = lines[i];
          line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
        }

     
        if(frame.empty()) break;

        imshow("Capture Example", frame);

        char c = waitKey(10);
        if( c == 'q' ) break;
    }

    capture.release();
    destroyWindow("Capture Example");
}
