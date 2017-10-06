#include "curve.h"
#include <cstdlib>
#include <iostream>

CurvePoint::CurvePoint() : location(), width(), color() { }
CurvePoint::CurvePoint(const CurvePoint& c) : location(c.location), width(c.width), color(c.color), fill(c.fill) { }
CurvePoint::CurvePoint(Vec location_, float width_) : location(location_), width(width_), color() { }
CurvePoint::CurvePoint(float x, float y, float width_) : location(x, y), width(width_), color() { }
CurvePoint::CurvePoint(Vec location_, float width_, RGB color_, RGB fill_) : location(location_), width(width_), color(color_), fill(fill_) { }
CurvePoint::CurvePoint(float x, float y, float width_, RGB color_, RGB fill_) : location(x, y), width(width_), color(color_), fill(fill_) { }

CurvePoint CurvePoint::interpolate(CurvePoint other, float amount) {
    CurvePoint point;
    point.location = location + (other.location - location) * amount;
    point.width = width + (other.width - width) * amount;
    point.color = color.interpolate(other.color, amount);
    point.fill = fill.interpolate(other.fill, amount);
    return point;
}

void Curve::line(Graphics g) const {
    g.begin_line_strip();
    
    for(const CurvePoint& p : *this) {
        g.point(p.location);
    }
    
    g.end();
}

void Curve::stroke(Graphics g) const {
    if(size() < 2) return;
    
    g.begin_quad_strip();
    
    {
        Vec delta = at(1).location - at(0).location;
        Vec orth = delta.ortho().scaled(at(0).width * 0.5f);
        
        g.rgb(at(0).color);
        
        g.point(at(0).location + orth);
        g.point(at(0).location - orth);
    }
    
    /* "Final index" - last point indexed; also happens to be number at far range of for loop */
    size_t fi = size() - 1;
    
    for(size_t i = 1; i < fi; ++i) {
        CurvePoint next = at(i + 1);
        CurvePoint previous = at(i - 1);
        
        size_t j = i + 1;
        
        Vec delta, orth, bi, proj;
        
        while((next.location - at(i).location).lensqr() < 0.1f) {
            if(j == fi) goto next_loop;
            next = at(j);
            ++j;
        }
        
        j = i - 1;
        
        while((previous.location - at(i).location).lensqr() < 0.1f) {
            if(j == 0) goto next_loop;
            next = at(j);
            --j;
        }
        
        delta = at(i).location - previous.location;
        
        orth = delta.ortho().scaled(at(i).width * 0.5f); 
        bi = delta.bisector(at(i).location - next.location);
        
        proj = bi * orth.lensqr() / orth.dot(bi);
        
        g.rgb(at(i).color);
        
        g.point(at(i).location + proj);
        g.point(at(i).location - proj);
        
        next_loop: { }
    }
    
    {
        Vec delta = at(fi).location - at(fi - 1).location;
        Vec orth = delta.ortho().scaled(at(fi).width * 0.5f);
        
        g.rgb(at(fi).color);
        
        g.point(at(fi).location + orth);
        g.point(at(fi).location - orth);
    }
    
    g.end();
}

void Curve::fill(Graphics g) const {
    if(size() < 3) return;
    
    g.begin_triangle_fan();
    
    for(size_t i = 0; i < size(); ++i) {
        g.point(at(i).location);
    }
    
    g.end();
}