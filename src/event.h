#ifndef EVENT_H
#define EVENT_H

enum MouseButton {
    LEFT,
    MIDDLE,
    RIGHT
};

enum MouseAction {
    PRESS,
    RELEASE
};

class MouseEvent {
public:
    MouseButton button;
    MouseAction action;
};

#endif