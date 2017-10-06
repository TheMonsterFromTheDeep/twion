#include <stddef.h>

#include <iostream>

#include "shape.h"

EditCurvePoint::EditCurvePoint(CurvePoint *source_) : source(source_), selected(false) { }
EditVec::EditVec(Vec *source_) : source(source_), selected(false) { }

ShapeEditor::ShapeEditor(Shape* source_) : source(source_), constrain_x(false), constrain_y(false), state(NONE), select_state(ZERO) {    
    generate();
}

void ShapeEditor::generate() {
    curvepoints.clear();
    vecs.clear();
    curvepoints_tfrm.clear();
    vecs_tfrm.clear();
    
    for(size_t i = 0; i < source->points.size(); ++i) {
        curvepoints.push_back(EditCurvePoint(&(source->points[i])));
        curvepoints_tfrm.push_back(*curvepoints[i].source);
    }
    
    for(size_t i = 0; i < source->handles.size(); ++i) {
        vecs.push_back(EditVec(&(source->handles[i])));
        vecs_tfrm.push_back(*vecs[i].source);
    }
}

static void diamond(Graphics g, Vec pos, float radius) {
    g.begin_quad();
    g.point(pos + Vec(radius, 0));
    g.point(pos + Vec(0, radius));
    g.point(pos - Vec(radius, 0));
    g.point(pos - Vec(0, radius));
    g.end();
}

static void draw_control_point(Graphics g, CurvePoint p, bool selected) {
    float width_0 = g.normalize(6);
    float width_1 = g.normalize(3);
    
    if(selected) g.rgb(1.f, 0.8f, 0.1f);
    else         g.rgb(0.0f, 0.0f, 0.0f);
    diamond(g, p.location, width_0);
    
    g.rgb(p.color);
    diamond(g, p.location, width_1);
}

static void draw_handle(Graphics g, Vec v, bool selected) {
    float radius = g.normalize(4);
    
    if(selected) g.rgb(1.f, 0.8f, 0.1f);
    else         g.rgb(0.f, 0.f, 0.f);
    
    g.draw_circle(v, radius);
}

void ShapeEditor::draw(Graphics g) {
    source->draw(g);
    
    RGB select(1.f, 0.8f, 0.1);
    RGB unselect(0.f, 0.f, 0.f);
    
    for(size_t i = 0; i < curvepoints.size(); ++i) {
        g.rgb(curvepoints[i].selected && vecs[i * 2].selected ? select : unselect);
        g.line(curvepoints[i].source->location, *(vecs[i * 2].source));
        g.rgb(curvepoints[i].selected && vecs[i * 2 + 1].selected ? select : unselect);
        g.line(curvepoints[i].source->location, *(vecs[1 + (i * 2)].source));
    }
    
    for(size_t i = 0; i < curvepoints.size(); ++i) {
        draw_control_point(g, *curvepoints[i].source, curvepoints[i].selected);
    }
    
    for(size_t i = 0; i < vecs.size(); ++i) {
        draw_handle(g, *(vecs[i].source), vecs[i].selected);
    }
    
    if(constrain_x) {
        g.rgb(1.f, 0.f, 0.f);
        g.line(action_pivot - Vec(100, 0), action_pivot + Vec(100, 0));
    }
    
    if(constrain_y) {
        g.rgb(0.f, 1.f, 0.f);
        g.line(action_pivot - Vec(0, 100), action_pivot + Vec(0, 100));
    }
    
    if(state != NONE) {
        g.rgb(0.f, 0.f, 0.f);
        
        float lw = g.normalize(10);
        float r = g.normalize(5);
        
        g.line(action_pivot - Vec(0, lw), action_pivot + Vec(0, lw));
        g.line(action_pivot - Vec(lw, 0), action_pivot + Vec(lw, 0));
        g.draw_circle(action_pivot, r);
    }
    
    g.rgb(1.f, 0.f, 0.f);
    source->line(g);
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
    
    bool single_handle = divisor == 0 && (state == ROTATE || state == SCALE);
    size_t single_index;
    
    for(size_t i = 0; i < vecs.size(); ++i) {
        vecs_tfrm[i] = *vecs[i].source;
        if(vecs[i].selected) {
            action_pivot += *vecs[i].source;
            single_index = i;
            ++divisor;
        }
    }
    
    single_handle = single_handle && divisor == 1;
    
    /* Basically, if only a single handle is selected, rotate around that
     * handle's parent control point.
     */
    if(single_handle) {
        action_pivot = curvepoints[single_index / 2].source->location;
    }
    
    if(divisor > 0) {
        action_pivot /= divisor;
    }
     
    constrain_x = false;
    constrain_y = false;
}

