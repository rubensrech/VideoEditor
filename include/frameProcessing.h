#ifndef FRAME_PROCESSING_H
#define FRAME_PROCESSING_H

#include <opencv2/opencv.hpp>
using namespace cv;

enum Operation {
    Color       = 1 << 0,  //            (c)
    Blur        = 1 << 1,  // Gaussian   (b)
    Contrast    = 1 << 2,  //            (o)
    Gradient    = 1 << 3,  // Sobel      (d)
    Edges       = 1 << 4,  // Canny      (e)
    Grayscale   = 1 << 5,  //            (g)
    Negative    = 1 << 6,  //            (n)
    Brightness  = 1 << 7,  //            (i)
    Resize      = 1 << 8,  //            (z)
    Rotate      = 1 << 9,  //            (t)
    Record      = 1 << 10, //            (r)
    FlipH       = 1 << 11, //            (h)
    FlipV       = 1 << 12, //            (v)

    Exit        = -1,
    Invalid     = -2
};

Operation operationFromKey(int opKey);
void setOperationFlags(int &opFlags, Operation newOp);
void processFrame(Mat &input, Mat &output, int opFlags, int arg);

Mat gaussianBlur(Mat &input, int kernelSize = 3);
Mat cannyEdges(Mat input, int kernelSize = 3, double lowThresh = 50, double highThresh = 100);
Mat sobelGradient(Mat &input, int kernelSize = 3);
Mat adjustContrast(Mat &input, double alpha);
Mat adjustBrightness(Mat &input, double beta);
Mat toNegative(Mat &input);
Mat toGrayscale(Mat &input);
Mat resize(Mat &input, double fx, double fy);
Mat rotate(Mat &input, int rotationCode);
Mat flipH(Mat &input);
Mat flipV(Mat &input);

#endif