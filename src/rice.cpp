#include <opencv2/opencv.hpp>
#include <stdlib.h>

#define UMBRAL 60

cv::Mat mask(cv::Mat& in);

int main(int argc, char **argv) {
    if (argc < 2) {
        perror("Must provide a path\n");
        return EXIT_FAILURE;
    }
    char *path = argv[1];
    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);

    std::vector<cv::Mat> channels;
    split(img, channels);
    cv::Mat blue_channel = channels[0];
    
    int rows = img.size().height, cols = img.size().width;
    cv::imshow("Before", blue_channel);
    
    cv::Mat out = mask(blue_channel);

    cv::imshow("After processed", blue_channel);
    cv::imshow("Rice segmented", out);

    cv::waitKey(0);
    return EXIT_SUCCESS;
}


cv::Mat mask(cv::Mat& in) {
    int rows = in.size().height;
    int cols = in.size().width;

    cv::Mat out = cv::Mat(rows, cols, CV_8U);

    for (size_t i = 0; i < rows; i++) {
        int min = 255;
        for (size_t j = 0; j < cols; j++) {
            if (min > in.at<uchar>(i, j)) {
                min = in.at<uchar>(i, j);
            }
        }

        for (size_t j = 0; j < cols; j++) {
            in.at<uchar>(i, j) = in.at<uchar>(i, j) - min;
            out.at<uchar>(i, j) = in.at<uchar>(i, j) < UMBRAL? 0: 255;
        }
    }
    
    return out;
}