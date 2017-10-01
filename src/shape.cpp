#include "shape.h"
#include <iostream>

#include <GLFW/glfw3.h>

Shape::Shape() : points(), handles(), curves(), looped(false) { }

void Shape::generate() {
    int len = points.size() - 1;
    if(len > 0) {
        curves.clear();
        for(int i = 0; i < len; ++i) {
            curves.push_back(InterpolatedCubic(&points[i], &points[i + 1], &handles[i * 2 + 1], &handles[i * 2 + 2]));
        }
        if(looped) {
            curves.push_back(InterpolatedCubic(&points[len], &points[0], &handles[2 * len + 1], &handles[0]));
        }
    }
}

ShapeEditor *Shape::get_editor() {
    return new ShapeEditor(this);
}

void Shape::loop() {
    if(looped) return;
    
    looped = true;
    generate();
}

void Shape::unloop() {
    if(!looped) return;
    
    looped = false;
    generate();
}

void Shape::add(CurvePoint c, Vec v0, Vec v1) {
    points.push_back(c);
    handles.push_back(v0);
    handles.push_back(v1);
    generate();
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
    
    for(InterpolatedCubic& ic : curves) {
        ic.calculate();
        Curve c = ic.generate(0.01f);
        for(CurvePoint& p : c) {
            calc.push_back(p);
        }
    }
    
    calc.line(g);
}

void Shape::draw(Graphics g) {
    Curve calc;
    
    for(InterpolatedCubic& ic : curves) {
        ic.calculate();
        Curve c = ic.generate(0.01f);
        for(CurvePoint& p : c) {
            calc.push_back(p);
        }
    }
    
    g.rgb(1.f, 1.f, 1.f);
    
    fill_unloop(calc, g);
    
    calc.stroke(g);
}