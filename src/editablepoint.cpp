#include "editablepoint.h"

#include <iostream>

EditablePoint::EditablePoint(CurvePoint c) : point(c), grab(false), grab_point(c), widen_center(c) { }

bool EditablePoint::select(Vec v) {
    return (v - point.location).lensqr() < 5 * 5;
}

void EditablePoint::cancel() {
    point = grab_point;
    point.width = original_width;
    
    grab = false;
    widen = false;
}

void EditablePoint::confirm() {
    grab = false;
    widen = false;
}

void EditablePoint::mouse(Vec where, Vec delta) {
    if(grab) point.location += delta;
    if(widen) point.width = (where - widen_center.location).len();
}

bool EditablePoint::key(char which) {
    if(which == 'G') {        
        grab = true;
        grab_point = point;
        return true;
    }
    if(which == 'A') {
        original_width = point.width;
        widen = true;
        widen_center = point;
    }
    return false;
}