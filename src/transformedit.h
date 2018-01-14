#ifndef TRANSFORM_EDIT_H
#define TRANSFORM_EDIT_H

#include "editor.h"
#include "transform.h"
#include <functional>

/* Actions implemented by general TransformEditor */
#define ACTION_NONE   0
#define ACTION_GRAB   1
#define ACTION_ROTATE 2
#define ACTION_SCALE  3

class Selectable {
public:
	Selectable();

	void select();
	void deselect();
	void set_selected(bool);
	bool is_selected();
	virtual bool should_select(Vec);
protected:
	virtual void on_selected();
	virtual void on_deselected();
private:
	bool selected;
};

class Transformable : public Selectable {
public:
	Transformable();

	void set_transform(const Transform&);
	const Transform& get_transform();
	void apply();
	void cancel();
protected:
	virtual void on_transform_changed();
	virtual void on_transform_applied();
	virtual void on_transform_canceled();
private:
	Transform transform;
};

typedef std::function<bool(Transformable&)> TransformAction;
typedef std::function<bool(Selectable&)> SelectableAction;

class TransformEditor : public Editor {
public:
    TransformEditor();
    
    virtual void draw(Graphics);
    virtual void key(KeyEvent,Vec);
    virtual void mouse_move(Vec,Vec);
    virtual void mouse(MouseEvent,Vec);
    
    virtual void init_left_menu(Control&);
    
    RGB edit_color;
protected:
	virtual void do_select_pass(SelectableAction);
	virtual void do_transform_pass(TransformAction);
	virtual size_t children_count();
private:
    
    enum SelectState {
        ZERO,
        ONE,
        SOME
    };
    
    bool constrain_x;
    bool constrain_y;
    
	int action;
    SelectState select_state;
    
    void all_select();
    void select(Vec);
    void shift_select(Vec);

    void confirm();
    void cancel();
    
    void generate();
    
    void split();
    void extrude();
    void dir_match();
    
    void init_action(int,Vec);
    
    Vec action_center;
    Vec action_pivot;
    
    /* TODO: Get this from parent, as we shouldn't need to store it. */
    Vec mouse_last;
};

#endif