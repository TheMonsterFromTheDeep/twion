#include "shape.h"

#include <iostream>

EditCurvePoint::EditCurvePoint(CurvePoint *source_) : source(source_), selected(false) { }
EditVec::EditVec(Vec *source_) : source(source_), selected(false) { }

ShapeEditor::ShapeEditor(Shape* source_) : source(source_), state(NONE), select_state(ZERO) {    
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
    
    for(int i = 0; i < curvepoints.size(); ++i) {
        g.rgb(curvepoints[i].selected ? select : unselect);
        g.draw_circle(curvepoints[i].source->location, 5);
    }
    
    for(int i = 0; i < vecs.size(); ++i) {
        g.rgb(vecs[i].selected ? select : unselect);
        g.draw_circle(*(vecs[i].source), 3);
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
    int new_selection = -1;
    
    SelectState new_state = ZERO;
    
    for(int i = 0; i < curvepoints.size(); ++i) {
        if(within_radius(curvepoints[i].source->location, pos)) {
            new_selection = i;
            break;
        }
    }
    
    if(new_selection >= 0) {
        for(int i = 0; i < curvepoints.size(); ++i) {
            /* We want to:
             * - Unselect all points other than the new one
             * - Select the new one if it is not selected (i.e. !curvepoints[i].selected)
             * - Unselected the new one if it is selected
             * - Always select the new one if more than one is selected (otherwise, if more than one is selected,
             *   and one that is already selected is selected again, it will become un-selected).
             * This simple statement does all of that: whether a particular point is selected
             * is true if it has the same index as the new point, and is false if it doesn't
             * have the same index or if it was already selected.
             */
            curvepoints[i].selected = (i == new_selection) && (!curvepoints[i].selected || select_state == SOME);
            if(curvepoints[i].selected) new_state = ONE;
        }
        
        for(int i = 0; i < vecs.size(); ++i) {
            vecs[i].selected = false;
        }
        
        select_state = new_state;
        
        /* A point has already been selected - so no need to look for more. */
        return;
    }
   
    /* new_selection is still -1 at this point; does not need to be reset. */
   
    for(int i = 0; i < vecs.size(); ++i) {
        if(within_radius(*vecs[i].source, pos)) {
            new_selection = i;
            break;
        }
    }
    
    if(new_selection >= 0) {
        for(int i = 0; i < vecs.size(); ++i) {
            vecs[i].selected = (i == new_selection) && (!vecs[i].selected || select_state == SOME);
            if(vecs[i].selected) new_state = ONE;
        }
        
        for(int i = 0; i < curvepoints.size(); ++i) {
            curvepoints[i].selected = false;
        }
        
        select_state = new_state;
    }
}

void ShapeEditor::shift_select(Vec pos) {
    int select_count = 0;
    
    for(int i = 0; i < curvepoints.size(); ++i) {
        if(within_radius(curvepoints[i].source->location, pos)) {
            curvepoints[i].selected = !curvepoints[i].selected;
        }
        if(curvepoints[i].selected) ++select_count;
    }
    for(int i = 0; i < vecs.size(); ++i) {
        if(within_radius(*vecs[i].source, pos)) {
            vecs[i].selected = !vecs[i].selected;
        }
        if(vecs[i].selected) ++select_count;
    }
    
    if(select_count > 1) { select_state = SOME; }
    else if(select_count == 1) { select_state = ONE; }
    else { select_state = ZERO; }
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
        if(state == NONE) {
            if(m.shift_down) shift_select(pos);
            else select(pos);
        }
        else cancel();
    }
    
    if(m.button == LEFT && m.action == PRESS) {
        if(state != NONE) confirm();
    }
}