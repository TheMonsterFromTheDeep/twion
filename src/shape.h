#ifndef SHAPE_H
#define SHAPE_H

#include "vector.h"
#include "cubic.h"
#include "curve.h"
#include "graphics.h"
#include "event.h"
#include "editortype.h"
#include <vector>

class ShapeEditor;

class ShapePoint;

/* TODO: Tighten this and InterpolatedCubic; they're almost the exact
 * same thing */
class ShapeLine {
public:
    ShapePoint *start;
	ShapePoint *end;
	
	Vec start_handle;
	Vec end_handle;
	
	Vec start_ease();
	Vec end_ease();
	
	void draw_handles(Graphics);
};

class ShapePoint {
public:
    CurvePoint data;
	
	ShapePoint(const CurvePoint&);
	
	void draw(Graphics);
	std::vector<ShapeLine*> lines;
};

class Shape {
public:
    Vec position;

    Shape();
    
    void draw(Graphics);
    void line(Graphics);
	
	ShapePoint* add_point(const CurvePoint&);
	ShapeLine* add_line(const CurvePoint&, const CurvePoint&, Vec ae=Vec(), Vec be=Vec());
	void connect(ShapePoint*,ShapePoint*);
    
    ShapeEditor *get_editor();
private:
    std::vector<ShapePoint*> points;
	std::vector<ShapeLine*> lines;
    
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

class ShapeEditor : public Editor {
public:
    ShapeEditor(Shape*);

    Shape *source;
    
    virtual void draw(Graphics);
    virtual void key(KeyEvent,Vec);
    virtual void mouse_move(Vec,Vec);
    virtual void mouse(MouseEvent,Vec);
    
    virtual void init_left_menu(Control&);
    
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
    void dir_match();
    
    void init_action();
    
    Vec action_center;
    Vec action_pivot;
    
    /* TODO: Get this from parent, as we shouldn't need to store it. */
    Vec mouse_last;
};

#endif