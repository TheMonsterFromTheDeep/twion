#include "editor.h"
#include "curve.h"

#include "shape.h"

#include <iostream>
#include <cmath>

Editor::Editor(int x, int y, int w, int h) :
	Control(x, y, w, h, RGB(0.45f, 0.45f, 0.45f)),
    zoom_exp(0), zoom_amount(1), panning(false), test() {
        test.add(CurvePoint(0, 0, 1, RGB(0, 1, 0)), Vec(0 - 10, 0 - 10), Vec(0 + 10, 0 + 10));
        test.add(CurvePoint(100, 0, 5, RGB(0, 0, 1)), Vec(100 - 10, 0 + 10), Vec(100 + 10, 0 - 10));
        test.add(CurvePoint(100, -100, 3, RGB(1, 0, 0)), Vec(100 + 10, -100 + 10), Vec(100 - 10, -100 - 10));
        
        editor = test.get_editor();
    }
	
Vec Editor::getCursor() {
	return (getMouse() - pan) / zoom_amount;
}

bool Editor::key(KeyEvent ke) {
    if(ke.action == PRESS) {
        editor->key(ke, getCursor());
    }
    
    return false;
}
    
bool Editor::mouse_move(Vec position, Vec delta) {
    if(panning) {
        pan += delta;
    }
    
    Vec cursor = getCursor();
    
    editor->mouse_move(cursor, delta / zoom_amount);
    
    return false;
}

bool Editor::scroll(Vec v) {
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
    
    return false;
}

bool Editor::mouse_button(MouseEvent e) {
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
    
    editor->mouse(e, getCursor());
    
    return false;
}

void Editor::draw(Graphics g) {
    g.translate(pan);
    g.scale(zoom_amount);
    
    editor->draw(g);
}