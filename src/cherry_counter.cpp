#include <opencv2/opencv.hpp>
#include <stdlib.h> 

#define UMBRAL 235
#define DETECTED 255

#define VISITED 100
#define NOT_VISITED 0
#define IS_CHERRY 255

#define UMBRAL_AREA 70
int count;

typedef struct position {
    int x;
    int y;
} Position;


void expandir(int i, int j, cv::Mat &visited, std::vector<Position> &expanded, cv::Mat &out);

int main(int argc, char **argv) {
    const char *path = "../images/frutos_rojos.jpg";

    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
    int rows = img.size().height, cols = img.size().width;

    std::vector<cv::Mat> channels;
    split(img, channels);
    
    cv::Mat out = cv::Mat(rows, cols, CV_8U);
    // cv::imshow("Cherry Counter", img);
    
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            out.at<uchar>(i, j) = channels[2].at<uchar>(i, j) < UMBRAL ? 0: 255;
        }
    }

    cv::Mat visited = cv::Mat(rows, cols, CV_8U);
    std::vector<Position> expanded = {};

    // sacar el area
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            count = 0;
            uchar pix = out.at<uchar>(i, j);
            if (pix == DETECTED && visited.at<uchar>(i, j) != VISITED) {
                // visited.at<uchar>(i, j) = VISITED;
                expandir(i, j, visited, expanded, out);
                
                if (count > 0) printf("Count: %d\n", count);
                
                if (count < UMBRAL_AREA) {
                    // lo que acabamos de expandir no es cereza
                    // poner lo que acabamos de expandir en cero
                    for (Position pos : expanded) {
                        out.at<uchar>(pos.x, pos.y) = 0;
                    }
                }

                expanded.clear();
            }
        }
    }
    

    // sacar ahora solo el perimetro


    cv::imshow("Cherry Red Channel", channels[2]);
    cv::imshow("Cherries Segmented", out);
    cv::imshow("Visted Matrix", visited);

    cv::waitKey(0);
   
    return EXIT_SUCCESS;
}

void expandir(int i, int j, cv::Mat &visited, std::vector<Position> &expanded, cv::Mat &out) {
    if (visited.at<uchar>(i, j) == VISITED) {
        return;
    }
    visited.at<uchar>(i, j) = VISITED;

    if (out.at<uchar>(i, j) == DETECTED) {
        count++;
    }

    if (i != 0) {
        // consultar arriba
        if (visited.at<uchar>(i - 1, j) != VISITED) {
            Position pos;
            pos.x = i - 1;
            pos.y = j;
            expanded.push_back(pos);
            expandir(i - 1, j, visited, expanded, out);
        }

        if (j != 0) {
            // consultar diagonal arriba izq
            if (visited.at<uchar>(i - 1, j - 1) != VISITED) {
                Position pos;
                pos.x = i - 1;
                pos.y = j - 1;
                expanded.push_back(pos);
                expandir(i - 1, j - 1, visited, expanded, out);
            }
        }

        if (j != visited.size().width - 1) {
            // consultar diagonal arriba der
            if (visited.at<uchar>(i - 1, j + 1) != VISITED) {
                Position pos;
                pos.x = i - 1;
                pos.y = j + 1;
                expanded.push_back(pos);
                expandir(i - 1, j + 1, visited, expanded, out);
            }
        }
    }

    if (i != visited.size().height - 1) {
        // consultar abajo
        if (visited.at<uchar>(i + 1, j) != VISITED) {
            Position pos;
            pos.x = i + 1;
            pos.y = j;
            expanded.push_back(pos);
            expandir(i + 1, j, visited, expanded, out);
        }
        if (j != 0) {
            // consultar abajo izq
            if (visited.at<uchar>(i + 1, j - 1) != VISITED) {
                Position pos;
                pos.x = i + 1;
                pos.y = j - 1;
                expanded.push_back(pos);
                expandir(i + 1, j - 1, visited, expanded, out);
            }
        }

        if (j != visited.size().width - 1) { 
            // consultar abajo der
            if (visited.at<uchar>(i + 1, j + 1) != VISITED) {
                Position pos;
                pos.x = i + 1;
                pos.y = j + 1;
                expanded.push_back(pos);
                expandir(i + 1, j + 1, visited, expanded, out);
            }

        }
    }

    if (j != 0) {
        // consultar izq
        if (visited.at<uchar>(i, j - 1) != VISITED) {
            Position pos;
            pos.x = i;
            pos.y = j - 1;
            expanded.push_back(pos);
            expandir(i, j - 1, visited, expanded, out);
        }
    }

    if (j != visited.size().width - 1) {
        // consultar der
        if (visited.at<uchar>(i, j + 1) != VISITED) {
            Position pos;
            pos.x = i;
            pos.y = j + 1;
            expanded.push_back(pos);
            expandir(i, j + 1, visited, expanded, out);
        }
    }
}
