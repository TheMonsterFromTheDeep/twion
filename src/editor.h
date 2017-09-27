#include "control.h"
#include "cubic.h"
#include <vector>

class Editor : public Control {
public:
	Editor(int,int,int,int);

	virtual void mouse_move(Vec,Vec);
	virtual void scroll(Vec);
    virtual void mouse_button(MouseEvent);
protected:
	virtual void draw(Graphics);
private:
	Vec dot;
	
	float zoom_exp;
	float zoom_amount;
    
    Vec pan;
    bool panning;
    
    Vec getCursor();
    
    std::vector<InterpolatedCubic> cubics;
    
    InterpolatedCubic *current;
    int edit_mode;
};