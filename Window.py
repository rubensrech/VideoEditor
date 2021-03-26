
import cv2 as cv

class Window:
    def __init__(self, name):
        self.name = name
        cv.namedWindow(name, cv.WINDOW_AUTOSIZE)
    
    def showFrame(self, frame):
        cv.imshow(self.name, frame)

    def setPosition(self, x, y):
        cv.moveWindow(self.name, x, y)

class WindowWithTrackbar(Window):
    def __init__(self, name, trackbarLabel, trackbarMax):
        Window.__init__(self, name)
        self.trackbarLabel = trackbarLabel
        self.trackbarListener = None
        cv.createTrackbar(trackbarLabel, name, 0, trackbarMax, lambda val: self.__onTrackbarChange(val))

    def __onTrackbarChange(self, value):
        if self.trackbarListener is not None:
            self.trackbarListener(value)

    def onTrackbarChange(self, handlerFunc):
        self.trackbarListener = handlerFunc

    def setTrackbarValue(self, value):
        cv.setTrackbarPos(self.trackbarLabel, self.name, value)
    
    def getTrackbarValue(self):
        return cv.getTrackbarPos(self.trackbarLabel, self.name)