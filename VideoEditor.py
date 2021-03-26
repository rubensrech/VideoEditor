#!/opt/anaconda3/bin/python3

import numpy as np
import cv2 as cv
import time

from Window import Window, WindowWithTrackbar
from FrameProcessing import Operation, setOperationFlags, processFrame, printOperationsHelp

# Video recording params
codec = cv.VideoWriter_fourcc(*'X264')
outFilename = "output.mp4"

def main():
    # Open the default camera, use something different from 0 otherwise
    camera = 0
    cap = cv.VideoCapture(camera)

    # Build two windows for showing the original and the processed video stream
    originalWindow = Window("Original video stream")
    processedWindow = WindowWithTrackbar("Processed video stream", "Value:", 50)
    originalWindow.setPosition(0,0)
    processedWindow.setPosition(100,0)

    # Operations
    opFlags = Operation.Color
    printOperationsHelp()

    # Video recording
    videoWriter = None

    # FPS calculation
    fps = 15.0
    t0 = time.time()
    frames = 0

    while cap.isOpened():
        # Read the input video stream
        validStream, originalFrame = cap.read()

        # Stop execution if input video stream is empty
        if not validStream: break

        # Show original input video stream
        originalWindow.showFrame(originalFrame)

        # Set operation flags according to the key pressed
        opKey = cv.waitKey(1)
        op = Operation.fromKey(opKey)
        if op == Operation.Exit: break
        opFlags, recordingToggled, isRecording = setOperationFlags(opFlags, op)

        # Process input stream
        arg = processedWindow.getTrackbarValue()
        processedFrame = processFrame(originalFrame, opFlags, arg)

        # Show processed video stream
        processedWindow.showFrame(processedFrame)

        # Recording management
        if isRecording:
            if videoWriter is None:
                # Start recording
                (h, w) = processedFrame.shape[:2]
                print(f"Started video recording (FPS = {fps})")
                videoWriter = cv.VideoWriter(outFilename, codec, fps, (w, h))
            
            # Write frame to output video
            videoWriter.write(processedFrame)
        
        if recordingToggled and not isRecording:
            # Stop recording
            if videoWriter is not None:
                print("Stopped video recording")
                videoWriter.release()
                videoWriter = None

        # Calculate average FPS
        frames += 1
        if frames == 20:
            currFps = frames / (time.time() - t0)
            fps = 0.6 * fps + 0.4 * currFps
            t0 = time.time()
            frames = 0

    # When everything done, release the capture
    cap.release()
    cv.destroyAllWindows()
    if videoWriter is not None: videoWriter.release()

    exit(0)

if __name__ == "__main__":
    main()