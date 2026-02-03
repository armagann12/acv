#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main()

{
	Mat img = imread("Trees.jpeg");
	imwrite("Trees.ppm",img);

	return 0;
}
