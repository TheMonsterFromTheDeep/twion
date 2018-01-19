#include <GLFW/glfw3.h>

#include "control.h"

#include <iostream>

void Sizer::size(Control* c, int winWidth, int winHeight) { }

ScaleSizer::ScaleSizer() : x(1), y(1) { }
ScaleSizer::ScaleSizer(float x_, float y_, float ox_, float oy_) : x(x_), y(y_), ox(ox_), oy(oy_) { }

void ScaleSizer::size(Control* c, int winWidth, int winHeight) {
	c->width = winWidth * x;
	c->height = winHeight * y;
    
    c->x = winWidth * ox;
    c->y = winHeight * oy;
}

Control::Control(int x_, int y_, int width_, int height_,RGB bg) :
	x(x_), y(y_), width(width_), height(height_), background(bg), sizer(NULL) { }

static void child_draw(Control* c) {
    glEnable(GL_SCISSOR_TEST);
	
	glScissor(c->x, c->y, c->width, c->height);
	
	Graphics gfx(Vec(c->x, c->y));
    
	c->render(gfx);
    
	glDisable(GL_SCISSOR_TEST);
}

void Control::invalidate() {
    glfwPostEmptyEvent();
}

void Control::size(int sz_width, int sz_height) {
    if(sizer) sizer->size(this, sz_width, sz_height);
    for(Control* c : children) {
        if(c) c->size(width, height);
    }
}
    
void Control::render(Graphics g) {
	g.rgb(background);
	g.fill_rect(0, 0, width, height);
    
    for(Control* c : children) {
        if(c) child_draw(c);
    }
    
    g.reset();
    draw(g);
}

void Control::key_(KeyEvent e) {
    if(key(e)) return;
    
    for(Control* c : children) {
        if(c) c->key_(e);
    }
}

void Control::mouse_move_(Vec pos, Vec delta) {
    if(mouse_move(pos, delta)) return;
    
    for(Control* c : children) {
        if(c) c->mouse_move_(pos - Vec(c->x, c->y), delta);
    }
}

void Control::scroll_(Vec delta) {
    if(scroll(delta)) return;
    
    for(Control* c : children) {
        if(c) c->scroll_(delta);
    }
}

void Control::mouse_button_(MouseEvent e) {    
    if(mouse_button(e)) return;
    
    for(Control* c : children) {
        if(c) {
            MouseEvent child_event = e;
            child_event.position -= Vec(c->x, c->y);
            c->mouse_button_(child_event);
        }
    }
}

void Control::reinitialize() {
    /* TODO: Figure out how to nicely handle deleting pointers so there isn't a memory link */
    children.clear();
}

void Control::attach(Control* c) {
    if(!c) return;
    
    children.push_back(c);
    c->parent = this;
    c->size(width, height);
}

bool Control::key(KeyEvent e) { return false; }
	
void Control::draw(Graphics g) { }

bool Control::mouse_move(Vec pos, Vec delta) { return false;}
bool Control::scroll(Vec delta) { return false;}

bool Control::mouse_button(MouseEvent e) { return false;}

Vec Control::getMouse() {
    return parent->getMouse() - Vec(x, y);
}