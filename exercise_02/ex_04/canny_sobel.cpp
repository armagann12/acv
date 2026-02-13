#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

enum Mode
{
    MODE_NONE,
    MODE_SOBEL,
    MODE_CANNY,
    MODE_ALL
};

int main()
{
    VideoCapture cap(0);  // Open default webcam

    if (!cap.isOpened())
    {
        cout << "Error: Could not open camera." << endl;
        return -1;
    }
	
    Mode currentMode = MODE_NONE;
    string mode = "NONE";
    Mat frame, gray, output;

    cout << "Press:\n";
    cout << "  n - Normal\n";
    cout << "  s - Sobel\n";
    cout << "  c - Canny\n";
    cout << "  o - Canny + Sobel\n";
    cout << "  q - Quit\n";


    struct timespec start, end;
    double diff;
    int frame_count = 0;
    clock_gettime(CLOCK_MONOTONIC, &start);
    bool changed = false;
    while (true)
    {
        cap >> frame;
        if (frame.empty()) break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        switch (currentMode)
        {
            case MODE_NONE:
		if (changed == true)
		{
			clock_gettime(CLOCK_MONOTONIC, &start);
			changed = false;
			frame_count = 0;
		}

      		clock_gettime(CLOCK_MONOTONIC, &end);

      		diff = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
      		frame_count++;
		cout << diff << "----" << frame_count << endl;

      		if (diff >= 60.0)
      		{
	      		double average_fps = frame_count / diff;
	      		cout << "Mode none - average fps over : " << diff << " seconds : " << average_fps << endl;
	      		frame_count = 0;

	      		clock_gettime(CLOCK_MONOTONIC, &start);
      		frame_count = 0;}


                output = frame;
                break;

            case MODE_SOBEL:
            {
		Mat grad_x, grad_y, abs_grad_x, abs_grad_y;
		if (changed == true)
                {
                        clock_gettime(CLOCK_MONOTONIC, &start);
                        changed = false;
                	frame_count = 0;
		}
		clock_gettime(CLOCK_MONOTONIC, &end);

                diff = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
                frame_count++;
		cout << diff << "----" << frame_count << endl;
                if (diff >= 60.0)
                {
                        double average_fps = frame_count / diff;
                        cout << "Mode Sobel - average fps over : " << diff << " seconds : " << average_fps << endl;
                        frame_count = 0;

                        clock_gettime(CLOCK_MONOTONIC, &start);
                }

                Sobel(gray, grad_x, CV_16S, 1, 0, 3);
                Sobel(gray, grad_y, CV_16S, 0, 1, 3);

                convertScaleAbs(grad_x, abs_grad_x);
                convertScaleAbs(grad_y, abs_grad_y);

                addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, output);
                break;
            }

            case MODE_CANNY:
            {
		if (changed == true)
                {
                        clock_gettime(CLOCK_MONOTONIC, &start);
                        changed = false;
                	frame_count = 0;
		}
		clock_gettime(CLOCK_MONOTONIC, &end);

                diff = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
                frame_count++;
		cout << diff << "----" << frame_count << endl;

                if (diff >= 60.0)
                {
                        double average_fps = frame_count / diff;
                        cout << "Mode Canny - average fps over : " << diff << " seconds : " << average_fps << endl;
                        frame_count = 0;

                        clock_gettime(CLOCK_MONOTONIC, &start);
                }

                Canny(gray, output, 50, 150);
                break;
            }
	    case MODE_ALL:
	    {
	        if (changed == true)
                {
                        clock_gettime(CLOCK_MONOTONIC, &start);
                        changed = false;
                	frame_count = 0;
		}
		clock_gettime(CLOCK_MONOTONIC, &end);

                diff = (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
                frame_count++;
		cout << diff << "----" << frame_count << endl;

                if (diff >= 60.0)
                {
                        double average_fps = frame_count / diff;
                        cout << "Mode Canny + Sobel - average fps over : " << diff << " seconds : " << average_fps << endl;
                        frame_count = 0;

                        clock_gettime(CLOCK_MONOTONIC, &start);
                }

		Mat canny_out, sobel_out;
		Mat grad_x, grad_y, abs_grad_x, abs_grad_y;

	    	Canny(gray, canny_out, 50, 150);


                Sobel(gray, grad_x, CV_16S, 1, 0, 3);
                Sobel(gray, grad_y, CV_16S, 0, 1, 3);

                convertScaleAbs(grad_x, abs_grad_x);
                convertScaleAbs(grad_y, abs_grad_y);

                addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, sobel_out);

		addWeighted(canny_out, 0.5, sobel_out, 0.5, 0, output);
                break;
	    }

        }
	if (output.channels() == 1)
	{
    		cvtColor(output, output, COLOR_GRAY2BGR);
	}
	putText(output,"MODE: " + mode, Point(20, 40), FONT_HERSHEY_SIMPLEX, 1.0,Scalar(0, 255, 0), 2);
        imshow("Edge Viewer", output);

        char key = (char)waitKey(1);

        if (key == 'q') break;
        if (key == 'n'){
		mode = "NONE";
	       	currentMode = MODE_NONE;
		changed = true;
	}
	if (key == 's'){ 
		mode = "SOBEL";
		currentMode = MODE_SOBEL;
		changed = true;
        }

        if (key == 'c'){ 
		mode = "CANNY";
		currentMode = MODE_CANNY;
		changed = true;
        }

	if (key == 'o'){
		mode = "ALL";
	       	currentMode = MODE_ALL;
		changed = true;
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}

