#include "colorselector.h"
#include <cmath>

#include <iostream>

ColorSelector::ColorSelector(RGB *target_) :
	Control(0, 0, 0, 0, RGB(0.65f, 0.65f, 0.65f)),
    hsv_wheel("hsv.png"), value_bar("value.png"), editing_color(false), editing_value(false), value(1), color_position(0, 0), target(target_) { }

bool ColorSelector::key(KeyEvent ke) {
    return false;
}
    
#define RAD_TO_DEG 57.2957795131
    
bool ColorSelector::mouse_move(Vec position, Vec delta) {
    int size = width - 10;
    Vec center(5 + (size / 2));
    
    if(editing_color) {
        color_position = (position - center) / (size / 2);
        if(color_position.lensqr() > 1) color_position.normalize();
    }
    
    if(editing_value) {
        value = (position.x - 10) / (size - 10);
        if(value < 0) value = 0;
        if(value > 1) value = 1;
    }
    
    if(target) {
        float hue = color_position.angle(Vec());
        hue *= RAD_TO_DEG;
        hue += 180;
        hue = fmod(hue, 360);
        
        *target = RGB::fromHSV(hue, color_position.len(), value);
    }
    
    return false;
}


bool ColorSelector::mouse_button(MouseEvent e) {
    if(e.button == LEFT) {
        int size = width - 10;
        Vec center(5 + (size / 2));
        editing_color = (e.action == PRESS) && (e.position - center).lensqr() <= (size / 2) * (size / 2);
        
        int value_height = (size - 10) * (64 / 512.f);
        editing_value = (e.action == PRESS) && (e.position.y > 10 + size && e.position.y < 10 + size + value_height);
    }
    
    return false;
}

void ColorSelector::draw(Graphics g) {    
    int size = width - 10;
    
    g.rgb(value, value, value);
    g.draw_image(hsv_wheel, 10, 10, size, size);
    
    g.rgb(1.f, 1.f, 1.f);
    int value_height = (size - 10) * (64 / 512.f);
    g.draw_image(value_bar, 10, 10 + size, size, 10 + size + value_height);
    
    Vec center(5 + (size / 2));
    
    value_height /= 2;
    
    g.rgb(0.f, 0.f, 0.f);
    g.draw_circle(center + color_position * (size / 2), value_height);
    g.draw_circle(10 + (size - 10) * value, 10 + size + value_height, value_height);
    
    
    g.rgb(1.f, 1.f, 1.f);
    g.draw_circle(center + color_position * (size / 2), value_height - 1);
    g.draw_circle(10 + (size - 10) * value, 10 + size + value_height, value_height - 1);
}