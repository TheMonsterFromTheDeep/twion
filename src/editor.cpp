#include "editor.h"
#include "curve.h"

#include "editablepoint.h"

#include <iostream>
#include <cmath>

Editor::Editor(int x, int y, int w, int h) :
	Control(x, y, w, h, RGB(0.7f, 0.7f, 0.7f)),
    zoom_exp(0), zoom_amount(1), panning(false), is_editing(false) {
        world.push_back(new EditablePoint(CurvePoint(0, 0, 5)));
    }
	
Vec Editor::getCursor() {
	return (getMouse() - pan) / zoom_amount;
}

void Editor::key(char c) {
    for(Editable *e : selected) {
        if(e->key(c)) { is_editing = true; }
    }
}
    
void Editor::mouse_move(Vec position, Vec delta) {
    if(panning) {
        pan += delta;
    }
    
    Vec cursor = getCursor();
    
    for(Editable *e : selected) {
        e->mouse(cursor, delta / zoom_amount);
    }
}

static bool check(Vec location, Vec mouse) {
	return (location - mouse).lensqr() < 5 * 5;
}

void Editor::scroll(Vec v) {
	if(v.y != 0) {
        float old_amount = zoom_amount;
        
        zoom_exp += v.y;
        zoom_amount = pow(1.1, zoom_exp);
        
        old_amount = zoom_amount / old_amount;
        
        Vec m = getMouse();
        
        pan -= m;
        pan *= old_amount;
        pan += m;
    }
}

void Editor::mouse_button(MouseEvent e) {
    if(e.button == MIDDLE) {
        if(e.action == PRESS) {
            panning = true;
        }
        if(e.action == RELEASE) {
            panning = false;
        }
    }
    if(e.button == LEFT) {
        if(e.action == PRESS) {
            for(Editable *e : selected) {
                e->confirm();
            }
            is_editing = false;
        }
    }
    if(e.button == RIGHT) {
        if(e.action == PRESS) {
            if(is_editing) {
                for(Editable *e : selected) {
                    e->cancel();
                }
                is_editing = false;
            }
            else {
                Vec m = getCursor();
                
                for(Editable *e : world) {
                    if(e->select(m)) { selected.push_back(e); }
                }
            }
        }
    }
}

void Editor::draw(Graphics g) {
    g.translate(pan);
    g.scale(zoom_amount);
    
    for(Editable *e : world) {
        CurvePoint p = static_cast<EditablePoint*>(e)->point;
        g.rgb(0.f, 0.f, 0.f);
        g.fill_circle(p.location, p.width);
        g.rgb(1.0f, 0.f, 0.f);
        g.draw_circle(p.location, 5);
    }
}