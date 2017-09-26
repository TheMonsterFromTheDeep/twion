#include "cubic.h"

InterpolatedCubic::InterpolatedCubic(CurvePoint start_, CurvePoint end_, Vec ease_in_, Vec ease_out_) :
    start(start_), end(end_), ease_in(ease_in_), ease_out(ease_out_) { }
    
#define EASE_FACTOR 3
    
void InterpolatedCubic::calculate() {
    d = start.location;
    c = ease_in * EASE_FACTOR;
    b = (end.location - start.location) * 3 - (ease_in * EASE_FACTOR) * 2 + (ease_out * EASE_FACTOR);
    a = end.location - start.location - (ease_in * EASE_FACTOR) - b;
}

CurvePoint InterpolatedCubic::evaluate(float t) {
    return CurvePoint(a * t * t * t + b * t * t + c * t + d, start.width + (end.width - start.width) * t);
}
    
Curve InterpolatedCubic::generate(float resolution) {
    Curve out;
    
    for(float t = 0; t < 1; t += resolution) {
        out.push_back(evaluate(t));
    }
    
    return out;
}