#include "transformedit.h"

Selectable::Selectable() : selected(false) { }

void Selectable::select() {
	selected = true;
	on_selected();
}

void Selectable::deselect() {
	selected = false;
	on_deselected;
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

void Transformable::on_transform_changed() { }
void Transformable::on_transform_applied() { }

void TransformEditor::all_select() {
	/* Select points if none are selected, deselect all selected
	* points if one or more is selected */
	bool new_value = (select_state == ZERO);

	if(new_value) {
		/* Handle all possible "select all" scenarios */
		if(children_count() == 1) select_state = ONE;
		else if(children_count() > 1) select_state = SOME;
		else select_state = ZERO;
	}
	else { select_state = ZERO; }

	/* Pass the new value to all children */
	do_select_pass([=](Selectable& s) {
		s.set_selected(new_value);
		return false;
	});
}

void TransformEditor::select(Vec pos) {
	bool has_new = false;
	Selectable *new_selection;

	SelectState new_state = ZERO;

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
			s.set_selected((&s == new_selection) && (!s.is_selected() || select_state == SOME));
			if (s.is_selected()) new_state = ONE;
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
		select_state = SOME;
    else if(select_count == 1)
		select_state = ONE;
    else
		select_state = ZERO;
}