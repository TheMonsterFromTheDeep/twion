#include <GLFW/glfw3.h>
#include <cmath>

#include "graphics.h"

RGB::RGB() : r(0), g(0), b(0) { }
RGB::RGB(float gray) : r(gray), g(gray), b(gray) { }
RGB::RGB(float r_, float g_, float b_) : r(r_), g(g_), b(b_) { }

RGB RGB::interpolate(RGB other, float t) {
    return RGB(r + (other.r - r) * t, g + (other.g - g) * t, b + (other.b - b) * t);
}

Graphics::Graphics(Vec corner_) : corner(corner_) {
	reset();
}

void Graphics::rgb(float r, float g, float b) {
	glColor3f(r, g, b);
}

void Graphics::rgb(RGB color) { rgb(color.r, color.g, color.b); }

void Graphics::next_layer() { glClear(GL_DEPTH_BUFFER_BIT); }

void Graphics::begin_points() { glBegin(GL_POINTS); }

void Graphics::begin_lines() { glBegin(GL_LINES); }
void Graphics::begin_line_strip() { glBegin(GL_LINE_STRIP); }
void Graphics::begin_line_loop() { glBegin(GL_LINE_LOOP); }

void Graphics::begin_quad() { glBegin(GL_QUADS); }
void Graphics::begin_quad_strip() { glBegin(GL_QUAD_STRIP); }

void Graphics::begin_triangle() { glBegin(GL_TRIANGLES); }
void Graphics::begin_triangle_strip() { glBegin(GL_TRIANGLE_STRIP); }
void Graphics::begin_triangle_fan() { glBegin(GL_TRIANGLE_FAN); }

void Graphics::begin_polygon() { glBegin(GL_POLYGON); }

void Graphics::end() { glEnd(); }

void Graphics::point(float x, float y) { glVertex2f(x, y); }
void Graphics::point(Vec v) { point(v.x, v.y); }

void Graphics::fill_rect(float x, float y, float width, float height) {
	begin_quad();
		point(x, y);
		point(x, y + height);
		point(x + width, y + height);
		point(x + width, y);
	end();
}

void Graphics::fill_rect(float x, float y, Vec size) { fill_rect(x, y, size.x, size.y); }
void Graphics::fill_rect(Vec pos, float w, float h) { fill_rect(pos.x, pos.y, w, h); }
void Graphics::fill_rect(Vec pos, Vec size) { fill_rect(pos.x, pos.y, size.x, size.y); }

void Graphics::draw_rect(float x, float y, float width, float height) {
	begin_line_loop();
		point(x, y);
		point(x, y + height);
		point(x + width, y + height);
		point(x + width, y);
	end();
}

void Graphics::draw_rect(float x, float y, Vec size) { draw_rect(x, y, size.x, size.y); }
void Graphics::draw_rect(Vec pos, float w, float h) { draw_rect(pos.x, pos.y, w, h); }
void Graphics::draw_rect(Vec pos, Vec size) { draw_rect(pos.x, pos.y, size.x, size.y); }

void Graphics::fill_circle(float cx, float cy, float radius) {
	begin_triangle_fan();
	
	point(cx, cy);
	
	for(float t = 0; t < 6.28; t += 0.27f) {
		point(cx + radius * cos(t), cy + radius * sin(t));
	}
	
	point(cx + radius, cy);
	
	end();
}

void Graphics::fill_circle(Vec center, float radius) { fill_circle(center.x, center.y, radius); }

void Graphics::draw_circle(float cx, float cy, float radius) {
	begin_line_loop();
	
	for(float t = 0; t < 6.28; t += 0.27f) {
		point(cx + radius * cos(t), cy + radius * sin(t));
	}
	
	end();
}

void Graphics::draw_circle(Vec center, float radius) { draw_circle(center.x, center.y, radius); }

void Graphics::line(float sx, float sy, float ex, float ey) {
    begin_lines();
    
    point(sx, sy);
    point(ex, ey);
    
    end();
}
void Graphics::line(Vec s, float ex, float ey) { line(s.x, s.y, ex, ey); }
void Graphics::line(float sx, float sy, Vec e) { line(sx, sy, e.x, e.y); }
void Graphics::line(Vec s, Vec e) { line(s.x, s.y, e.x, e.y); }

void Graphics::translate(float dx, float dy) { glTranslatef(dx, dy, 0); }
void Graphics::translate(Vec d) { translate(d.x, d.y); }

void Graphics::rotate(float angle) { glRotatef(angle, 0, 0, 1); }
void Graphics::rotate(float angle, float px, float py) {
	translate(px, py);
	rotate(angle);
	translate(-px, -py);
}
void Graphics::rotate(float angle, Vec pivot) { rotate(angle, pivot.x, pivot.y); }

void Graphics::scale(float amount) { glScalef(amount, amount, 1.f); }
void Graphics::scale(float amount, float px, float py) {
	translate(px, py);
	scale(amount);
	translate(-px, -py);
}
void Graphics::scale(float amount, Vec pivot) { scale(amount, pivot.x, pivot.y); }

void Graphics::reset() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	translate(corner);
}