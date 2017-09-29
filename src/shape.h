#ifndef SHAPE_H
#define SHAPE_H

#include "vector.h"
#include "cubic.h"
#include "curve.h"
#include "graphics.h"
#include "event.h"
#include <vector>

class ShapeEditor;

class Shape {
public:
    Shape();

    void loop();
    void unloop();
    
    void add(CurvePoint,Vec,Vec);
    void draw(Graphics);
    
    ShapeEditor *get_editor();
private:
    std::vector<CurvePoint> points;
    std::vector<Vec> handles;
    std::vector<InterpolatedCubic> curves;
    
    void generate();
    
    bool looped;
    
    friend class ShapeEditor;
};

class EditCurvePoint {
public:
    EditCurvePoint(CurvePoint*);

    CurvePoint *source;
    bool selected;
};

class EditVec {
public:
    EditVec(Vec*);

    Vec *source;
    bool selected;
};

class ShapeEditor {
public:
    ShapeEditor(Shape*);

    Shape *source;
    
    void draw(Graphics);
    void key(char,Vec);
    void mouse_move(Vec,Vec);
    void mouse(MouseEvent,Vec);
private:
    enum EditState {
        NONE,
        GRAB,
        ROTATE,
        SCALE
    };
    
    enum SelectState {
        ZERO,
        ONE,
        SOME
    };
    
    EditState state;
    SelectState select_state;
    
    
    void select(Vec);
    void shift_select(Vec);
    void confirm();
    void cancel();
    
    std::vector<EditCurvePoint> curvepoints;
    std::vector<EditVec> vecs;
    
    Vec action_center;
    Vec action_delta;
};

#endif