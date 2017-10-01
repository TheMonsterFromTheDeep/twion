#include "colorselector.h"

ColorSelector::ColorSelector() :
	Control(0, 0, 0, 0, RGB(0.67f, 0.67f, 0.67f)), hsv_wheel("hsv.png") {
        
    }

void ColorSelector::key(KeyEvent ke) {
}
    
void ColorSelector::mouse_move(Vec position, Vec delta) {
}

void ColorSelector::scroll(Vec v) {
}

void ColorSelector::mouse_button(MouseEvent e) {
}

void ColorSelector::draw(Graphics g) {
    int size = width - 10;
    
    g.rgb(1.f, 1.f, 1.f);
    g.draw_image(hsv_wheel, 10, height - 10, size, height - size);
}