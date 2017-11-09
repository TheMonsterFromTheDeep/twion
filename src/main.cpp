#include "window.h"
#include "editor.h"
#include "view.h"
#include "colorselector.h"
#include "cursors.h"
#include <iostream>

int main() {
    init_cursors();
    set_cursor(CURSOR_DEFAULT);
    
	Window w(800, 600, "Twion | Untitled.twi");
	
   // View v(0, 0, 1, 1, RGB(1.f, 0.1f, 0.1f), 0.7f);
    //v.sizer = new ScaleSizer(1, 1);
    
	EditorContainer editor(0, 0, 400, 400);
	editor.sizer = new ScaleSizer(1, 1);
	
    //ColorSelector cs(&editor.editor->edit_color);
    //cs.sizer = &v;
    
    //v.attach(&editor);
    //v.attach(&cs);
    
    //w.attach(&v);
	w.attach(&editor);
    
    //set_cursor(CURSOR_DEFAULT);
    
	w.open();
	
	return 0;
}