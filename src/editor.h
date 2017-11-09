#include "control.h"
#include "cubic.h"
#include "shape.h"
#include "editortype.h"
#include <vector>

class ObjectEditor;

class EditorContainer : public Control {
public:
	EditorContainer(int,int,int,int);

	virtual bool mouse_move(Vec,Vec);
	virtual bool scroll(Vec);
    virtual bool mouse_button(MouseEvent);
    virtual bool key(KeyEvent);
    
    Editor *editor;
    
    void set_editor(Editor*);
    ObjectEditor *get_object_editor();
protected:
	virtual void draw(Graphics);
private:
	float zoom_exp;
	float zoom_amount;
    
    Vec pan;
    bool panning;
    
    Vec getCursor();
    
    std::vector<Shape> shapes;
    
};