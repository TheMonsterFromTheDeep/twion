#include "colorselector.h"
#include <cmath>

#include <iostream>

ColorSelector::ColorSelector(RGB *target_) :
	Control(0, 0, 0, 0, RGB(0.67f, 0.67f, 0.67f)),
    hsv_wheel("hsv.png"), editing_color(false), color_position(0, 0), target(target_) { }

void ColorSelector::key(KeyEvent ke) {
}
    
#define RAD_TO_DEG 57.2957795131
    
void ColorSelector::mouse_move(Vec position, Vec delta) {
    if(editing_color) {
        int size = width - 10;
        Vec center(5 + (size / 2));
        color_position = (position - center) / (size / 2);
        if(color_position.lensqr() > 1) color_position.normalize();
        
        if(target) {
            float hue = color_position.angle(Vec());
            hue *= RAD_TO_DEG;
            hue += 180;
            hue = fmod(hue, 360);
            
            *target = RGB::fromHSV(hue, color_position.len(), 1);
        }
    }
}


void ColorSelector::mouse_button(MouseEvent e) {
    if(e.button == LEFT) {
        int size = width - 10;
        Vec center(5 + (size / 2));
        editing_color = (e.action == PRESS) && (e.position - center).lensqr() <= size * size;
    }
}

void ColorSelector::draw(Graphics g) {
    int size = width - 10;
    
    g.rgb(1.f, 1.f, 1.f);
    g.draw_image(hsv_wheel, 10, 10, size, size);
    
    Vec center(5 + (size / 2));
    
    g.rgb(0.f, 0.f, 0.f);
    g.draw_circle(center + color_position * (size / 2), 5);
}