#include <opencv2/opencv.hpp>
#include <stdlib.h>

#define MAX_PIX 255

int main(int argc, char **argv) {
    if (argc < 2 ) {
        perror("Must provide a path");
        return EXIT_FAILURE;
    }
    char* path = argv[1];

    cv::Mat img = cv::imread(path);
    std::vector<cv::Mat> channels;

    split(img, channels);

    int rows = img.size().height;
    int cols = img.size().width;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            channels[0].at<uchar>(i, j) = MAX_PIX - channels[0].at<uchar>(i, j);
            channels[1].at<uchar>(i, j) = MAX_PIX - channels[1].at<uchar>(i, j);
            channels[2].at<uchar>(i, j) = MAX_PIX - channels[2].at<uchar>(i, j);

        }
    }
    
    cv::Mat out;

    cv::merge(channels, out);

    cv::imshow("Img Source", img);
    cv::imshow("Img Negative", out);

    cv::waitKey(0);
    return EXIT_SUCCESS;
}