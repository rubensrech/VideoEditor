#!/opt/anaconda3/bin/python3

import numpy as np
import cv2 as cv

from Window import Window, WindowWithTrackbar
from FrameProcessing import Operation, setOperationFlags, processFrame, printOperationsHelp

# Build two windows for showing the original and the processed video stream
originalWindow = Window("Original video stream")
processedWindow = WindowWithTrackbar("Processed video stream", "Value:", 50)
originalWindow.setPosition(0,0)
processedWindow.setPosition(100,0)

# Open the default camera, use something different from 0 otherwise
camera = 0
cap = cv.VideoCapture(camera)

opFlags = Operation.Color
printOperationsHelp()

while cap.isOpened():
    # Read the input video stream
    ret, originalFrame = cap.read()

    # # Stop execution if input video stream is empty
    # if originalFrame.empty(): break

    # Show original input video stream
    originalWindow.showFrame(originalFrame)

    # Set operation flags according to the key pressed
    opKey = cv.waitKey(1)
    op = Operation.fromKey(opKey)
    if op == Operation.Exit: break;
    opFlags = setOperationFlags(opFlags, op)

    # Process input stream
    arg = processedWindow.getTrackbarValue()
    processedFrame = processFrame(originalFrame, opFlags, arg)

    # Show processed video stream
    processedWindow.showFrame(processedFrame)

# When everything done, release the capture
cap.release()
cv.destroyAllWindows()

exit(0)