#ifndef EDITORTYPE_H
#define EDITORTYPE_H

#include "control.h"

class EditorContainer;

class Editor {
public:
    virtual void draw(Graphics);
    virtual void key(KeyEvent,Vec);
    virtual void mouse_move(Vec,Vec);
    virtual void mouse(MouseEvent,Vec);
    
    void parent(EditorContainer *c);
    virtual void init_left_menu(Control&);
    virtual void init_right_menu(Control&);
protected:
    void change_editor(Editor*);
    EditorContainer *container;
};

#endif