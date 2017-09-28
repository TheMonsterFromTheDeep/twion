#ifndef SHAPE_H
#define SHAPE_H

#include "vector.h"
#include "cubic.h"
#include "curve.h"
#include "graphics.h"
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

class ShapeEditor {
public:
    ShapeEditor(Shape*);

    Shape *source;
    
    void draw(Graphics);
};

#endif