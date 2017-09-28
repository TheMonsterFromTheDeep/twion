#include "control.h"
#include "cubic.h"
#include "editable.h"
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
	Vec dot;
	
	float zoom_exp;
	float zoom_amount;
    
    Vec pan;
    bool panning;
    
    Vec getCursor();
    
    std::vector<Editable*> world;
    std::vector<Editable*> selected;
    
    bool is_editing;
};