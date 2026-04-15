#include <opencv2/opencv.hpp>
#include <stdlib.h>

#define UMBRAL 120

int main(int argc, char **argv) {
    const char *path = "../images/single_rice.jpg";

    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
    int rows = img.size().height, cols = img.size().width;

    std::vector<cv::Mat> channels;
    split(img, channels);
    
    cv::Mat out = cv::Mat(rows, cols, CV_8U);
    
    cv::imshow("Rice", img);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            int pix_channel_0 = channels[0].at<uchar>(i, j);
            int pix_channel_1 = channels[1].at<uchar>(i, j);
            int pix_channel_2 = channels[2].at<uchar>(i, j);

            int sum =  pix_channel_0 + pix_channel_1 + pix_channel_2;
            out.at<uchar>(i, j) = (sum / 3) < UMBRAL ? 0: 255;
        }
    }
    
    cv::imshow("Rice segmented", out);

    cv::waitKey(0);
    return EXIT_SUCCESS;
}
