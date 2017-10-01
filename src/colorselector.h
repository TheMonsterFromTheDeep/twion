#ifndef COLOR_SELECTOR_H
#define COLOR_SELECTOR_H

#include "control.h"

class ColorSelector : public Control {
    public:
	ColorSelector();

	virtual void mouse_move(Vec,Vec);
	virtual void scroll(Vec);
    virtual void mouse_button(MouseEvent);
    virtual void key(KeyEvent);
protected:
	virtual void draw(Graphics);
private:
    Image hsv_wheel;
};

#endif