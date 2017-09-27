#include "control.h"

class Editor : public Control {
public:
	Editor(int,int,int,int);

	virtual void mouse_move(Vec,Vec);
	virtual void scroll(Vec);
    virtual void mouse_button(MouseEvent);
protected:
	virtual void draw(Graphics) const;
private:
	Vec dot;
	
	float zoom_exp;
	float zoom_amount;
    
    Vec pan;
    bool panning;
};