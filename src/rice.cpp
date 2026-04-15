#include <opencv2/opencv.hpp>
#include <stdlib.h>

#define UMBRAL 60

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    const char *path = "../images/rice.jpg";
    Mat img = imread(path, IMREAD_COLOR);

    std::vector<Mat> channels;
    split(img, channels);
    Mat processed = channels[0];
    int rows = img.size().height, cols = img.size().width;
    imshow("Before", processed);
    Mat out = Mat(rows, cols, CV_8U);

    for (size_t i = 0; i < img.size().height; i++) {
        int min = 255;
        for (size_t j = 0; j < img.size().width; j++) {
            if (min > processed.at<uchar>(i, j)) {
                min = processed.at<uchar>(i, j);
            }
        }

        for (size_t j = 0; j < img.size().width; j++) {
            processed.at<uchar>(i, j) = processed.at<uchar>(i, j) - min;
            out.at<uchar>(i, j) = processed.at<uchar>(i, j) < UMBRAL? 0: 255;
        }
    }
    imshow("After processed", processed);
    imshow("Rice segmented", out);

    waitKey(0);
    return EXIT_SUCCESS;
}
