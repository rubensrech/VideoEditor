#ifndef WINDOW_WITH_TRACKBAR_H
#define WINDOW_WITH_TRACKBAR_H

#include "window.h"

class WindowWithTrackbar : public Window {

private:
    string trackbarLabel;
    int trackbarValue = 0;
    void (*listener)(int val) = nullptr;

    static void onChange(int value, void *selfPtr) {
        WindowWithTrackbar *self = (WindowWithTrackbar *)selfPtr;
        if (self->listener) {
            self->listener(value);
        }
    }

public:
    WindowWithTrackbar(string name, string trackbarLabel, int trackbarMax) : Window(name) {
        this->trackbarLabel = trackbarLabel;

        createTrackbar(this->trackbarLabel, this->name, &trackbarValue, trackbarMax, &onChange, this);
    }

    int getTrackbarValue() {
        return trackbarValue;
    }

    void setTrackbarValue(int value) {
        setTrackbarPos(trackbarLabel, name, value);
    }

    void onTrackbarChange(void (*listener)(int)) {
        this->listener = listener;
    }


};

#endif