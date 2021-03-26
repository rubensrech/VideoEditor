#ifndef WINDOW_H
#define WINDOW_H

#include <opencv2/opencv.hpp>
using namespace cv;

#include <string>
using namespace std;

class Window {

protected:
    string name;

public:
    Window(string name) {
        this->name = name;

        namedWindow(this->name, WINDOW_AUTOSIZE);
    }

    void showFrame(Mat frame) {
        imshow(this->name, frame);
    }

};

#endif