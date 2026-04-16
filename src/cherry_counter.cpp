#include <opencv2/opencv.hpp> 
#include <stdlib.h> 

#define UMBRAL 200
#define DETECTED 255

#define VISITED 100
#define NOT_VISITED 0
#define IS_CHERRY 255

#define UMBRAL_AREA 700
int count = 0;
int cherry_counter = 0;

typedef struct position {
    int x;
    int y;
} Position;

cv::Mat visited;
std::vector<Position> expanded;
cv::Mat out;

void expand(int i, int j);
bool is_borde(int i, int j);
bool is_cherry(int i, int j);

int main(int argc, char **argv) {
    const char *path = "./images/frutos_rojos.jpg";

    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
    int rows = img.size().height, cols = img.size().width;

    std::vector<cv::Mat> channels;
    split(img, channels);
    
    out = cv::Mat(rows, cols, CV_8U);
    // cv::imshow("Cherry Counter", img);
    
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            out.at<uchar>(i, j) = channels[2].at<uchar>(i, j) < UMBRAL ? 0: 255;
        }
    }

    visited = cv::Mat(rows, cols, CV_8U);
    expanded = std::vector<Position>();

    // sacar el area
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            uchar pix = out.at<uchar>(i, j);
            
            if (pix == DETECTED && visited.at<uchar>(i, j) != VISITED) {
                
                count = 0;
                expanded.clear();
                expand(i, j);
                
                if (count < UMBRAL_AREA) {
                    // lo que acabamos de expandir no es cereza
                    // poner lo que acabamos de expandir en cero
                    for (Position pos : expanded) {
                        out.at<uchar>(pos.x, pos.y) = 0;
                    }
                } else {
                    cherry_counter ++;
                    for (Position pos : expanded) {
                        out.at<uchar>(pos.x, pos.y) = IS_CHERRY;
                    }
                }
            }
        }
    }
    printf("Cherries: %d\n", cherry_counter);
    
    
    cv::Mat perimeter = cv::Mat::zeros(img.size(), CV_8U);

    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            if (is_cherry(i, j) && is_borde(i, j)) {
                perimeter.at<uchar>(i, j) = 255;
            }
        }
    }

    cv::Mat merged = cv::Mat::zeros(out.size(), CV_8U);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (perimeter.at<uchar>(i, j) == 255) {
                channels[0].at<uchar>(i, j) = 0;                
                channels[1].at<uchar>(i, j) = 255;                
            }
        }
    }

    cv::merge(channels, merged);

    cv::imshow("1 Image Source", img);
    cv::imshow("2 Cherry Red Channel", channels[2]);
    cv::imshow("3 Visted Matrix", visited);
    cv::imshow("4 Cherries Segmented", out);
    cv::imshow("5 Perimeter Matrix", perimeter);
    cv::imshow("6 Superpuestas Matrix", merged);
    cv::waitKey(0);
   
    return EXIT_SUCCESS;
}

bool is_cherry(int i, int j) {
    return out.at<uchar>(i, j) == IS_CHERRY;
}

bool is_borde(int i, int j) {
    bool isBorde = 0;

    for (int di = -1; di < 2; di++) {
        for (int dj = -1; dj < 2; dj++) {
            if (di == 0 && dj == 0) continue;
            if (out.at<uchar>(i + di, j + dj) == 0) {
                isBorde = 1;
            }
        }
    }
    return isBorde;
}

void expand(int i, int j) {
    if (visited.at<uchar>(i, j) == VISITED) return;
    
    visited.at<uchar>(i, j) = VISITED;
    
    if (out.at<uchar>(i, j) != DETECTED) return;
    
    expanded.push_back({i, j});
    count++;

    int rows = visited.size().height;
    int cols = visited.size().width;
    
    for (int di = -1; di < 2; di++) {
        for (int dj = -1; dj < 2; dj++) {
            if (di == 0 && dj == 0) continue;
            
            int ni = i + di;
            int nj = j + dj;
            
            if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                expand(ni, nj);
            }
        }
    }
}
