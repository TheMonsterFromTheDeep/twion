#include <cstddef>

#include <iostream>

#include "shape.h"

#include "cursors.h"
#include "objecteditor.h"
#include "editor.h"
#include "colorselector.h"

namespace shape {

	ShapeEditor::ShapeEditor(Shape* source_) : source(source_) { }

	void ShapeEditor::init_left_menu(Control& menu) {
		ColorSelector *cs = new ColorSelector(&edit_color);
		cs->sizer = new ScaleSizer(1.f, 1.f);
		menu.attach(cs);
	}

	void ShapeEditor::do_transform_pass(TransformAction f) {
		for (Point *p : source->points) {
			f(*p);
		}

		for (Line *l : source->lines) {
			f(*l->ease_in);
			f(*l->ease_out);
		}
	}

	void ShapeEditor::do_select_pass(SelectableAction f) {
		for (Point *p : source->points) {
			f(*p);
		}

		for (Line *l : source->lines) {
			f(*l->ease_in);
			f(*l->ease_out);
		}
	}

	std::size_t ShapeEditor::children_count() {
		return source->points.size() + 2 * source->lines.size();
	}

	static void diamond(Graphics g, Vec pos, float radius) {
		g.begin_quad();
		g.point(pos + Vec(radius, 0));
		g.point(pos + Vec(0, radius));
		g.point(pos - Vec(radius, 0));
		g.point(pos - Vec(0, radius));
		g.end();
	}

	static void draw_control_point(Graphics g, CurvePoint p, bool selected) {
		float width_0 = g.normalize(6);
		float width_1 = g.normalize(3);

		if (selected) g.rgb(1.f, 0.8f, 0.1f);
		else         g.rgb(0.0f, 0.0f, 0.0f);
		diamond(g, p.location, width_0);

		g.rgb(p.color);
		diamond(g, p.location, width_1);
	}

	static void draw_handle(Graphics g, Vec v, bool selected) {
		float radius = g.normalize(4);

		if (selected) g.rgb(1.f, 0.8f, 0.1f);
		else          g.rgb(0.f, 0.f, 0.f);

		g.draw_circle(v, radius);
	}

	void ShapeEditor::draw(Graphics g) {
		RGB select(1.f, 0.8f, 0.1);
		RGB unselect(0.f, 0.f, 0.f);

		g.rgb(select);

		for (Line *sl : source->lines) {
			sl->draw(g);
		}

		for (Point *sp : source->points) {
			sp->draw(g);
		}

		g.rgb(1.f, 0.f, 0.f);
	}

	Vec ShapeEditor::get_pivot() {
		float divisor = 0;
		Vec pivot;

		for (Point *p : source->points) {
			if (p->is_selected()) {
				pivot += p->position();
				++divisor;
			}
		}

		for (Line *l : source->lines) {
			/* TODO?: Store separate vector of Handles? */
			if (l->ease_in->is_selected()) {
				pivot += l->ease_in->position();
				++divisor;
			}
			if (l->ease_out->is_selected()) {
				pivot += l->ease_out->position();
				++divisor;
			}
		}

		if (divisor > 0) {
			pivot /= divisor;
		}
		
		return pivot;
	}

	void ShapeEditor::key(KeyEvent e, Vec mouse) {
		if(e.key == 258) {
			change_editor(container->get_object_editor());
			return;
		}

		if(selection != SELECT_NONE && action == ACTION_NONE) {
			if(e.key == 'G') {
				init_action((e.shift_down ? ACTION_GRAB_CORRECTION : ACTION_GRAB), mouse);
			}

			if (e.key == 'E') {
				extrude();
			}

			if(e.key == 'R') {
				init_action(ACTION_ROTATE, mouse);
			}

			if(e.key == 'S') {
				/* TODO: Make init_action poll mouse position */
				init_action(ACTION_SCALE, mouse);
			}
		}

		if (e.key == 261) {
			remove();
		}

		if (e.key == 'F') {
			connect();
		}

		if(e.key == 'A') {
			if (e.control_down) {
				init_action(ACTION_SCALE_WIDTH, mouse);
			}
			else all_select();
		}

		if(action == ACTION_GRAB || action == ACTION_SCALE) {
			if(e.key == 'X') {
				constrain_x = !constrain_x;
				constrain_y = constrain_y && !constrain_x;
			}

			if(e.key == 'Y') {
				constrain_y = !constrain_y;
				constrain_x = constrain_x && !constrain_y;
			}
		}
	}

	void ShapeEditor::mouse_move(Vec position, Vec delta) {
		TransformEditor::mouse_move(position, delta);

		switch (action) {
			case ACTION_GRAB_CORRECTION:
				do_grab_update(position);
				for (Line *l : source->lines) {
					if (l->start->is_selected() != l->end->is_selected()) {
						l->straighten();
					}
				}
				break;
			case ACTION_SCALE_WIDTH: {
				float m = (position - action_pivot).len() / (action_center - action_pivot).len();
				for (Point *p : source->points) {
					if (p->is_selected()) {
						p->set_width_multiplier(m);
					}
				}
			}	
		}
	}

	void ShapeEditor::extrude() {
		for (Point *p : source->points) {
			if (p->is_selected()) {
				int connection_count = 0;
				for (Point *c : p->connections) {
					if (c->is_selected()) {
						++connection_count;
					}
				}

				if (connection_count < 2) {
					Point *dup = source->add_point(p->position());

					std::vector<Point*> to_disconnect;

					/* Move all connections from the original point
					 * to the new point
					 */
					for (Point *c : p->connections) {
						if (!c->is_selected()) {
							source->connect(dup, c);
							to_disconnect.push_back(c);
						}
					}

					for (Point *c : to_disconnect) {
						source->disconnect(p, c);
					}

					source->connect(p, dup);
				}
			}
		}

		init_action(ACTION_GRAB_CORRECTION, mouse_last);
	}

	void ShapeEditor::remove() {
		for (Point *p : source->points) {
			if (p->is_selected()) {
				source->remove_point(p);
			}
		}
	}

	void ShapeEditor::connect() {
		Point *a = nullptr;
		Point *b = nullptr;

		for (Point *p : source->points) {
			if (p->is_selected()) {
				b = a;
				a = p;
				if (b != nullptr) {
					source->connect(a, b);
				}
			}
		}
	}

	void ShapeEditor::mouse(MouseEvent m, Vec pos) {
		if(m.button == RIGHT && m.action == PRESS) {
			if(action == ACTION_NONE) {
				if(m.shift_down) shift_select(pos);
				else select(pos);
			}
			else cancel();
		}

		if(m.button == LEFT && m.action == PRESS) {
			if(action != ACTION_NONE) confirm();
		}
	}
}