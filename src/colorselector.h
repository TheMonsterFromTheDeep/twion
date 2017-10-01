#ifndef COLOR_SELECTOR_H
#define COLOR_SELECTOR_H

#include "control.h"

class ColorSelector : public Control {
    public:
	ColorSelector(RGB*);

	virtual void mouse_move(Vec,Vec);
    virtual void mouse_button(MouseEvent);
    virtual void key(KeyEvent);
protected:
	virtual void draw(Graphics);
private:
    Image hsv_wheel;
    
    bool editing_color;
    
    Vec color_position;
    
    RGB *target;
};

#endif