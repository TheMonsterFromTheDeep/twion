#include "control.h"
#include "cubic.h"
#include "shape.h"
#include "editortype.h"
#include "view.h"
#include <vector>

class ObjectEditor;

class EditorWindow;

class EditorMenu : public Control {
public:
    virtual void draw(Graphics);
    EditorMenu();
};

class EditorContainer : public Control {
public:
	EditorContainer(int,int,int,int);

	virtual bool mouse_move(Vec,Vec);
	virtual bool scroll(Vec);
    virtual bool mouse_button(MouseEvent);
    virtual bool key(KeyEvent);
    
    Editor *editor;
    EditorWindow *window;
    
    void set_editor(Editor*);
	void init_editor();
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

class EditorWindow : public Control {
public:
    EditorMenu left;
    EditorMenu right;
    EditorContainer container;
    
    EditorWindow();
private:
    View left_view;
    View right_view;
};