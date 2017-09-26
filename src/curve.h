#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include "vector.h"

class CurvePoint {
public:
    Vec location;
    float width;
    
    CurvePoint(Vec,float);
    CurvePoint(float,float,float);
};

class Curve : public std::vector<CurvePoint> { };

#endif