#include "objecteditor.h"
#include <iostream>

ObjectEditor::ObjectEditor(std::vector<Shape> *source_) : source(source_) { }

void ObjectEditor::draw(Graphics g) {
    for(Shape& c : *source) {
        c.draw(g);
    } 
}

#define ADD_CIRCLE_POINT(x, y) s->add(CurvePoint(x, y, thickness, color), Vec(x - y * 0.5, y + x * 0.5), Vec(x + y * 0.5, y - x * 0.5))

void ObjectEditor::add_circle() {
    float radius = 100;
    float thickness = 1;
    RGB color(0.3f, 1.f, 0.3f);
    
    source->push_back(Shape());
    Shape *s = &(*source)[source->size() - 1];
    ADD_CIRCLE_POINT(-radius, 0);
    ADD_CIRCLE_POINT(0, radius);
    ADD_CIRCLE_POINT(radius, 0);
    ADD_CIRCLE_POINT(0, -radius);
    
    if(source->size() == 1) original_pos = Vec();
}

void ObjectEditor::key(KeyEvent e, Vec mouse) {
    if(e.key == 'A' && e.shift_down) {
        add_circle();
    }
    
    if(e.key == 258) {
        change_editor(source->at(0).get_editor());
    }
    
    if(e.key == 'G') {
        state = GRAB;
        
        action_center = mouse;
    }
}
void ObjectEditor::mouse_move(Vec position,Vec d) {
    switch(state) {
        case GRAB: {
            Vec translation = position - action_center;
            source->at(0).position = original_pos + translation;
        }
        break;
    }
}

void ObjectEditor::cancel() {
    source->at(0).position = original_pos;
    
    state = NONE;
}

void ObjectEditor::confirm() {
    state = NONE;
    original_pos = source->at(0).position;
}

void ObjectEditor::mouse(MouseEvent e,Vec v) {
    if(e.button == RIGHT && e.action == PRESS) {
        cancel();
    }
    
    if(e.button == LEFT && e.action == PRESS) {
        if(state != NONE) confirm();
    }
}