void ShapeEditor::key(KeyEvent e, Vec mouse) {
    if(select_state != ZERO) {
        if(e.key == 'G') {
            state = GRAB;
            action_center = mouse;
            
            init_action();
        }
        
        if(e.key == 'R') {
            if(e.control_down) {
                split();
            }
            else {
                state = ROTATE;
                action_center = mouse;
                
                init_action();
            }
        }
        
        if(e.key == 'S') {
            state = SCALE;
            action_center = mouse;
            
            init_action();
        }
        
        if(e.key == 'A' && e.control_down) {
            state = THICKEN;
            action_center = mouse;
            
            init_action();
        }
        
        if(e.key == 'C') {
            if(e.shift_down) {
                for(size_t i = 0; i < curvepoints.size(); ++i) {
                    if(curvepoints[i].selected) {
                        curvepoints[i].source->color = edit_color;
                    }
                }
            }
            else {
                for(size_t i = 0; i < curvepoints.size(); ++i) {
                    if(curvepoints[i].selected) {
                        curvepoints[i].source->fill = edit_color;
                    }
                }
            }
        }
    }
    
    if(e.key == 'A' && !e.control_down) { all_select(); }
    
    if(e.key == 'L') { source->toggle_loop(); }
    
    if(state == GRAB || state == SCALE) {
        if(e.key == 'X') {
            constrain_x = !constrain_x;
            constrain_y = constrain_y && !constrain_x;
        }
        
        if(e.key == 'Y') {
            constrain_y = !constrain_y;
            constrain_x = constrain_x && !constrain_y;
        }
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
        
        case SCALE: {
            /* This is less efficient than it could be (two square roots, action_center - action_pivot could be cached)
             * but it doesn't really matter right now.
             */
            float magnitude = (position - action_pivot).len() / (action_center - action_pivot).len();
            
            Vec scale(magnitude);
            if(constrain_x) scale.y = 1;
            if(constrain_y) scale.x = 1;
            
            for(size_t i = 0; i < curvepoints.size(); ++i) {
                if(curvepoints[i].selected) {
                    curvepoints[i].source->location = curvepoints_tfrm[i].location.scale(scale, action_pivot);
                }
            }
            for(size_t i = 0; i < vecs.size(); ++i) {
                if(vecs[i].selected) {
                    *vecs[i].source = vecs_tfrm[i].scale(scale, action_pivot);
                }
            }
        }
        break;
        
        case THICKEN: {
            float magnitude = (position - action_pivot).len() / (action_center - action_pivot).len();
            
            for(size_t i = 0; i < curvepoints.size(); ++i) {
                if(curvepoints[i].selected) {
                    curvepoints[i].source->width = curvepoints_tfrm[i].width * magnitude;
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

void ShapeEditor::split() {
    std::vector<size_t> split_points;
    
    for(size_t i = 0; i < curvepoints.size() - 1; ++i) {
        if(curvepoints[i].selected && curvepoints[i + 1].selected) split_points.push_back(i);
    }
    
    std::vector<CurvePoint> new_points;
    std::vector<Vec> new_deltas;
    
    for(size_t i = 0; i < split_points.size(); ++i) {
        new_points.push_back(source->curves[split_points[i]].evaluate(0.5f));
        
        /* TODO: Figure out why dividing handle length by 2 maintains original shape */
        new_deltas.push_back(source->curves[split_points[i]].eval_ease_delta(0.5f) / 2);
        
        source->handles[i * 2 + 1] = (source->handles[i * 2 + 1] - source->points[i].location) / 2 + source->points[i].location;
        source->handles[i * 2 + 2] = (source->handles[i * 2 + 2] - source->points[i + 1].location) / 2 + source->points[i + 1].location;
    }
    
    size_t offset = 1;
    
    for(size_t i = 0; i < split_points.size(); ++i) {        
        size_t x = split_points[i] + offset;
        
        source->points.insert(source->points.begin() + x, new_points[i]);
        
        size_t vp = x * 2;
        
        source->handles.insert(source->handles.begin() + vp, new_points[i].location + new_deltas[i]);
        source->handles.insert(source->handles.begin() + vp, new_points[i].location - new_deltas[i]);
        
        source->generate();
        
        ++offset;
    }
    
    generate();
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