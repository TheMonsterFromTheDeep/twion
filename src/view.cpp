#include "view.h"
#include <iostream>

View::View(int x_, int y_, int width_, int height_,RGB bg) :
	Control(x_, y_, width_, height_, bg),
    active(false), amount(0.5f) { }
    
bool View::mouse_move(Vec pos, Vec delta) {
    if(active) {
        amount = pos.x / width;
        
        size(children[0], width, height);
        size(children[1], width, height);
    }
    
    return active;
}

bool View::mouse_button(MouseEvent e) {
    float delta = e.position.x - (width * amount);
    
    if(delta < 5 && delta > -5) {
        if(e.button == LEFT && e.action == PRESS) active = true;
    }
    
    if(e.button == LEFT && e.action == RELEASE) active = false;
    
    return active;
}

void View::draw(Graphics g) {    
    g.rgb(0.f, 0.f, 0.f);
    g.line(width * amount, 0, width * amount, height);
}

void View::size(Control* c, int width, int height) {
    if(c == children[0]) {
        c->x = 0;
        c->y = 0;
        c->width = width * amount;
        c->height = height;
    }
    else if(c == children[1]) {
        c->x = width * amount;
        c->y = 0;
        c->width = width * (1 - amount);
        c->height = height;
    }
}