#include <stddef.h>

#include "shape.h"

EditCurvePoint::EditCurvePoint(CurvePoint *source_) : source(source_), selected(false) { }
EditVec::EditVec(Vec *source_) : source(source_), selected(false) { }

ShapeEditor::ShapeEditor(Shape* source_) : source(source_), constrain_x(false), constrain_y(false), state(NONE), select_state(ZERO) {    
    for(size_t i = 0; i < source->points.size(); ++i) {
        curvepoints.push_back(EditCurvePoint(&(source->points[i])));
        curvepoints_tfrm.push_back(*curvepoints[i].source);
    }
    
    for(size_t i = 0; i < source->handles.size(); ++i) {
        vecs.push_back(EditVec(&(source->handles[i])));
        vecs_tfrm.push_back(*vecs[i].source);
    }
}

void ShapeEditor::draw(Graphics g) {
    source->draw(g);
    
    RGB unselect(0.f, 0.f, 1.f);
    RGB select(0.f, 1.f, 1.f);
    
    for(size_t i = 0; i < curvepoints.size(); ++i) {
        g.rgb(curvepoints[i].selected ? select : unselect);
        g.draw_circle(curvepoints[i].source->location, 5);
    }
    
    for(size_t i = 0; i < vecs.size(); ++i) {
        g.rgb(vecs[i].selected ? select : unselect);
        g.draw_circle(*(vecs[i].source), 3);
    }
    
    if(constrain_x) {
        g.rgb(1.f, 0.f, 0.f);
        g.line(action_pivot - Vec(100, 0), action_pivot + Vec(100, 0));
    }
    
    if(constrain_y) {
        g.rgb(0.f, 1.f, 0.f);
        g.line(action_pivot - Vec(0, 100), action_pivot + Vec(0, 100));
    }
}

void ShapeEditor::all_select() {
    
    /* Select points if none are selected, deselect all selected
     * points if one or more is selected */
    bool new_value = (select_state == ZERO);
    
    if(new_value) {
        /* Handle all possible "select all" scenarios */
        if(curvepoints.size() + vecs.size() == 1) select_state = ONE;
        else if(curvepoints.size() + vecs.size() > 1) select_state = SOME;
        else select_state = ZERO;
    }
    else { select_state = ZERO; }
    
    for(size_t i = 0; i < curvepoints.size(); ++i) {
         curvepoints[i].selected = new_value;
    }
    for(size_t i = 0; i < vecs.size(); ++i) {
         vecs[i].selected = new_value;
    }
}

void ShapeEditor::init_action() {
    action_pivot = Vec();
    float divisor = 0;
    
    for(size_t i = 0; i < curvepoints.size(); ++i) {
        curvepoints_tfrm[i] = *curvepoints[i].source;
        if(curvepoints[i].selected) {
            action_pivot += curvepoints[i].source->location;
            ++divisor;
        }
    }
    
    for(size_t i = 0; i < vecs.size(); ++i) {
        vecs_tfrm[i] = *vecs[i].source;
        if(vecs[i].selected) {
            action_pivot += *vecs[i].source;
            ++divisor;
        }
    }
    
    if(divisor > 0) {
        action_pivot /= divisor;
    }
     
    constrain_x = false;
    constrain_y = false;
}

void ShapeEditor::key(char c, Vec mouse) {
    if(c == 'G' && select_state != ZERO) {
        state = GRAB;
        action_center = mouse;
        
        init_action();
    }
    
    if(c == 'R' && select_state != ZERO) {
        state = ROTATE;
        action_center = mouse;
        
        init_action();
    }
    
    if(c == 'A') { all_select(); }
    
    if(c == 'X' && select_state != ZERO) {
        constrain_x = !constrain_x;
        constrain_y = constrain_y && !constrain_x;
    }
    
    if(c == 'Y' && select_state != ZERO) {
        constrain_y = !constrain_y;
        constrain_x = constrain_x && !constrain_y;
    }
}

