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