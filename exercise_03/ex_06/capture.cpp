// Written by Sam Siewert and XXX
//
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

// Pipeline - Video -> Blur -> -> Threshold -> COM -> imshow/frames

// 3x3 gaussian blur
const int kernelSize = 3;
const int gaussianKernel[3][3] = { {1, 2, 1},
                                   {2, 4, 2},
                                   {1, 2, 1} };
const int kernelSum = 16;

Mat manualGaussianBlur(const Mat &src)
{
    Mat dst = Mat::zeros(src.size(), src.type());

    for(int y = 1; y < src.rows - 1; y++)
    {
        for(int x = 1; x < src.cols - 1; x++)
        {
	    int sum = 0;
            for(int ky = 0; ky < kernelSize; ky++)
            {
                for(int kx = 0; kx < kernelSize; kx++)
                {
                    uchar pixel = src.at<uchar>(y + ky - 1, x + kx - 1);
                    int k = gaussianKernel[ky][kx];

                    sum += pixel * k;
                }
            }

            dst.at<uchar>(y,x) = sum / kernelSum;
        }
    }

    return dst;
}

int main()
{
    Mat new_image, frame;

    VideoCapture cap("video.mpeg");

    if(!cap.isOpened())
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    while(1)
    {
        cap >> frame;
        if(frame.empty()) break;

        new_image = Mat::zeros(frame.size(), CV_8UC1);
	
        // red channel extract
        for(int y = 0; y < frame.rows; y++)
        {
            for(int x = 0; x < frame.cols; x++)
            {
		new_image.at<uchar>(y,x) = saturate_cast<uchar>(frame.at<Vec3b>(y,x)[2]);
            }
        }

        Mat blurred = manualGaussianBlur(new_image);
	
 	for (int y = 0; y < blurred.rows; y++)
	{
		for (int x = 0; x < blurred.cols; x++)
		{
			uchar &pixel = blurred.at<uchar>(y,x);
			
			if (pixel > 50)
				pixel = 255;
			else
				pixel = 0;
		}
	}
	

        double sumX = 0;
	double sumY = 0;
	double totalMass = 0;

        for(int y = 0; y < blurred.rows; y++)
        {
            for(int x = 0; x < blurred.cols; x++)
            {
                uchar &pixel = blurred.at<uchar>(y,x);
                if(pixel > 0)
                {
                    sumX += x * pixel;
                    sumY += y * pixel;
                    totalMass += pixel;
                }
            }
        }

        if(totalMass > 0)
        {
            int centerX = sumX / totalMass;
            int centerY = sumY / totalMass;
            circle(blurred, Point(centerX, centerY), 8, Scalar(0,255,0), -1);
        }
	
	namedWindow("Original Image", WINDOW_NORMAL);
	resizeWindow("Original Image", 960, 540);
        imshow("Original Image", frame);

	namedWindow("Final Image", WINDOW_NORMAL);
	resizeWindow("Final Image", 960, 540);
        imshow("Final Image", blurred);
	waitKey(200);
    }

    return 0;
}
