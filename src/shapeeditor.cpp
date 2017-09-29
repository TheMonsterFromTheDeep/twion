#include "shape.h"

#include <iostream>

EditCurvePoint::EditCurvePoint(CurvePoint *source_) : source(source_), selected(false) { }
EditVec::EditVec(Vec *source_) : source(source_), selected(false) { }

ShapeEditor::ShapeEditor(Shape* source_) : source(source_), state(NONE) {    
    for(int i = 0; i < source->points.size(); ++i) {
        curvepoints.push_back(EditCurvePoint(&(source->points[i])));
    }
    
    for(int i = 0; i < source->handles.size(); ++i) {
        vecs.push_back(EditVec(&(source->handles[i])));
    }
}

void ShapeEditor::draw(Graphics g) {
    source->draw(g);
    
    RGB unselect(0.f, 0.f, 1.f);
    RGB select(0.f, 1.f, 1.f);
    
    //std::cout << "number of curve points: " << curvepoints.size() << std::endl;
    
    for(int i = 0; i < curvepoints.size(); ++i) {
        g.rgb(curvepoints[i].selected ? select : unselect);
        g.draw_circle(curvepoints[i].source->location, 5);
    }
    
    for(int i = 0; i < vecs.size(); ++i) {
        g.rgb(vecs[i].selected ? select : unselect);
        g.draw_circle(*(vecs[i].source), 5);
    }
}

void ShapeEditor::key(char c, Vec mouse) {
    if(c == 'G') {
        state = GRAB;
        action_center = mouse;
        action_delta = Vec();
    }
}

void ShapeEditor::mouse_move(Vec position, Vec delta) {
    action_delta += delta;
    
    switch(state) {
        case GRAB: {
            for(int i = 0; i < curvepoints.size(); ++i) {
                if(curvepoints[i].selected) {
                    curvepoints[i].source->location += delta;
                }
            }
            for(int i = 0; i < vecs.size(); ++i) {
                if(vecs[i].selected) {
                    *vecs[i].source += delta;
                }
            }
        }
        break;
    }
}

static bool within_radius(Vec a, Vec b) {
    return (b - a).lensqr() < 5 * 5;
}

void ShapeEditor::select(Vec pos) {
    for(int i = 0; i < curvepoints.size(); ++i) {
        if(within_radius(curvepoints[i].source->location, pos)) {
            curvepoints[i].selected = !curvepoints[i].selected;
        }
    }
    for(int i = 0; i < vecs.size(); ++i) {
        if(within_radius(*vecs[i].source, pos)) {
            vecs[i].selected = !vecs[i].selected;
        }
    }
}

void ShapeEditor::cancel() {
    switch(state) {
        case GRAB: {
            for(int i = 0; i < curvepoints.size(); ++i) {
                if(curvepoints[i].selected) {
                    curvepoints[i].source->location -= action_delta;
                }
            }
            for(int i = 0; i < vecs.size(); ++i) {
                if(vecs[i].selected) {
                    *vecs[i].source -= action_delta;
                }
            }
        }
        break;
    }
    
    state = NONE;
    action_delta = Vec();
}

void ShapeEditor::confirm() {
    state = NONE;
    action_delta = Vec();
}

void ShapeEditor::mouse(MouseEvent m, Vec pos) {
    if(m.button == RIGHT && m.action == PRESS) {
        if(state == NONE) select(pos);
        else cancel();
    }
    
    if(m.button == LEFT && m.action == PRESS) {
        if(state != NONE) confirm();
    }
}