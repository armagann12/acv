#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <filesystem>
#include <sstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat mat_frame, mat_red, mat_blue, mat_green;
    VideoCapture cap("video.mpeg");

    if(!cap.isOpened()) 
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    cap >> mat_frame;
    string output_folder = "frames/";
    int frameCount = 0;
    filesystem::create_directories(output_folder);

    while(1)
    {
	cap >> mat_frame;

	mat_red = mat_frame.clone();
	mat_green = mat_frame.clone();
        mat_blue = mat_frame.clone();

	cv::extractChannel(mat_frame, mat_blue, 0);
        cv::extractChannel(mat_frame, mat_green, 1);
	cv::extractChannel(mat_frame, mat_red, 2);
	
	stringstream filename_red, filename_blue, filename_green;
        filename_red << output_folder << "frame" << setw(4) << setfill('0') << frameCount << "-red.pgm";
        filename_blue << output_folder << "frame" << setw(4) << setfill('0') << frameCount << "-blue.pgm";
        filename_green << output_folder << "frame" << setw(4) << setfill('0') << frameCount << "-green.pgm";

        imwrite(filename_red.str(), mat_red);
        imwrite(filename_blue.str(), mat_blue);
        imwrite(filename_green.str(), mat_green);

        frameCount++;

        if(frameCount == 10){
            cout << "Saved " << frameCount << " frames" << endl;
	    break;
	}
    }
    cout << "Done - frames: " << frameCount << endl;
    return 0;

};
