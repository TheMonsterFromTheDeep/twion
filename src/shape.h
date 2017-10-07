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
    void toggle_loop();
    
    void add(CurvePoint,Vec,Vec);
    void draw(Graphics);
    void line(Graphics);
    
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
    void key(KeyEvent,Vec);
    void mouse_move(Vec,Vec);
    void mouse(MouseEvent,Vec);
    
    RGB edit_color;
private:
    enum EditState {
        NONE,
        GRAB,
        ROTATE,
        SCALE,
        THICKEN,
        GRAB_CORRECTION,
        GRAB_ALONG
    };
    
    enum SelectState {
        ZERO,
        ONE,
        SOME
    };
    
    bool constrain_x;
    bool constrain_y;
    
    EditState state;
    SelectState select_state;
    
    void all_select();
    void select(Vec);
    void shift_select(Vec);
    void confirm();
    void cancel();
    
    void generate();
    
    void split();
    void extrude();
    
    void init_action();
    
    std::vector<EditCurvePoint> curvepoints;
    std::vector<EditVec> vecs;
    
    std::vector<CurvePoint> curvepoints_tfrm;
    std::vector<Vec> vecs_tfrm;
    
    Vec action_center;
    Vec action_pivot;
    
    /* TODO: Get this from parent, as we shouldn't need to store it. */
    Vec mouse_last;
};

#endif