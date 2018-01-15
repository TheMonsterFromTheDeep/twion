#ifndef OBJECT_EDITOR_H
#define OBJECT_EDITOR_H

#include <vector>
#include "shape.h"

class ObjectEditor : public Editor {
public:
    std::vector<shape::Shape*> *source;
    
    ObjectEditor(std::vector<shape::Shape*>*);
    
    virtual void draw(Graphics);
    virtual void key(KeyEvent,Vec);
    virtual void mouse_move(Vec,Vec);
    virtual void mouse(MouseEvent,Vec);
private:
    enum EditState {
        NONE,
        GRAB,
        ROTATE,
        SCALE
    };
    
    void confirm();
    void cancel();
    
    EditState state;

    void add_circle();
    
    Vec action_center;
    
    Vec original_pos;
};

#endif