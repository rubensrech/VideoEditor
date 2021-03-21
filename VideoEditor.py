import numpy as np
import cv2 as cv

from Window import Window, WindowWithTrackbar

def onTrackbarChange(val):
    print(val)

# Build two windows for showing the original and the processed video stream
originalWindow = Window("Original video stream")
processedWindow = WindowWithTrackbar("Processed video stream", "Value:", 50)

# Open the default camera, use something different from 0 otherwise
camera = 0
cap = cv.VideoCapture(camera)

while cap.isOpened():
    # Read the input video stream
    ret, originalFrame = cap.read()

    # # Stop execution if input video stream is empty
    # if originalFrame.empty(): break

    # Show original input video stream
    originalWindow.showFrame(originalFrame)

    # Set operation flags according to the key pressed
    opKey = cv.waitKey(1)
    if opKey == 27: break

    # Process input stream
    processedFrame = originalFrame

    # Show processed video stream
    processedWindow.showFrame(processedFrame)

# When everything done, release the capture
cap.release()
cv.destroyAllWindows()

exit(0)