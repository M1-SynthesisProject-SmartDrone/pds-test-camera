
#include <sys/stat.h>
#include <random>
#include <vector>
#include <chrono>

#include <opencv2/opencv.hpp>
#include <libpng/png.h>

using namespace cv;
using namespace std;

const int IMG_WIDTH = 640;
const int IMG_HEIGHT = 480;

void testBasic();

void testCompressionSize();

void testCompressionTime();

int main(int argc, char const* argv[])
{
    srand(time(NULL));

    cout << "==== Compression size ====" << "\n";
    testCompressionSize();
    cout << "\n\n==== Compression time ====" << "\n";
    testCompressionTime();

    return 0;
}

Mat createRandomImage()
{
    Mat randomImage(IMG_HEIGHT, IMG_WIDTH, CV_8UC3, Scalar(0, 0, 0));
    for (int y = 0; y < randomImage.rows; y++)
    {
        for (int x = 0; x < randomImage.cols; x++)
        {
            randomImage.at<Vec3b>(Point(x, y)) = Vec3b(rand() % 255, rand() % 255, rand() % 255);
        }
    }
    return randomImage;
}

long getFileSize(string filename)
{
    struct stat stat_buffer;
    int rc = stat(filename.c_str(), &stat_buffer);
    return rc == 0 ? stat_buffer.st_size : -1;
}

void testBasic()
{
    // An image with one color only should be the tinest one
    Mat blackImage(IMG_HEIGHT, IMG_WIDTH, CV_8UC3, Scalar(0, 0, 0));

    // This image will have randomized colors, this should approach the max image size possible
    Mat coloredImage = createRandomImage();

    imwrite("./out/blackImage.png", blackImage);
    imwrite("./out/coloredImage.png", coloredImage);
}

vector<int> createCompressionParams(int compressionRate)
{
    return vector<int> { IMWRITE_PNG_COMPRESSION, compressionRate, IMWRITE_PNG_STRATEGY, IMWRITE_PNG_STRATEGY_RLE };
}

Mat loadImage()
{
    return imread("./imgs/test1.png");
}

void testCompressionSize()
{
    Mat coloredImage = loadImage();
    imwrite("./out/compression_base.png", coloredImage);

    // Save the image for each compression rate (default setting is set to 1)
    for (int i = 0; i < 10; i++)
    {
        cout << "Create image with compression " << i << "\n";
        auto compressionParams = createCompressionParams(i);

        string filename = "./out/compression_" + to_string(i) + ".png";
        imwrite(filename, coloredImage, compressionParams);
        long fileSize = getFileSize(filename);
        if (fileSize <= 0)
        {
            cout << "File was probably not created (error)" << "\n";
        }
        else
        {
            cout << "Size of the file : " << fileSize << " bytes ("
                << fileSize / 1000 << " ko)\n";
        }
    }
}

void testCompressionTime()
{
    const int nbIterations = 100;
    vector<uchar> buffer(2048); // 2mb should be enough for any image (max saved is ~970kb)
    Mat coloredImage = loadImage();

    for (int i = 0; i < 10; i++)
    {
        cout << "Start" << 100 << " saves for image with compression " << i << "\n";
        auto compressionParams = createCompressionParams(i);
        
        auto startClock = chrono::steady_clock::now();
        
        imencode(".png", coloredImage, buffer, compressionParams);
        
        auto endClock = chrono::steady_clock::now();
        cout << "Time spent : " << chrono::duration_cast<chrono::milliseconds>(endClock - startClock).count() 
            << "ms" << "\n"; 
    }
    
}