#include <GLFW/glfw3.h>

#include "control.h"

#include <iostream>

void Sizer::size(Control& c, int winWidth, int winHeight) { }

ScaleSizer::ScaleSizer() : x(1), y(1) { }
ScaleSizer::ScaleSizer(float x_, float y_, float ox_, float oy_) : x(x_), y(y_), ox(ox_), oy(oy_) { }

void ScaleSizer::size(Control& c, int winWidth, int winHeight) {
	c.width = winWidth * x;
	c.height = winHeight * y;
    
    c.x = winWidth * ox;
    c.y = winHeight * oy;
}

Control::Control(int x_, int y_, int width_, int height_,RGB bg) :
	x(x_), y(y_), width(width_), height(height_), background(bg) { }

void Control::render(Graphics g) {
	g.rgb(background);
	g.fill_rect(0, 0, width, height);
	
	draw(g);
}

void Control::key(KeyEvent e) { }
	
void Control::draw(Graphics g) { }

void Control::mouse_move(Vec pos, Vec delta) { }
void Control::scroll(Vec delta) { }

void Control::mouse_button(MouseEvent e) { }

Vec Control::getMouse() {
    return parent->getMouse() - Vec(x, y);
}