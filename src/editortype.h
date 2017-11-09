#ifndef EDITORTYPE_H
#define EDITORTYPE_H

class EditorContainer;

class Editor {
public:
    virtual void draw(Graphics);
    virtual void key(KeyEvent,Vec);
    virtual void mouse_move(Vec,Vec);
    virtual void mouse(MouseEvent,Vec);
    
    void parent(EditorContainer *c);
protected:
    void change_editor(Editor*);
    EditorContainer *container;
};

#endif