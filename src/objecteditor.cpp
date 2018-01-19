#include "objecteditor.h"
#include <iostream>

ObjectEditor::ObjectEditor(std::vector<shape::Shape*> *source_) : source(source_) { }

void ObjectEditor::do_transform_pass(TransformAction f) {
	for (shape::Shape *s : *source) {
		if (f(*s)) break;
	}
}

void ObjectEditor::do_select_pass(SelectableAction f) {
	for (shape::Shape *s : *source) {
		if (f(*s)) break;
	}
}

void ObjectEditor::draw(Graphics g) {
    for(shape::Shape* c : *source) {
		c->draw(g);
    } 
}

std::size_t ObjectEditor::children_count() {
	return source->size();
}

Vec ObjectEditor::get_pivot() {
	Vec pivot;
	for (shape::Shape* c : *source) {
		/* Add transform applied to origin to pivot */
		pivot += c->get_transform() * Vec();
	}
	/* Center pivot */
	pivot /= source->size();
	return pivot;
}

void ObjectEditor::key(KeyEvent e, Vec mouse) {
	if (e.key == 'A') {
		if (e.shift_down) {
			source->push_back(shape::Shape::square(50));
		}
		else all_select();
	}
    
    if(e.key == 258) {
		for (shape::Shape* c : *source) {
			if (c->is_selected()) {
				change_editor(c->get_editor());
				break;
			}
		}
    }

	if (selection != SELECT_NONE && action == ACTION_NONE) {
		if (e.key == 'G') {
			init_action((e.shift_down ? ACTION_GRAB_CORRECTION : ACTION_GRAB), mouse);
		}

		if (e.key == 'R') {
			init_action(ACTION_ROTATE, mouse);
		}

		if (e.key == 'S') {
			/* TODO: Make init_action poll mouse position */
			init_action(ACTION_SCALE, mouse);
		}
	}

	//if (e.key == 261) {
	//	remove();
	//}

	if (action == ACTION_GRAB || action == ACTION_SCALE) {
		if (e.key == 'X') {
			constrain_x = !constrain_x;
			constrain_y = constrain_y && !constrain_x;
		}

		if (e.key == 'Y') {
			constrain_y = !constrain_y;
			constrain_x = constrain_x && !constrain_y;
		}
	}
}

void ObjectEditor::mouse(MouseEvent m, Vec pos) {
	if (m.button == RIGHT && m.action == PRESS) {
		if (action == ACTION_NONE) {
			if (m.shift_down) shift_select(pos);
			else select(pos);
		}
		else cancel();
	}

	if (m.button == LEFT && m.action == PRESS) {
		if (action != ACTION_NONE) confirm();
	}
}