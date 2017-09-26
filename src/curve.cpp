#include "curve.h"

CurvePoint::CurvePoint(Vec location_, float width_) : location(location_), width(width_) { }
CurvePoint::CurvePoint(float x, float y, float width_) : location(x, y), width(width_) { }
