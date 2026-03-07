#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    VideoCapture cap(0);
    if(!cap.isOpened())
    {
        cout << "Cannot open camera" << endl;
        return -1;
    }

    int frameId = 0;
    Mat src, gray, binary, mfblur, skel, temp, eroded;
    Mat element = getStructuringElement(MORPH_CROSS, Size(3,3));

    while(true)
    {
        cap >> src;
        if(src.empty())
            break;

        cvtColor(src, gray, COLOR_BGR2GRAY);

        threshold(gray, binary, 70, 255, THRESH_BINARY);
        binary = 255 - binary;

        medianBlur(binary, mfblur, 1);

        skel = Mat::zeros(mfblur.size(), CV_8UC1);
        int iterations = 0;
        bool done;
        Mat temp_mfblur = mfblur.clone();

        do
        {
            erode(temp_mfblur, eroded, element);
            dilate(eroded, temp, element);
            subtract(temp_mfblur, temp, temp);
            bitwise_or(skel, temp, skel);
            eroded.copyTo(temp_mfblur);

            done = (countNonZero(temp_mfblur) == 0);
            iterations++;
        } while(!done && (iterations < 100));

        imshow("Camera", src);
        imshow("Skeleton", skel);

        char c = (char)waitKey(30);
        if(c == 27)  // ESC key
            break;
	if (frameId > 2999)
		break;
	string filename = "frames/frame_" + to_string(frameId) + ".png";
	imwrite(filename, skel);
	frameId++;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
