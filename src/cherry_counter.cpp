#include <opencv2/opencv.hpp>
#include <stdlib.h> 

#define UMBRAL 235
#define DETECTED 255
#define VISITED 255
#define UMBRAL_AREA 70
int count;

typedef struct position {
    int x;
    int y;
} Position;

void expandir(int i, int j, cv::Mat visited, std::vector<Position> expanded);

int main(int argc, char **argv) {
    const char *path = "../images/frutos_rojos.jpg";

    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
    int rows = img.size().height, cols = img.size().width;

    std::vector<cv::Mat> channels;
    split(img, channels);
    
    cv::Mat out = cv::Mat(rows, cols, CV_8U);
    cv::imshow("Cherry Counter", img);
    
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            out.at<uchar>(i, j) = channels[2].at<uchar>(i, j) < UMBRAL ? 0: 255;
        }
    }

    cv::Mat visited = cv::Mat(rows, cols, CV_8U);
    std::vector<Position> expaded = {};

    // sacar el area
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            visited.at<uchar>(i, j) = VISITED;
            count = 0;
            uchar pix = out.at<uchar>(i, j);
            if (pix == DETECTED) {

            }

            if (count < UMBRAL_AREA) {
                // lo que acabamos de expandir no es cereza
                // poner lo que acabamos de expandir en cero
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

void expandir(int i, int j, cv::Mat visited, std::vector<Position> expanded) {
    uchar pix = visited.at<uchar>(i, j);
    if (i != 0) {
        // consultar arriba

        if (j != 0) {
            // consulatar a la izquierda
            // aqui validar diagonal arriba izq
            if (pix != VISITED) {
                Position pos;
                pos.x = i;
                pos.y = j;
                expanded.push_back(pos);
                pix = VISITED;
            }
        }

        if (j != visited.size().width - 1) {
            // no consultar a la derecha
            // aqui validar diagonal arriba der
        }

        // validar arriba
    }

    if (i != visited.size().height - 1) {
        // consultar abajo
        
        if (j != 0) {
            // consultar abajo izq
        }

        if (j != visited.size().width - 1) { 
            // consultar abajo der
        }
    }
}
