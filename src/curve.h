#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include "vector.h"
#include "graphics.h"

class CurvePoint {
public:
    Vec location;
    float width;
    RGB color;
    RGB fill;
    
    CurvePoint();
    CurvePoint(const CurvePoint&);
    CurvePoint(Vec,float);
    CurvePoint(float,float,float);
    CurvePoint(Vec,float,RGB,RGB fill_=RGB(1.f,1.f,1.f));
    CurvePoint(float,float,float,RGB,RGB fill_=RGB(1.f,1.f,1.f));
};

class Curve : public std::vector<CurvePoint> {
public:
    void stroke(Graphics) const;
    void fill(Graphics) const;
};

#endif