#include <opencv2/opencv.hpp>
#include <stdlib.h>

#define UMBRAL 60

int main(int argc, char **argv) {
    const char *path = "../images/rice.jpg";
    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);

    std::vector<cv::Mat> channels;
    split(img, channels);
    cv::Mat processed = channels[0];
    
    int rows = img.size().height, cols = img.size().width;
    cv::imshow("Before", processed);
    
    cv::Mat out = cv::Mat(rows, cols, CV_8U);

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
    cv::imshow("After processed", processed);
    cv::imshow("Rice segmented", out);

    cv::waitKey(0);
    return EXIT_SUCCESS;
}
