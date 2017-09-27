#include "window.h"
#include "editor.h"

int main() {
	Window w(800, 600, "Twion | Untitled.twi");
	
	Editor editor(0, 0, 400, 400);
	editor.sizer = new ScaleSizer(1, 1);
	
	w.attach(&editor);
	
	w.open();
	
	
	
	return 0;
}