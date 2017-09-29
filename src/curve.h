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
    
    CurvePoint();
    CurvePoint(const CurvePoint&);
    CurvePoint(Vec,float);
    CurvePoint(float,float,float);
    CurvePoint(Vec,float,RGB);
    CurvePoint(float,float,float,RGB);
};

class Curve : public std::vector<CurvePoint> {
public:
    void stroke(Graphics) const;
    void fill(Graphics) const;
};

#endif