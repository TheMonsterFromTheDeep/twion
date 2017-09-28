#include "curve.h"
#include <cstdlib>
#include <iostream>

CurvePoint::CurvePoint() : location(), width(), color() { }
CurvePoint::CurvePoint(const CurvePoint& c) : location(c.location), width(c.width), color(c.color) { }
CurvePoint::CurvePoint(Vec location_, float width_) : location(location_), width(width_), color() { }
CurvePoint::CurvePoint(float x, float y, float width_) : location(x, y), width(width_), color() { }
CurvePoint::CurvePoint(Vec location_, float width_, RGB color_) : location(location_), width(width_), color(color_) { }
CurvePoint::CurvePoint(float x, float y, float width_, RGB color_) : location(x, y), width(width_), color(color_) { }

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