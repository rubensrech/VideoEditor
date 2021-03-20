#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
#include <string>
using namespace std;

#include "ui.h"

void onTrackbarChange(int value, void *optinalData) {}

int main(int argc, char **argv) {
    VideoCapture cap;

    // Open the default camera, use something different from 0 otherwise;
    int camera = 0;
    if (!cap.open(camera)) return 0;

    // > Prepare the UI elements
    // Create a new window for showing the original video stream
    string origWindowName = "Original video stream";
    namedWindow(origWindowName, WINDOW_AUTOSIZE);

    // Create a new window with a Trackbar for showing the processed video stream
    string procWindowName = "Processed video stream";
    namedWindow(procWindowName, WINDOW_AUTOSIZE);
    int trackbarValue = 0;
    const int trackbarMax = 50;
    string trackbarName = "Value: [0, 50]";
    createTrackbar(trackbarName, procWindowName, &trackbarValue, trackbarMax, onTrackbarChange);

    Mat originalFrame;
    Mat processedFrame;

    for (;;) {
        // Read the input video stream
        cap >> originalFrame;

        // Stop execution if input video stream is empty
        if (originalFrame.empty()) break;

        // Process input video stream
        
        // - Gaussian blur
        int kernelSize = trackbarValue % 2 == 1 ? trackbarValue : trackbarValue + 1;
        GaussianBlur(originalFrame, processedFrame, Size(kernelSize, kernelSize), 0, 0);

        // - Canny edge detector
        // int ratio = 2;
        // int lowTreshold = 50;
        // int highTreshold = lowTreshold * ratio;
        // int kernelSize = 3;
        // Canny(originalFrame, processedFrame, lowTreshold, highTreshold, kernelSize);

        // - Sobel gradient
        // Mat gray;
        // Mat grad_x, grad_y;
        // Mat abs_grad_x, abs_grad_y;
        // cvtColor(originalFrame, gray, COLOR_BGR2GRAY);
        // Sobel(gray, grad_x, CV_16S, 1, 0, 3);
        // Sobel(gray, grad_y, CV_16S, 0, 1, 3);
        // convertScaleAbs(grad_x, abs_grad_x);
        // convertScaleAbs(grad_y, abs_grad_y);
        // addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, processedFrame);

        // - ConvertTo: constrast, brightness and negative
        // # Constrast
        // double constrast = trackbarValue / 5.0;
        // originalFrame.convertTo(processedFrame, CV_8U, constrast);
        // # Brightness
        // double brightness = (trackbarValue - 25) * 2.0;
        // originalFrame.convertTo(processedFrame, CV_8U, 1, brightness);
        // # Negative
        // originalFrame.convertTo(processedFrame, CV_8U, -1, 255);

        // Show original input video stream
        imshow(origWindowName, originalFrame);

        // Show processed video stream
        imshow(procWindowName, processedFrame);

        // Stop capturing by pressing ESC
        if (waitKey(1) == 27) break;
    }

    // Release the VideoCapture object
    cap.release();

    return 0;
}