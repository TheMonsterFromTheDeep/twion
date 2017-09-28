#ifndef CONTROL_H
#define CONTROL_H

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
	virtual void size(Control&,int,int);
};

class ScaleSizer : public Sizer {
public:
	ScaleSizer();
	ScaleSizer(float,float);

	float x, y;
	virtual void size(Control&,int,int);
};

class Control : public EventSource {
public:
	int x, y, width, height;
	RGB background;
	
	Sizer* sizer;
	
	Control(int,int,int,int,RGB bg=RGB(0.8f));
	
	void render(Graphics);
	
	virtual void mouse_move(Vec,Vec);
	virtual void scroll(Vec);
    virtual void mouse_button(MouseEvent);
    virtual void key(char);
    
    virtual Vec getMouse();
    
    EventSource *parent;
protected:
	virtual void draw(Graphics);
private:
    
};

#endif