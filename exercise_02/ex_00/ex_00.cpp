#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;
using namespace cv;

// second arg -> number of frame
int main(int argc, char **argv)
{
	if (argc < 2)
	{
		cout << "2 arg needed" << endl;
		return (0);
	}
	VideoCapture cap("Big_Buck_Bunny_720p.mp4");
	int number_of_frame = atoi(argv[1]);

	if (!cap.isOpened())
	{
		std::cerr << "Error : could not open video file" << std::endl;
		return -1;
	}

	Mat frame;
	int frame_count = 0;
	string output_folder = "frames/";
	while (true)
	{
		if (frame_count >= number_of_frame)
			break;
		cap >> frame;
		
		if (frame.empty())
			break;

		string filename = output_folder + "frame_" + to_string(frame_count) + ".jpeg";
		if (imwrite(filename, frame))
		{
			cout << "--Saved--" << filename << endl;
			frame_count++;
		}
		else
			break;

		if (cv::waitKey(30) == 'c')
			break;
	}
	
	cap.release();
	cv::destroyAllWindows();
	
	return 0;
}
