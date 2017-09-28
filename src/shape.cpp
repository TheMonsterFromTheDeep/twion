#include "shape.h"
#include <iostream>

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

void Shape::draw(Graphics g) {
    for(InterpolatedCubic& ic : curves) {
        ic.calculate();
        Curve c = ic.generate(0.01f);
        c.stroke(g);
    }
}