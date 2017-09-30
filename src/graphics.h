#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "vector.h"

class RGB {
public:
	float r, g, b;
	RGB();
	RGB(float);
	RGB(float,float,float);
    
    RGB interpolate(RGB,float);
};

class Graphics {
public:
	Graphics(Vec);

	void rgb(float,float,float);
	void rgb(RGB);
	
	void next_layer();
	
	void fill_rect(Vec,Vec);
	void fill_rect(Vec,float,float);
	void fill_rect(float,float,Vec);
	void fill_rect(float,float,float,float);
	
	void draw_rect(Vec,Vec);
	void draw_rect(Vec,float,float);
	void draw_rect(float,float,Vec);
	void draw_rect(float,float,float,float);
	
	void fill_circle(Vec,float);
	void fill_circle(float,float,float);
	
	void draw_circle(Vec,float);
	void draw_circle(float,float,float);
    
    void line(float,float,float,float);
    void line(Vec,float,float);
    void line(float,float,Vec);
    void line(Vec,Vec);
    
    void begin_points();
	
	void begin_lines();
	void begin_line_strip();
	void begin_line_loop();
	
	void begin_quad();
	void begin_quad_strip();
	
	void begin_triangle();
	void begin_triangle_strip();
	void begin_triangle_fan();
	
	void begin_polygon();
	
	void point(float,float);
	void point(Vec);
	
	void end();
	
	void translate(float,float);
	void translate(Vec);
	
	void rotate(float);
	void rotate(float,float,float);
	void rotate(float,Vec);
	
	void scale(float);
	void scale(float,float,float);
	void scale(float,Vec);
	
	void reset();
private:
	Vec corner;
};

#endif