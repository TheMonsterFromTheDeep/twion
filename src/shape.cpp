#include "shape.h"
#include <iostream>

#include <GLFW/glfw3.h>

ShapePoint::ShapePoint(const CurvePoint& data_) : data(data_), lines() { }

void ShapeLine::draw_handles(Graphics g) {
	float radius = g.normalize(4);
	
    g.draw_circle(start_handle, radius);
	g.draw_circle(end_handle, radius);
	
	g.line(start_handle, start->data.location);
	g.line(end_handle, end->data.location);
}

static void diamond(Graphics g, Vec pos, float radius) {
    g.begin_quad();
    g.point(pos + Vec(radius, 0));
    g.point(pos + Vec(0, radius));
    g.point(pos - Vec(radius, 0));
    g.point(pos - Vec(0, radius));
    g.end();
}

void ShapePoint::draw(Graphics g) {
	float width_0 = g.normalize(6);
    float width_1 = g.normalize(3);
    
	g.rgb(1.f, 0.8f, 0.1f);
    //if(selected) g.rgb(1.f, 0.8f, 0.1f);
    //else         g.rgb(0.0f, 0.0f, 0.0f);
    diamond(g, data.location, width_0);
    
    g.rgb(data.color);
    diamond(g, data.location, width_1);
}

Shape::Shape() : points(), lines() {
	add_line(
		CurvePoint(Vec(0, 0), 1.f, RGB(1.f, 1.f, 1.f)),
		CurvePoint(Vec(30, 30), 1.f, RGB(1.f, 1.f, 1.f)),
		Vec(3, 3),
		Vec(30, 15)
	);
}

ShapePoint* Shape::add_point(const CurvePoint &p) {
	points.push_back(new ShapePoint(p));
	return points.back();
}

ShapeLine* Shape::add_line(const CurvePoint &a, const CurvePoint& b, Vec ae, Vec be) {
	ShapeLine *sl = new ShapeLine();
	sl->start = add_point(a);
	sl->end = add_point(b);
	sl->start_handle = ae;
	sl->end_handle = be;
	lines.push_back(sl);
	return lines.back();
}

ShapeEditor *Shape::get_editor() {
    return new ShapeEditor(this);
}

void fill_unloop(Curve& points, Graphics g) {
    g.begin_quad_strip();
    
    size_t last = points.size() - 1;
    size_t i = 0;
    while(i < last - i) {
        g.rgb(points[i].fill);
        g.point(points[i].location);
        g.rgb(points[last - i].fill);
        g.point(points[last - i].location);
        ++i;
    }
    
    g.end();
}

void Shape::line(Graphics g) {
    Curve calc;
    
    g.translate(position);
    
    for(ShapeLine *sl : lines) {
		InterpolatedCubic ic(&sl->start->data, &sl->end->data, &sl->start_handle, &sl->end_handle);
        ic.calculate();
        Curve c = ic.generate(0.01f);
        for(CurvePoint& p : c) {
            calc.push_back(p);
        }
    }
    
    calc.line(g);
    
    g.translate(position * -1);
}

void Shape::draw(Graphics g) {
    Curve calc;
    
    g.translate(position);
    
    for(ShapeLine *sl : lines) {
		InterpolatedCubic ic(&sl->start->data, &sl->end->data, &sl->start_handle, &sl->end_handle);
        ic.calculate();
        Curve c = ic.generate(0.01f);
        for(CurvePoint& p : c) {
            calc.push_back(p);
        }
    }
    
    g.rgb(1.f, 1.f, 1.f);
    
    //fill_unloop(calc, g);
    
    calc.stroke(g);
    
    g.translate(position * -1);
}