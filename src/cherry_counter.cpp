#include <opencv2/opencv.hpp> 
#include <stdlib.h> 

#define UMBRAL 220
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
    
    
    // FALTA: sacar ahora solo el perimetro

    cv::imshow("Cherry Red Channel", channels[2]);
    cv::imshow("Cherries Segmented", out);
    cv::imshow("Visted Matrix", visited);

    cv::waitKey(0);
   
    return EXIT_SUCCESS;
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
