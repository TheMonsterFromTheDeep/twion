#include "editor.h"
#include "curve.h"

#include "shape.h"

#include <iostream>
#include <cmath>

Editor::Editor(int x, int y, int w, int h) :
	Control(x, y, w, h, RGB(0.7f, 0.7f, 0.7f)),
    zoom_exp(0), zoom_amount(1), panning(false), test() {
        test.add(CurvePoint(0, 0, 1, RGB(0, 0, 0)), Vec(0, 0), Vec(10, 10));
        test.add(CurvePoint(100, 0, 5, RGB(0, 0, 1)), Vec(-10, 10), Vec(10, -10));
        test.add(CurvePoint(100, -100, 1, RGB(0, 0, 0)), Vec(10, 10), Vec(0, 0));
        //test.add(CurvePoint(100, 100, 1, RGB(0, 0, 0)), Vec(0, -50));
    }
	
Vec Editor::getCursor() {
	return (getMouse() - pan) / zoom_amount;
}

void Editor::key(char c) {
}
    
void Editor::mouse_move(Vec position, Vec delta) {
    if(panning) {
        pan += delta;
    }
    
    Vec cursor = getCursor();
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
        }
    }
    if(e.button == RIGHT) {
        if(e.action == PRESS) {
        }
    }
}

void Editor::draw(Graphics g) {
    g.translate(pan);
    g.scale(zoom_amount);
    
    test.draw(g);
}