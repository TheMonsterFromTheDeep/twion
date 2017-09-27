#include "editor.h"
#include "curve.h"

#include <iostream>
#include <cmath>

Editor::Editor(int x, int y, int w, int h) :
	Control(x, y, w, h, RGB(0.7f, 0.7f, 0.7f)),
    zoom_exp(0), zoom_amount(1), panning(false), edit_mode(0) {
        cubics.push_back(InterpolatedCubic(CurvePoint(100, 100, 5), CurvePoint(400, 400, 0), Vec(-100, 0), Vec(100, 0)));
        cubics.push_back(InterpolatedCubic(CurvePoint(100, 100, 5), CurvePoint(-200, -200, 0), Vec(100, 0), Vec(100, 0)));
    }
	
Vec Editor::getCursor() {
	return (getMouse() - pan) / zoom_amount;
}
    
void Editor::mouse_move(Vec position, Vec delta) {
    if(panning) {
        pan += delta;
    }
    
    Vec cursor = getCursor();
    
    switch(edit_mode) {
		case 1:
			current->start.location = cursor;
			break;
		case 2:
			current->end.location = cursor;
			break;
		case 3:
			current->ease_in = cursor - current->start.location;
			break;
		case 4:
			current->ease_out = cursor - current->end.location;
			break;
		default: { }
	}
	
	/*if(edit_mode) {
		current->calculate();
	
		curve = cubic.generate(0.01f);
	}*/
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
            Vec cursor = getCursor();
                
            edit_mode = 0;
                
            for(InterpolatedCubic &ic : cubics) {
                     if(check(ic.start.location,cursor)) edit_mode = 1;
                else if(check(ic.end.location, cursor)) edit_mode = 2;
                else if(check(ic.start.location + ic.ease_in, cursor)) edit_mode = 3;
                else if(check(ic.end.location + ic.ease_out, cursor)) edit_mode = 4;
                if(edit_mode) {
                    current = &ic;
                    break;
                }
            }
        }
        if(e.action == RELEASE) { edit_mode = 0; }
    }
}

void Editor::draw(Graphics g) {
    g.translate(pan);
    g.scale(zoom_amount);
    
    for(InterpolatedCubic &ic : cubics) {
        ic.calculate();
        Curve c = ic.generate(0.01f);
        
        g.rgb(0.f, 0.f, 0.f);
        c.stroke(g);
        
        g.rgb(1.f, 0.f, 0.f);
        g.fill_circle(ic.start.location, 2.5f);
        g.fill_circle(ic.end.location, 2.5f);
        g.draw_circle(ic.start.location + ic.ease_in, 2.5f);
        g.draw_circle(ic.end.location + ic.ease_out, 2.5f);
    }
}