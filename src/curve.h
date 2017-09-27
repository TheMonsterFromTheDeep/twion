#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include "vector.h"

#include "graphics.h"

class CurvePoint {
public:
    Vec location;
    float width;
    
    CurvePoint(Vec,float);
    CurvePoint(float,float,float);
};

class Curve : public std::vector<CurvePoint> {
public:
    void stroke(Graphics) const;
};

#endif