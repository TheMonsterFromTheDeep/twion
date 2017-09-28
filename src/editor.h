#include "control.h"
#include "cubic.h"
#include "shape.h"
#include <vector>

class Editor : public Control {
public:
	Editor(int,int,int,int);

	virtual void mouse_move(Vec,Vec);
	virtual void scroll(Vec);
    virtual void mouse_button(MouseEvent);
    virtual void key(char);
protected:
	virtual void draw(Graphics);
private:
	float zoom_exp;
	float zoom_amount;
    
    Vec pan;
    bool panning;
    
    Vec getCursor();
    
    Shape test;
};