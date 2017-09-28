#include "shape.h"

ShapeEditor::ShapeEditor(Shape* source_) : source(source_) { }

void ShapeEditor::draw(Graphics g) {
    source->draw(g);
    
    g.rgb(1, 0, 0);
    
    for(InterpolatedCubic &ic : source->curves) {
        g.draw_circle(ic.start->location, 5);
        g.draw_circle(ic.end->location, 5);
    }
}