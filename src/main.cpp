#include "window.h"
#include "editor.h"
#include "view.h"
#include "colorselector.h"

int main() {
	Window w(800, 600, "Twion | Untitled.twi");
	
    View v(0, 0, 1, 1, RGB(1.f, 0.1f, 0.1f));
    v.sizer = new ScaleSizer(1, 1);
    
	Editor editor(0, 0, 400, 400);
	editor.sizer = &v; //new ScaleSizer(0.8f, 1);
	
    ColorSelector cs(&editor.editor->edit_color);
    cs.sizer = &v; //new ScaleSizer(0.2f, 1, 0.8f);
    
    v.attach(&editor);
    v.attach(&cs);
    
    w.attach(&v);
	
	w.open();
	
	return 0;
}