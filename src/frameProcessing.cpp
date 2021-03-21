#include "frameProcessing.h"

#include <iostream>
#include <algorithm>
using namespace std;

#include <unistd.h>

#define ENSURE_ODD(v) { v = (v % 2 == 0) ? v+1 : v; };

int exclusiveOpsMask =  Operation::Color        |
                        Operation::Blur         |
                        Operation::Contrast     |
                        Operation::Brightness   |
                        Operation::Edges        |
                        Operation::Gradient     |
                        Operation::Grayscale    |
                        Operation::Negative;

int toggleOpsMask =     Operation::Rotate |
                        Operation::Resize |
                        Operation::Record |
                        Operation::FlipH  |
                        Operation::FlipV;

Operation operationFromKey(int opKey) {
    switch (opKey) {
    case 'c': case 'C': return Operation::Color;
    case 'b': case 'B': return Operation::Blur;
    case 'o': case 'O': return Operation::Contrast;
    case 'i': case 'I': return Operation::Brightness;
    case 'e': case 'E': return Operation::Edges;
    case 'd': case 'D': return Operation::Gradient;
    case 'g': case 'G': return Operation::Grayscale;
    case 'n': case 'N': return Operation::Negative;
    case 'z': case 'Z': return Operation::Resize;
    case 't': case 'T': return Operation::Rotate;
    case 'r': case 'R': return Operation::Record;
    case 'h': case 'H': return Operation::FlipH;
    case 'v': case 'V': return Operation::FlipV;
    case 27: return Operation::Exit;
    default: return Operation::Invalid;
    }
}

void setOperationFlags(int &opFlags, Operation newOp) {
    if (newOp < 0) return;

    // While recording, resize and rotate are not allowed
    if (opFlags & Operation::Record) {
        if (newOp == Operation::Rotate) return;
        if (newOp == Operation::Resize) return;
    }

    // Operation newOp is exclusive (should disable other exclusive ops)
    if (newOp & exclusiveOpsMask) {
        opFlags &= ~exclusiveOpsMask;
    }

    // Set newOp flag
    if (newOp & toggleOpsMask) {
        // Operation newOp is toggling
        opFlags ^= newOp;
    } else {
        opFlags |= newOp;
    }
}

void processFrame(Mat &input, Mat &output, int opFlags, int arg) {
    output = input;

    if (opFlags & Operation::Color) {}
    if (opFlags & Operation::Edges) output = cannyEdges(output);
    if (opFlags & Operation::Grayscale) output = toGrayscale(output);
    if (opFlags & Operation::Negative) output = toNegative(output);
    if (opFlags & Operation::Resize) output = resize(output, 0.5, 0.5);
    if (opFlags & Operation::Rotate) output = rotate(output, ROTATE_90_CLOCKWISE);
    if (opFlags & Operation::FlipH) output = flipH(output);
    if (opFlags & Operation::FlipV) output = flipV(output);

    if (opFlags & Operation::Gradient) {
        int kernelSize = arg;
        output = sobelGradient(output, kernelSize);
    }

    if (opFlags & Operation::Blur) {
        int kernelSize = arg;
        output = gaussianBlur(output, kernelSize);
    }

    if (opFlags & Operation::Contrast) {
        double alpha = arg / 5.0;
        output = adjustContrast(output, alpha);
    }

    if (opFlags & Operation::Brightness) {
        double beta = (arg - 25) * 2.0;
        output = adjustBrightness(output, beta);
    }

    if (opFlags & Operation::Record) {}
}


Mat gaussianBlur(Mat &input, int kernelSize) {
    ENSURE_ODD(kernelSize);

    Mat output;
    GaussianBlur(input, output, Size(kernelSize, kernelSize), 0, 0);
    return output;
}

Mat cannyEdges(Mat input, int kernelSize, double lowThresh, double highThresh) {
    ENSURE_ODD(kernelSize);
    kernelSize = max(min(kernelSize, 7), 3);

    Mat output;
    Canny(input, output, lowThresh, highThresh, kernelSize);
    return output;
}

Mat sobelGradient(Mat &input, int kernelSize) {
    ENSURE_ODD(kernelSize);
    Mat output;
    Mat gray, grad_x, grad_y, abs_grad_x, abs_grad_y;
    
    // Convert input frame to grayscale
    cvtColor(input, gray, COLOR_BGR2GRAY);

    // Calculate the directional gradients (X and Y) using the Sobel operator
    Sobel(gray, grad_x, CV_16S, 1, 0, kernelSize);
    Sobel(gray, grad_y, CV_16S, 0, 1, kernelSize);

    // Convert the gradient frames back to unsigned 8-bit format
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);

    // Combine the directional gradients
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, output);

    return output;
}

Mat adjustContrast(Mat &input, double alpha) {
    Mat output;
    input.convertTo(output, CV_8U, alpha);
    return output;
}

Mat adjustBrightness(Mat &input, double beta) {
    Mat output;
    input.convertTo(output, CV_8U, 1, beta);
    return output;
}

Mat toNegative(Mat &input) {
    Mat output;
    input.convertTo(output, CV_8U, -1, 255);
    return output;
}

Mat toGrayscale(Mat &input) {
    Mat output;
    cvtColor(input, output, COLOR_BGR2GRAY);
    return output;
}

Mat resize(Mat &input, double fx, double fy) {
    Mat output;
    cv::resize(input, output, Size(), fx, fy);
    return output;
}

Mat rotate(Mat &input, int rotationCode) {
    Mat output;
    cv::rotate(input, output, rotationCode);
    return output;
}

Mat flipH(Mat &input) {
    Mat output;
    cv::flip(input, output, 1);
    return output;
}

Mat flipV(Mat &input) {
    Mat output;
    cv::flip(input, output, 0);
    return output;
}