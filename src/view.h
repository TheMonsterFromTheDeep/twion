#ifndef VIEW_H
#define VIEW_H

#include "control.h"

class View : public Control, public Sizer {
public:
    View(int,int,int,int,RGB);

    virtual bool mouse_move(Vec, Vec);
    virtual bool mouse_button(MouseEvent);
    virtual void draw(Graphics);
    
    virtual void size(Control*,int,int);
private:
    bool active;
    float amount;
};

#endif