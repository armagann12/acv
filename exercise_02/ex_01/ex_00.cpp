#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv,
        "{@input   |../data/lena.jpg|input image}"
        "{ksize   k|1|ksize (hit 'K' to increase its value)}"
        "{scale   s|1|scale (hit 'S' to increase its value)}"
        "{delta   d|0|delta (hit 'D' to increase its value)}"
        "{help    h|false|show help message}");

    cout << "The sample uses Sobel OpenCV functions for edge detection\n\n";
    parser.printMessage();
    cout << "\nPress 'ESC' to exit program.\nPress 'R' to reset values (ksize will be -1 equal to Scharr function)";

    // Declare the variables
    Mat image, src, grad;
    const String window_name = "Sobel Demo - Simple Edge Detector";
    int ksize = parser.get<int>("ksize");
    int scale = parser.get<int>("scale");
    int delta = parser.get<int>("delta");
    int ddepth = CV_16S;

    String imageName = parser.get<String>("@input");

    // Load the image
    image = imread(imageName, IMREAD_COLOR); // Load an image
    if (image.empty())
    {
        printf("Error opening image: %s\n", imageName.c_str());
        return 1;
    }

    for (;;)
    {
        cout << "\nk_size : " << ksize << " scale : " << scale << " delta : " << delta << endl;

        // Remove noise by blurring with a Gaussian filter (kernel size = 3)
        GaussianBlur(image, src, Size(3, 3), 0, 0, BORDER_DEFAULT);

        // Extract a single color channel (e.g., Blue channel)
        Mat single_channel;
        extractChannel(src, single_channel, 0); // 0 = Blue channel, 1 = Green, 2 = Red

        // Sobel edge detection
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;

        // Apply Sobel in both X and Y directions
        Sobel(single_channel, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
        Sobel(single_channel, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);

        // Check if grad_x and grad_y have the same size
        if (grad_x.size() != grad_y.size())
        {
            cout << "Error: grad_x and grad_y have different sizes!" << endl;
            return -1;
        }

        // Convert back to CV_8U
        convertScaleAbs(grad_x, abs_grad_x);
        convertScaleAbs(grad_y, abs_grad_y);

        // Check if the matrices are the same size before using addWeighted
        if (abs_grad_x.size() == abs_grad_y.size())
        {
            addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
            imshow(window_name, grad);
        }
        else
        {
            cout << "Error: Matrices have mismatched sizes!" << endl;
        }

        // Key events for adjusting parameters or exiting
        char key = (char)waitKey(0);
        if (key == 27)  // ESC key to exit
        {
            return 0;
        }

        if (key == 'k' || key == 'K')
        {
            ksize = ksize < 30 ? ksize + 2 : -1;
        }

        if (key == 's' || key == 'S')
        {
            scale++;
        }

        if (key == 'd' || key == 'D')
        {
            delta++;
        }

        if (key == 'r' || key == 'R')
        {
            scale = 1;
            ksize = -1;
            delta = 0;
        }
    }

    return 0;
}

