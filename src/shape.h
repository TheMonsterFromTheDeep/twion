#ifndef SHAPE_H
#define SHAPE_H

#include "vector.h"
#include "cubic.h"
#include "curve.h"
#include "graphics.h"
#include <vector>

class Shape {
public:
    Shape();

    void add(CurvePoint,Vec,Vec);
    void draw(Graphics);
private:
    std::vector<CurvePoint> points;
    std::vector<Vec> handles;
    std::vector<InterpolatedCubic> curves;
};

#endif