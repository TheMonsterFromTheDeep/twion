#ifndef CONTROL_H
#define CONTROL_H

#include <vector>

#include "graphics.h"
#include "vector.h"
#include "event.h"

class EventSource {
public:
    virtual Vec getMouse()=0;
};

class Control;

class Sizer {
public:
	virtual void size(Control*,int,int);
};

class ScaleSizer : public Sizer {
public:
	ScaleSizer();
	ScaleSizer(float,float,float ox_=0,float oy_=0);

	float x, y;
    float ox, oy;
	virtual void size(Control*,int,int);
};

class Control : public EventSource {
public:
	int x, y, width, height;
	RGB background;
	
	Sizer* sizer;
	
	Control(int,int,int,int,RGB bg=RGB(0.8f));
	
	void render(Graphics);
	
	virtual bool mouse_move(Vec,Vec);
	virtual bool scroll(Vec);
    virtual bool mouse_button(MouseEvent);
    virtual bool key(KeyEvent);
    
    void mouse_move_(Vec, Vec);
    void scroll_(Vec);
    void mouse_button_(MouseEvent);
    void key_(KeyEvent);
    
    virtual Vec getMouse();
    
    void attach(Control*);
    void reinitialize();
    
    void invalidate();
    
    EventSource *parent;
    
    void size(int,int);
protected:
	virtual void draw(Graphics);
    std::vector<Control*> children;
};

#endif