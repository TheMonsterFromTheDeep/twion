#include "editor.h"
#include "curve.h"

#include "shape.h"
#include "objecteditor.h"

#include <iostream>
#include <cmath>

EditorWindow::EditorWindow() : Control(0,0,100,100,RGB(0.45, 0.45, 0.45)),
    left(), right(), container(0, 0, 100, 100), left_view(0, 0, 1, 1, RGB(), 0.3f), right_view(0, 0, 1, 1, RGB(), 0.3f) {
        left_view.attach(&left);
        left_view.attach(&container);
        left.sizer = &left_view;
        container.sizer = &left_view;
        right_view.attach(&left_view);
        right_view.attach(&right);
        left_view.sizer = &right_view;
        right.sizer = &right_view;
        attach(&right_view);
        right_view.sizer = new ScaleSizer(1.f, 1.f);
        
        container.window = this;
}

EditorMenu::EditorMenu() : Control(0, 0, 100, 100, RGB(0.6, 0.6, 0.6)) { }

void EditorMenu::draw(Graphics g) {
}

void Editor::draw(Graphics g) { }
void Editor::key(KeyEvent e,Vec v) { }
void Editor::mouse_move(Vec v,Vec d) { }
void Editor::mouse(MouseEvent e,Vec v) { }
void Editor::init_left_menu(Control& menu) { }
void Editor::init_right_menu(Control& menu) { }

void Editor::parent(EditorContainer *e) {
    container = e;
}

void Editor::change_editor(Editor *e) {
    container->set_editor(e);
}

EditorContainer::EditorContainer(int x, int y, int w, int h) :
	Control(x, y, w, h, RGB(0.45f, 0.45f, 0.45f)),
    zoom_exp(0), zoom_amount(1), panning(false), editor(NULL) {
       // test.add(CurvePoint(100, 100, 1, RGB(0, 1, 0)), Vec(0 - 10, 0 - 10), Vec(0 + 10, 0 + 10));
       // test.add(CurvePoint(200, 100, 5, RGB(0, 0, 1)), Vec(100 - 10, 0 + 10), Vec(100 + 10, 0 - 10));
        
      //  editor = test.get_editor();
        set_editor(get_object_editor());
    }
    
ObjectEditor *EditorContainer::get_object_editor() {
    return new ObjectEditor(&shapes);
}
    
void EditorContainer::set_editor(Editor *e) {
    if(e) {        
        editor = e;
        e->parent(this);
        window->left.reinitialize();
        e->init_left_menu(window->left);
        window->right.reinitialize();
        e->init_right_menu(window->right);
        invalidate();
    }
}
	
Vec EditorContainer::getCursor() {
	return (getMouse() - pan) / zoom_amount;
}

bool EditorContainer::key(KeyEvent ke) {    
    if(ke.action == PRESS) {
        if(editor) {
            editor->key(ke, getCursor());
        }
    }
    
    return false;
}
    
bool EditorContainer::mouse_move(Vec position, Vec delta) {
    if(panning) {
        pan += delta;
    }
    
    Vec cursor = getCursor();
    
    if(editor) {
        editor->mouse_move(cursor, delta / zoom_amount);
    }
    
    return false;
}

bool EditorContainer::scroll(Vec v) {
	if(v.y != 0) {
        float old_amount = zoom_amount;
        
        zoom_exp += v.y;
        zoom_amount = pow(1.1, zoom_exp);
        
        old_amount = zoom_amount / old_amount;
        
        Vec m = getMouse();
        
        pan -= m;
        pan *= old_amount;
        pan += m;
    }
    
    return false;
}

bool EditorContainer::mouse_button(MouseEvent e) {
    if(e.button == MIDDLE) {
        if(e.action == PRESS) {
            panning = true;
        }
        if(e.action == RELEASE) {
            panning = false;
        }
    }
    if(e.button == LEFT) {
        if(e.action == PRESS) {
        }
    }
    if(e.button == RIGHT) {
        if(e.action == PRESS) {
        }
    }
    
    if(editor) {
        editor->mouse(e, getCursor());
    }
    
    return false;
}

void EditorContainer::draw(Graphics g) {
    g.translate(pan);
    g.scale(zoom_amount);
    
    if(editor) {
        editor->draw(g);
    }
}