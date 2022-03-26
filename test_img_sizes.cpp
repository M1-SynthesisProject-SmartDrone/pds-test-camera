
#include <random>

#include <opencv2/opencv.hpp>

using namespace cv;

const int IMG_WIDTH = 640;
const int IMG_HEIGHT = 480;

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    // An image with one color only should be the tinest one
    Mat blackImage(IMG_HEIGHT, IMG_WIDTH, CV_8UC3, Scalar(0, 0, 0));

    // This image will have randomized colors, this should approach the max image size possible
    Mat coloredImage(IMG_HEIGHT, IMG_WIDTH, CV_8UC3, Scalar(0, 0, 0));
    for (int y = 0; y < coloredImage.rows; y++)
    {
        for (int x = 0; x < coloredImage.cols; x++)
        {
            coloredImage.at<Vec3b>(Point(x, y)) = Vec3b(rand() % 255, rand() % 255, rand() % 255);
        }
    }
    
    imwrite("./out/blackImage.png", blackImage);
    imwrite("./out/coloredImage.png", coloredImage);

    return 0;
}
