#include <opencv2/opencv.hpp>
#include <stdlib.h>

#define UMBRAL 120

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    const char *path = "../images/single_rice.jpg";
    Mat img = imread(path, IMREAD_COLOR);
    int rows = img.size().height, cols = img.size().width;

    vector<Mat> channels;
    split(img, channels);
    
    Mat out = Mat(rows, cols, CV_8U);
    
    imshow("Rice", img);

    for (size_t i = 0; i < img.size().height; i++) {
        for (size_t j = 0; j < img.size().width; j++) {
            int pix_channel_0 = channels[0].at<uchar>(i, j);
            int pix_channel_1 = channels[1].at<uchar>(i, j);
            int pix_channel_2 = channels[2].at<uchar>(i, j);

            int sum =  pix_channel_0 + pix_channel_1 + pix_channel_2;
            printf("[%d %d %d %d]", pix_channel_0, pix_channel_1, pix_channel_2, sum);
            out.at<uchar>(i, j) = (sum / 3) < UMBRAL ? 0: 255;
        }
        printf("\n");
    }
    
    imshow("Rice segmented", out);

    waitKey(0);
    return EXIT_SUCCESS;
}
