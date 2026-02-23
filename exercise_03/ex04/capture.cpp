#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

char difftext[20];


int main()
{
    Mat mat_frame, mat_gray, mat_diff, mat_gray_prev;
    VideoCapture cap("video.mpeg");

    unsigned int diffsum, maxdiff;
    double percent_diff;

    if(!cap.isOpened()) 
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    cap >> mat_frame;
    cv::cvtColor(mat_frame, mat_gray, COLOR_BGR2GRAY);    

    mat_diff = mat_gray.clone();
    mat_gray_prev = mat_gray.clone();

    maxdiff = (mat_diff.cols)*(mat_diff.rows)*255;
    string output_folder = "frames/";

    cv::GaussianBlur(mat_gray, mat_gray, cv::Size(5,5), 0);

    while(1)
    {
	cap >> mat_frame;
	
	cv::cvtColor(mat_frame, mat_gray, COLOR_BGR2GRAY);
	cv::GaussianBlur(mat_gray, mat_gray, cv::Size(5,5), 0);

	absdiff(mat_gray_prev, mat_gray, mat_diff);
	cv::threshold(mat_diff, mat_diff, 25, 255, cv::THRESH_BINARY);
	
	 // compute center of mass
    	Moments m = moments(mat_diff, true);

    	// convert gray to BGR for display
    	Mat display;
    	cvtColor(mat_gray, display, COLOR_GRAY2BGR);

    	if(m.m00 > 0)
    	{
        	int cx = int(m.m10 / m.m00);
        	int cy = int(m.m01 / m.m00);

        	circle(display, Point(cx, cy), 5, Scalar(0,0,255), -1);
        	printf("Center of Mass: (%d, %d)\n", cx, cy);
    	}

	// worst case sum is resolution * 255
	diffsum = (unsigned int)cv::sum(mat_diff)[0]; // single channel sum

	percent_diff = ((double)diffsum / (double)maxdiff)*100.0;

        printf("percent diff=%lf\n", percent_diff);
        sprintf(difftext, "%8d",  diffsum);

        // tested in ERAU Jetson lab
	if(percent_diff > 0.5) cv::putText(mat_diff, difftext, Point(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(200,200,250), 1, LINE_AA);
	namedWindow("FRAME", WINDOW_NORMAL);
	resizeWindow("FRAME", 720, 405);
	namedWindow("PREV FRAME", WINDOW_NORMAL);
        resizeWindow("PREV FRAME", 720, 405);
	namedWindow("DIFF (GRAY)", WINDOW_NORMAL);
        resizeWindow("DIFF (GRAY)", 720, 405);
	namedWindow("RED CIRCLE FRAME", WINDOW_NORMAL);
	resizeWindow("RED CIRCLE FRAME", 720, 405);

	cv::imshow("FRAME", mat_gray);
	cv::imshow("PREV FRAME", mat_gray_prev);
	cv::imshow("DIFF (GRAY)", mat_diff);
	cv::imshow("RED CIRCLE FRAME", display);

        char c = waitKey(); // take this out or reduce
        if( c == 'q' ) break;

	mat_gray_prev = mat_gray.clone();
    }

};
