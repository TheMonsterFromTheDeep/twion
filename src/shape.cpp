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

void Shape::draw(Graphics g) {
    std::vector<Curve> calc;
    
    for(InterpolatedCubic& ic : curves) {
        ic.calculate();
        Curve c = ic.generate(0.01f);
        c.stroke(g);
        calc.push_back(c);
    }
    
    g.rgb(0.5f, 0.5f, 0.5f);
    g.begin_triangle_fan();
    
    float angle_last;
    bool has_point_1 = false;
    bool has_point_2 = false;
    bool has_point_3 = false;
    
    bool has_direction = false;
    
    bool less_than = false;
    
    Vec last;
    
    std::vector<Vec> inner_points;
    
    for(Curve& c : calc) {
        for(CurvePoint& p : c) {
            g.point(p.location);
            
            if(!has_point_1) {
                has_point_1 = true;
                inner_points.push_back(p.location);
            }
            else if(!has_point_2) {
                has_point_2 = true;
                angle_last = p.location.angle(last);
            }
            else if(!has_point_3) {
                has_point_3 = true;
                float angle = p.location.angle(last);
                
                if(!has_direction) {
                    if(angle < angle_last) { less_than = true; has_direction = true; }
                    else if(angle > angle_last) { less_than = false; has_direction = true; }
                    else has_point_3 = false;
                }
                
                angle_last = angle;
            }
            else {
                float angle = p.location.angle(last);
                if(angle != angle_last) {
                    if((angle < angle_last) != less_than) {
                        has_point_1 = false;
                        has_point_2 = false;
                        has_point_3 = false;
                        
                        g.end();
                        g.begin_triangle_fan();
                        
                        inner_points.push_back(p.location);
                    }
                }
                angle_last = angle;
            }
            
            last = p.location;
        }
    }
    
    inner_points.push_back(last);
    
    g.end();
    
    g.begin_polygon();
    
    for(Vec& v : inner_points) {
        g.point(v);
    }
    
    g.end();
}