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

#define HRES 720
#define VRES 480


int main( int argc, char** argv )
{
    VideoCapture capture;
    Mat gray, canny_frame, frame,frame_two;
    vector<Vec4i> linesP;
    vector<Vec2f> linesH;

    if(argc > 1)
    {
	    capture.open(argv[1]);
	    printf("using %s\n", argv[1]);
    }

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

	frame_two = frame.clone();

	cvtColor(frame, gray, COLOR_BGR2GRAY);

        Canny(gray, canny_frame, 50, 200, 3);

	linesH.clear();
	linesP.clear();
	
	// Houghline
	HoughLines(canny_frame, linesH, 1, CV_PI/180, 250, 0, 0);
    	for( size_t i = 0; i < linesH.size(); i++ )
    	{
        	float rho = linesH[i][0], theta = linesH[i][1];
        	Point pt1, pt2;
        	double a = cos(theta), b = sin(theta);
        	double x0 = a*rho, y0 = b*rho;
        	pt1.x = cvRound(x0 + 1000*(-b));
        	pt1.y = cvRound(y0 + 1000*(a));
        	pt2.x = cvRound(x0 - 1000*(-b));
        	pt2.y = cvRound(y0 - 1000*(a));
        	line( frame_two, pt1, pt2, Scalar(0,0,255), 3, LINE_8);
    	}


        HoughLinesP(canny_frame, linesP, 1, CV_PI/180, 200, 30, 10);

	// Houghlines P
        for( size_t i = 0; i < linesP.size(); i++ )
        {
          Vec4i l = linesP[i];
          line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,255,0), 3, LINE_8);
        }

	namedWindow("Houghline - example", WINDOW_NORMAL);
	resizeWindow("Houghline - example", HRES, VRES);
	imshow("Houghline - example", frame_two);
	
	namedWindow("HoughLine P - Example", WINDOW_NORMAL);
	resizeWindow("HoughLine P - Example", HRES, VRES);
	imshow("HoughLine P - Example", frame);

        char c = waitKey(10);
        if( c == 'q' ) break;
    }

    capture.release();
    destroyWindow("Capture Example");
    return 0;
}
