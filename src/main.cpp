#include "window.h"
#include "editor.h"
#include "colorselector.h"

int main() {
	Window w(800, 600, "Twion | Untitled.twi");
	
	Editor editor(0, 0, 400, 400);
	editor.sizer = new ScaleSizer(0.8f, 1);
	
    ColorSelector cs(&editor.editor->edit_color);
    cs.sizer = new ScaleSizer(0.2f, 1, 0.8f);
    
	w.attach(&editor);
    w.attach(&cs);
	
	w.open();
	
	
	
	return 0;
}