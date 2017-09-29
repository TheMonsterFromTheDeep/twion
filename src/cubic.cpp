#include "cubic.h"

InterpolatedCubic::InterpolatedCubic(CurvePoint *start_, CurvePoint *end_, Vec *ease_in_, Vec *ease_out_) :
    start(start_), end(end_), ease_in(ease_in_), ease_out(ease_out_) { }
    
#define EASE_FACTOR 3
    
void InterpolatedCubic::calculate() {
    Vec in_delta = *ease_in - start->location;
    Vec out_delta = *ease_out - end->location;
    
    d = start->location;
    c = in_delta * EASE_FACTOR;
    b = (end->location - start->location) * 3 - (in_delta * EASE_FACTOR) * 2 + (out_delta * EASE_FACTOR);
    a = end->location - start->location - (in_delta * EASE_FACTOR) - b;
}

CurvePoint InterpolatedCubic::evaluate(float t) {
    return CurvePoint(a * t * t * t + b * t * t + c * t + d, start->width + (end->width - start->width) * t, start->color.interpolate(end->color, t));
}
    
Curve InterpolatedCubic::generate(float resolution) {
    Curve out;
    
    for(float t = 0; t < 1; t += resolution) {
        out.push_back(evaluate(t));
    }
    
    return out;
}