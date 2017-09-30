#ifndef EVENT_H
#define EVENT_H

enum MouseButton {
    LEFT,
    MIDDLE,
    RIGHT
};

enum Action {
    PRESS,
    RELEASE
};

class MouseEvent {
public:
    MouseButton button;
    Action action;
    
    bool shift_down;
    bool alt_down;
    bool control_down;
};

class KeyEvent {
public:
    char key;
    Action action;
    
    bool shift_down;
    bool alt_down;
    bool control_down;
};

#endif