#include "control.h"
#include "cubic.h"
#include "shape.h"
#include <vector>

class Editor : public Control {
public:
	Editor(int,int,int,int);

	virtual bool mouse_move(Vec,Vec);
	virtual bool scroll(Vec);
    virtual bool mouse_button(MouseEvent);
    virtual bool key(KeyEvent);
    
    /* This is where the more general object-specific editor class
     * will eventually live; for now its a test for jsut the shape
     * editor.
     */
    ShapeEditor *editor;
protected:
	virtual void draw(Graphics);
private:
	float zoom_exp;
	float zoom_amount;
    
    Vec pan;
    bool panning;
    
    Vec getCursor();
    
    /* This is where a list of all the entities will eventaully
     * live - although that will also eventually be moved away
     * further, to another part separate from the editor
     * in particular.
     */
    Shape test;
    
};