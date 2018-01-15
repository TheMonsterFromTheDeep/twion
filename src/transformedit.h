#ifndef TRANSFORM_EDIT_H
#define TRANSFORM_EDIT_H

#include <cstddef>
#include "editor.h"
#include "transform.h"
#include <functional>

/* Actions implemented by general TransformEditor 
 * Honestly this should probably be an enum again
 * (along with SELECT_...)
 */
#define ACTION_NONE   0
#define ACTION_GRAB   1
#define ACTION_ROTATE 2
#define ACTION_SCALE  3

#define ACTION_GRAB_CORRECTION 4

#define SELECT_NONE 0
#define SELECT_ONE  1
#define SELECT_SOME 2

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

    virtual void mouse_move(Vec,Vec);
    
    RGB edit_color;
protected:
	virtual void do_select_pass(SelectableAction);
	virtual void do_transform_pass(TransformAction);
	virtual std::size_t children_count();
	virtual Vec get_pivot();
protected:   
    bool constrain_x;
    bool constrain_y;
    
	int action;
    int selection;
    
    void all_select();
    void select(Vec);
    void shift_select(Vec);

    void confirm();
    void cancel();
    
    void init_action(int,Vec);

	void do_grab_update(Vec);
	void do_rotation_update(Vec);
	void do_scale_update(Vec);
    
    Vec action_center;
    Vec action_pivot;
    
    /* TODO: Get this from parent, as we shouldn't need to store it. */
    Vec mouse_last;
};

#endif