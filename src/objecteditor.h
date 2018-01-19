#ifndef OBJECT_EDITOR_H
#define OBJECT_EDITOR_H

#include <vector>
#include "shape.h"
#include "transformedit.h"

class ObjectEditor : public TransformEditor {
public:
    std::vector<shape::Shape*> *source;
    
    ObjectEditor(std::vector<shape::Shape*>*);
    
    virtual void draw(Graphics);
    virtual void key(KeyEvent,Vec);
    virtual void mouse(MouseEvent,Vec);
private:
	virtual void do_transform_pass(TransformAction);
	virtual void do_select_pass(SelectableAction);
	virtual std::size_t children_count();
	virtual Vec get_pivot();
};

#endif