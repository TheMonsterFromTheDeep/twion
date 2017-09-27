#include "editor.h"

#include <iostream>
#include <cmath>

Editor::Editor(int x, int y, int w, int h) :
	Control(x, y, w, h, RGB(0.7f, 0.7f, 0.7f)),
    zoom_exp(0), zoom_amount(1), panning(false) { }
	
void Editor::mouse_move(Vec position, Vec delta) {
	//dot = position;
    if(panning) {
        pan += delta;
    }
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
}

void Editor::draw(Graphics g) const {
    g.translate(pan);
    g.scale(zoom_amount);
}