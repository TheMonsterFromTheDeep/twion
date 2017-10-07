#include "window.h"
#include "editor.h"
#include "view.h"
#include "colorselector.h"
#include "cursors.h"

int main() {
    init_cursors();
    set_cursor(CURSOR_DEFAULT);
    
	Window w(800, 600, "Twion | Untitled.twi");
	
    View v(0, 0, 1, 1, RGB(1.f, 0.1f, 0.1f));
    v.sizer = new ScaleSizer(1, 1);
    
	Editor editor(0, 0, 400, 400);
	editor.sizer = &v;
	
    ColorSelector cs(&editor.editor->edit_color);
    cs.sizer = &v;
    
    v.attach(&editor);
    v.attach(&cs);
    
    w.attach(&v);
	
    set_cursor(CURSOR_DEFAULT);
    
	w.open();
	
	return 0;
}