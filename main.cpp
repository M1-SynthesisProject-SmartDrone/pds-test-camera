#include <stdio.h>
#include <iostream>
#include "opencv2/opencv.hpp"

using namespace cv;

using namespace std;

int main()
{

    Mat image;

    namedWindow("Display window");

    VideoCapture cap(2);

    if (!cap.isOpened())
    {

        cout << "cannot open camera";

    }



    while (true)
    {

        cap >> image;

        imshow("Display window", image);

        bool check = imwrite("./MyImage1.png", image);

        waitKey(25);

    }
    

    return 0;

}