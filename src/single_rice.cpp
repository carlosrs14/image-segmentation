#include <opencv2/opencv.hpp>
#include <stdlib.h>

#define UMBRAL 120

cv::Mat mask(const cv::Mat& in);

int main(int argc, char **argv) {
    if (argc < 2) {
        perror("Must provide a path\n");
        return EXIT_FAILURE;
    }

    char *path = argv[1];
    
    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
    int rows = img.size().height, cols = img.size().width;

    std::vector<cv::Mat> channels;
    split(img, channels);    
    
    cv::Mat out = mask(channels[0]);
    
    cv::imshow("Rice", img);
    cv::imshow("Rice segmented", out);

    cv::waitKey(0);
    return EXIT_SUCCESS;
}

cv::Mat mask(const cv::Mat &in) {
    int rows = in.size().height;
    int cols = in.size().width;

    cv::Mat out = cv::Mat(rows, cols, CV_8U);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            uchar pix = in.at<uchar>(i, j);
            out.at<uchar>(i, j) = pix < UMBRAL ? 0: 255;
        }
    }

    return out;
}
