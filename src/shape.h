#ifndef SHAPE_H
#define SHAPE_H

#include "vector.h"
#include "curve.h"
#include <vector>

class Shape {
public:
    void draw(Graphics);
private:
    std::vector<CurvePoint> points;
    std::vector<InterpolatedCubic> curves;
}

#endif