void ShapeEditor::mouse_move(Vec position, Vec delta) {    
    switch(state) {
        case GRAB: {
            Vec translation = position - action_center;
            if(constrain_x) { translation.y = 0; }
            if(constrain_y) { translation.x = 0; }
            
            for(size_t i = 0; i < curvepoints.size(); ++i) {
                if(curvepoints[i].selected) {
                    curvepoints[i].source->location = curvepoints_tfrm[i].location + translation;
                }
            }
            for(size_t i = 0; i < vecs.size(); ++i) {
                if(vecs[i].selected) {
                    *vecs[i].source = vecs_tfrm[i] + translation;
                }
            }
        }
        break;
        
        case ROTATE: {
            Vec direction = position.rotate_inverse(action_center - action_pivot, action_pivot) - action_pivot;
            
            for(size_t i = 0; i < curvepoints.size(); ++i) {
                if(curvepoints[i].selected) {
                    curvepoints[i].source->location = curvepoints_tfrm[i].location.rotate(direction, action_pivot);
                }
            }
            for(size_t i = 0; i < vecs.size(); ++i) {
                if(vecs[i].selected) {
                    *vecs[i].source = vecs_tfrm[i].rotate(direction, action_pivot);
                }
            }
        }
        break;
        
        default: { }
    }
}

static bool within_radius(Vec a, Vec b) {
    return (b - a).lensqr() < 5 * 5;
}

void ShapeEditor::select(Vec pos) {
    bool has_new = false;
    size_t new_selection = 0;
    
    SelectState new_state = ZERO;
    
    for(size_t i = 0; i < curvepoints.size(); ++i) {
        if(within_radius(curvepoints[i].source->location, pos)) {
            new_selection = i;
            has_new = true;
            break;
        }
    }
    
    if(has_new) {
        for(size_t i = 0; i < curvepoints.size(); ++i) {
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
        
        for(size_t i = 0; i < vecs.size(); ++i) {
            vecs[i].selected = false;
        }
        
        select_state = new_state;
        
        /* A point has already been selected - so no need to look for more. */
        return;
    }
   
    /* new_selection is still -1 at this point; does not need to be reset. */
   
    for(size_t i = 0; i < vecs.size(); ++i) {
        if(within_radius(*vecs[i].source, pos)) {
            new_selection = i;
            has_new = true;
            break;
        }
    }
    
    if(has_new) {
        for(size_t i = 0; i < vecs.size(); ++i) {
            vecs[i].selected = (i == new_selection) && (!vecs[i].selected || select_state == SOME);
            if(vecs[i].selected) new_state = ONE;
        }
        
        for(size_t i = 0; i < curvepoints.size(); ++i) {
            curvepoints[i].selected = false;
        }
        
        select_state = new_state;
    }
}

void ShapeEditor::shift_select(Vec pos) {
    size_t select_count = 0;
    
    for(size_t i = 0; i < curvepoints.size(); ++i) {
        if(within_radius(curvepoints[i].source->location, pos)) {
            curvepoints[i].selected = !curvepoints[i].selected;
        }
        if(curvepoints[i].selected) ++select_count;
    }
    for(size_t i = 0; i < vecs.size(); ++i) {
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
    for(size_t i = 0; i < curvepoints.size(); ++i) {
        if(curvepoints[i].selected) {
            curvepoints[i].source->location = curvepoints_tfrm[i].location;
            curvepoints[i].source->width = curvepoints_tfrm[i].width;
        }
    }
    for(size_t i = 0; i < vecs.size(); ++i) {
        if(vecs[i].selected) {
            *vecs[i].source = vecs_tfrm[i];
        }
    }
    
    state = NONE;
    
    constrain_x = constrain_y = false;
}

void ShapeEditor::confirm() {
    state = NONE;
    
    constrain_x = constrain_y = false;
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