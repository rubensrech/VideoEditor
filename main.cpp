#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
using namespace std;

#include "frameProcessing.h"
#include "window.h"
#include "windowWithTrackbar.h"

int main(int argc, char **argv) {
    cv::VideoCapture cap;

    // Open the default camera, use something different from 0 otherwise;
    int camera = 0;
    if (!cap.open(camera)) return 0;

    // > Build two windows for showing the original and the processed video stream
    Window originalWindow("Original video stream");
    WindowWithTrackbar processedWindow("Processed video stream", "Value: [0, 50]", 50);

    cv::Mat originalFrame;
    cv::Mat processedFrame;

    bool capturing = true;
    int opFlags = Operation::Color;

    while (capturing) {
        // Read the input video stream
        cap >> originalFrame;

        // Stop execution if input video stream is empty
        if (originalFrame.empty()) break;

        // Show original input video stream
        originalWindow.showFrame(originalFrame);

        // Set operation flags according to the key pressed
        Operation op = operationFromKey(cv::waitKey(1));
        if (op == Operation::Exit) break;
        setOperationFlags(opFlags, op);

        // Process input stream
        int arg = processedWindow.getTrackbarValue();
        processFrame(originalFrame, processedFrame, opFlags, arg);

        // Show processed video stream
        processedWindow.showFrame(processedFrame);
    }

    // Release the VideoCapture object
    cap.release();

    return 0;
}