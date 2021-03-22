from enum import Enum, IntFlag
import numpy as np
import cv2 as cv

class Operation(IntFlag):
    Color       = 1 << 0,  #            (c)
    Blur        = 1 << 1,  # Gaussian   (b)
    Contrast    = 1 << 2,  #            (o)
    Gradient    = 1 << 3,  # Sobel      (d)
    Edges       = 1 << 4,  # Canny      (e)
    Grayscale   = 1 << 5,  #            (g)
    Negative    = 1 << 6,  #            (n)
    Brightness  = 1 << 7,  #            (i)
    Resize      = 1 << 8,  #            (z)
    Rotate      = 1 << 9,  #            (t)
    Record      = 1 << 10, #            (r)
    FlipH       = 1 << 11, #            (h)
    FlipV       = 1 << 12, #            (v)
    Exit        = -1       #            (ESC: 27)
    Invalid     = -2       #

    @staticmethod
    def fromKey(opKey):
        if opKey == ord('c'):   return Operation.Color
        elif opKey == ord('b'): return Operation.Blur
        elif opKey == ord('o'): return Operation.Contrast
        elif opKey == ord('d'): return Operation.Gradient
        elif opKey == ord('e'): return Operation.Edges
        elif opKey == ord('g'): return Operation.Grayscale
        elif opKey == ord('n'): return Operation.Negative
        elif opKey == ord('i'): return Operation.Brightness
        elif opKey == ord('z'): return Operation.Resize
        elif opKey == ord('t'): return Operation.Rotate
        elif opKey == ord('r'): return Operation.Record
        elif opKey == ord('h'): return Operation.FlipH
        elif opKey == ord('v'): return Operation.FlipV
        elif opKey == 27: return Operation.Exit
        else: return Operation.Invalid

def printOperationsHelp():
    print("c   : Color (no processing)")
    print("b   : Blurring (Gaussian blur)")
    print("o   : cOntrast enhancement")
    print("d   : graDient (Sobel)")
    print("e   : Edges (Canny)")
    print("g   : Grayscale")
    print("n   : Negative")
    print("i   : brIghtness enhancement")
    print("z   : toggle resiZe (1/4 of the original size)")
    print("t   : roTate")
    print("r   : toggle Recording")
    print("h   : toggle flip Horizontally")
    print("v   : toggle flip Vertically")
    print("ESC : Exit")

exclusiveOpsMask =  Operation.Color        | \
                    Operation.Blur         | \
                    Operation.Contrast     | \
                    Operation.Brightness   | \
                    Operation.Edges        | \
                    Operation.Gradient     | \
                    Operation.Grayscale    | \
                    Operation.Negative

toggleOpsMask = Operation.Rotate | \
                Operation.Resize | \
                Operation.Record | \
                Operation.FlipH  | \
                Operation.FlipV

def setOperationFlags(opFlags, newOp):
    recordingToggled = False
    isRecording = opFlags & Operation.Record

    if newOp < 0: return opFlags, recordingToggled, isRecording

    # Detect recording toggled
    if newOp == Operation.Record:
        recordingToggled = True
        if isRecording:
            # Stop recording
            opFlags ^= Operation.Record
        else:
            # Start recording
            opFlags |= Operation.Record
        isRecording = not isRecording
        return opFlags, recordingToggled, isRecording

    # While recording, resize and rotate are not allowed
    if isRecording:        
        if newOp == Operation.Rotate: return opFlags, recordingToggled, isRecording
        if newOp == Operation.Resize: return opFlags, recordingToggled, isRecording

    # Operation newOp is exclusive (should disable other exclusive ops)
    if newOp & exclusiveOpsMask:
        opFlags &= ~exclusiveOpsMask

    # Set newOp flag
    if newOp & toggleOpsMask:
        # Operation newOp is toggling
        opFlags ^= newOp
    else:
        opFlags |= newOp

    return opFlags, recordingToggled, isRecording

def processFrame(input, opFlags, arg):
    output = input

    if opFlags & Operation.Color:       pass
    if opFlags & Operation.Edges:       output = cannyEdges(output)
    if opFlags & Operation.Grayscale:   output = toGrayscale(output)
    if opFlags & Operation.Negative:    output = toNegative(output)
    if opFlags & Operation.Resize:      output = resize(output, 0.5, 0.5)
    if opFlags & Operation.Rotate:      output = rotateClockwise(output)
    if opFlags & Operation.FlipH:       output = flipH(output)
    if opFlags & Operation.FlipV:       output = flipV(output)

    if opFlags & Operation.Gradient:
        kernelSize = arg
        output = sobelGradient(output, kernelSize)

    if opFlags & Operation.Blur:
        kernelSize = arg
        output = gaussianBlur(output, kernelSize)

    if opFlags & Operation.Contrast:
        alpha = arg / 5.0
        output = adjustContrast(output, alpha)

    if opFlags & Operation.Brightness:
        beta = (arg - 25) * 2.0
        output = adjustBrightness(output, beta)

    return output

def ENSURE_ODD(num):
    return num+1 if num % 2 == 0 else num

def CLAMP(n, minn, maxn):
    return max(min(maxn, n), minn)


def gaussianBlur(input, kernelSize = 3):
    kernelSize = ENSURE_ODD(kernelSize)
    return cv.GaussianBlur(input, (kernelSize, kernelSize), 0)

def cannyEdges(input, kernelSize = 3, lowThresh = 50, highThresh = 100):
    kernelSize = CLAMP(ENSURE_ODD(kernelSize), 1, 7)
    return cv.cvtColor(cv.Canny(input, lowThresh, highThresh, kernelSize), cv.COLOR_GRAY2RGB)

def sobelGradient(input, kernelSize = 3):
    kernelSize = CLAMP(ENSURE_ODD(kernelSize), 1, 7)
    
    # Convert input frame to grayscale
    gray = cv.cvtColor(input, cv.COLOR_BGR2GRAY)

    # Calculate the directional gradients (X and Y) using the Sobel operator
    grad_x = cv.Sobel(gray, cv.CV_16S, 1, 0, kernelSize)
    grad_y = cv.Sobel(gray, cv.CV_16S, 0, 1, kernelSize)

    # Convert the gradient frames back to unsigned 8-bit format
    abs_grad_x = cv.convertScaleAbs(grad_x)
    abs_grad_y = cv.convertScaleAbs(grad_y)

    # Combine the directional gradients
    return cv.cvtColor(cv.addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0), cv.COLOR_GRAY2RGB)

def adjustContrast(input, alpha):
    return cv.convertScaleAbs(input, alpha=alpha)

def adjustBrightness(input, beta):
    return cv.convertScaleAbs(input, beta=beta)

def toNegative(input):
    return cv.convertScaleAbs(input, alpha=-1, beta=255)

def toGrayscale(input):
    return cv.cvtColor(cv.cvtColor(input, cv.COLOR_BGR2GRAY), cv.COLOR_GRAY2RGB)

def resize(input, sx, sy):
    return cv.resize(input, (0,0), fx=sx, fy=sy)

def rotateClockwise(input):
    return cv.rotate(input, cv.ROTATE_90_CLOCKWISE)

def rotateCounterClockwise(input):
    return cv.rotate(input, cv.ROTATE_90_COUNTERCLOCKWISE)

def flipH(input):
    return cv.flip(input, 1)

def flipV(input):
    return cv.flip(input, 0)