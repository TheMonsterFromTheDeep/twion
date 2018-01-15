#include "transformedit.h"

Selectable::Selectable() : selected(false) { }

void Selectable::select() {
	selected = true;
	on_selected();
}

void Selectable::deselect() {
	selected = false;
	on_deselected();
}

void Selectable::set_selected(bool what) {
	if (what) select(); else deselect();
}

void Selectable::on_selected() { }
void Selectable::on_deselected() { }

bool Selectable::is_selected() { return selected; }

bool Selectable::should_select(Vec mouse) { return false; }

Transformable::Transformable() : transform() { }

void Transformable::set_transform(const Transform& t) {
	transform = t;
	on_transform_changed();
}
const Transform& Transformable::get_transform() { return transform; }
void Transformable::apply() {
	on_transform_applied();
	/* No need to call transform changed */
	transform = Transform();
}

void Transformable::cancel() {
	on_transform_canceled();
	/* No need to call transform changed */
	transform = Transform();
}

void Transformable::on_transform_changed() { }
void Transformable::on_transform_applied() { }
void Transformable::on_transform_canceled() { }

TransformEditor::TransformEditor() : constrain_x(false), constrain_y(false), action(ACTION_NONE), selection(SELECT_NONE) { }

/* Default handling */
void TransformEditor::do_select_pass(SelectableAction f) { }
void TransformEditor::do_transform_pass(TransformAction f) { }
size_t TransformEditor::children_count() { return 0; }
Vec TransformEditor::get_pivot() { return Vec(); }

void TransformEditor::all_select() {
	/* Select points if none are selected, deselect all selected
	* points if one or more is selected */
	bool new_value = (selection == SELECT_NONE);

	if(new_value) {
		/* Handle all possible "select all" scenarios */
		if(children_count() == 1) selection = SELECT_ONE;
		else if(children_count() > 1) selection = SELECT_SOME;
		else selection = SELECT_NONE;
	}
	else { selection = SELECT_NONE; }

	/* Pass the new value to all children */
	do_select_pass([=](Selectable& s) {
		s.set_selected(new_value);
		return false;
	});
}

void TransformEditor::select(Vec pos) {
	bool has_new = false;
	Selectable *new_selection;

	int new_select = SELECT_NONE;

	do_select_pass([&](Selectable& s) {
		/* Determine if there are any objects to be selected */
		if (s.should_select(pos)) {
			new_selection = &s;
			has_new = true;
			return true;
		}
		return false;
	});

	/* Update selections if something should be selected / unselected */
	if (has_new) {
		do_select_pass([&](Selectable& s) {
			/* We want to:
			* - Unselect all objects other than the new one
			* - Select the new one if it is not selected (i.e. !curvepoints[i].selected)
			* - Unselected the new one if it is selected
			* - Always select the new one if more than one is selected (otherwise, if more than one is selected,
			*   and one that is already selected is selected again, it will become un-selected).
			* This simple statement does all of that: whether a particular object is selected
			* is true if is the same as the one we stored earlier, and is false if it isn't
			* or if it was already selected.
			*/
			s.set_selected((&s == new_selection) && (!s.is_selected() || selection == SELECT_SOME));
			if (s.is_selected()) new_select = SELECT_ONE;
			return false;
		});
	}
}

void TransformEditor::shift_select(Vec pos) {
    size_t select_count = 0;

	do_select_pass([&,pos](Selectable& s) {
		/* If an object is qualifiable for selection, flip its state */
		if (s.should_select(pos)) {
			s.set_selected(!s.is_selected());
		}
		/* Keep running total of how many are selected */
		if (s.is_selected()) ++select_count;
		return false;
	});
    
	/* Update select state based on number of selected objects */
    if(select_count > 1)
		selection = SELECT_SOME;
    else if(select_count == 1)
		selection = SELECT_ONE;
    else
		selection = SELECT_NONE;
}

void TransformEditor::mouse_move(Vec position, Vec delta) {
	mouse_last = position;

    switch(action) {
        case ACTION_GRAB: {
            Vec translation_vec = position - action_center;
            if(constrain_x) { translation_vec.y = 0; }
            if(constrain_y) { translation_vec.x = 0; }

			Transform translation = Transform::translation(translation_vec);
            
			do_transform_pass([&](Transformable& t) {
				if(t.is_selected()) t.set_transform(translation);
				return false;
			});
        }
        break;
        
        case ACTION_ROTATE: {
            Vec direction = position.rotate_inverse(action_center - action_pivot, action_pivot) - action_pivot;
            
			Transform rotation = Transform::rotation(direction, action_pivot);

			do_transform_pass([&](Transformable& t) {
				if(t.is_selected()) t.set_transform(rotation);
				return false;
			});
        }
        break;
        
        case ACTION_SCALE: {
            /* This is less efficient than it could be (two square roots, action_center - action_pivot could be cached)
             * but it doesn't really matter right now.
             */
            float magnitude = (position - action_pivot).len() / (action_center - action_pivot).len();
            
            Vec scale_vec(magnitude);
            if(constrain_x) scale_vec.y = 1;
            if(constrain_y) scale_vec.x = 1;

			Transform scale = Transform::scale(scale_vec);

			do_transform_pass([&](Transformable& t) {
				if(t.is_selected()) t.set_transform(scale);
				return false;
			});
        }
        break;
        
        default: { }
    }
}

void TransformEditor::init_action(int act, Vec center) {
	action = act;
	action_center = center;

	/* Request pivot */
	action_pivot = get_pivot();

	constrain_x = constrain_y = false;
}

void TransformEditor::cancel() {
	do_transform_pass([](Transformable& t) {
		if(t.is_selected()) t.cancel();
		return false;
	});

	action = ACTION_NONE;
}

void TransformEditor::confirm() {
	do_transform_pass([](Transformable& t) {
		if(t.is_selected()) t.apply();
		return false;
	});

	action = ACTION_NONE;
}