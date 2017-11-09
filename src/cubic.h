#ifndef CUBIC_H
#define CUBIC_H

#include "curve.h"

class InterpolatedCubic {
public:
    CurvePoint *start;
    CurvePoint *end;
    
    Vec *ease_in;
    Vec *ease_out;
    
    InterpolatedCubic(CurvePoint*,CurvePoint*,Vec*,Vec*);
    
    void calculate();
    
    CurvePoint evaluate(float);
    
    Vec eval_ease_delta(float);
    
    Curve generate(float,Vec offset=Vec());
private:
    Vec a, b, c, d;
};

#endif