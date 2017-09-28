#ifndef EDITABLE_POINT_H
#define EDITABLE_POINT_H

#include "editable.h"
#include "curve.h"

class EditablePoint : public Editable {
public:
    EditablePoint(CurvePoint);

    bool select(Vec);
    void mouse(Vec,Vec);
    void cancel();
    void confirm();
    bool key(char);
    
    CurvePoint point;
private:
    CurvePoint grab_point;
    float original_width;
    CurvePoint widen_center;
    
    bool grab;
    bool widen;
};

#endif