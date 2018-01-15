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
		else         g.rgb(0.f, 0.f, 0.f);

		g.draw_circle(v, radius);
	}

	void ShapeEditor::draw(Graphics g) {
		//source->draw(g);

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
		//source->line(g);
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
		/*if(state == GRAB) {
			if(e.key == 'G') {
				state = GRAB_ALONG;
			}
		}*/

		if(e.key == 258) {
			change_editor(container->get_object_editor());
			return;
		}

		if(selection != SELECT_NONE && action == ACTION_NONE) {
			if(e.key == 'G') {
				//state = (e.shift_down ? GRAB_CORRECTION : GRAB);
				init_action(ACTION_GRAB, mouse);
			}

			if(e.key == 'R') {
				//if(e.control_down) {
				//	split();
				//}
				//else {
					init_action(ACTION_ROTATE, mouse);
				//}
			}

			if(e.key == 'S') {
				/* TODO: Make init_action poll mouse position */
				init_action(ACTION_SCALE, mouse);
			}

			/*if(e.key == 'A' && e.control_down) {
				state = THICKEN;
				action_center = mouse - Vec(10, 0);

				init_action();
			}

			if(e.key == 'C') {
				if(e.shift_down) {
					for(size_t i = 0; i < curvepoints.size(); ++i) {
						if(curvepoints[i].selected) {
							curvepoints[i].source->color = edit_color;
						}
					}
				}
				else {
					for(size_t i = 0; i < curvepoints.size(); ++i) {
						if(curvepoints[i].selected) {
							curvepoints[i].source->fill = edit_color;
						}
					}
				}
			}

			if(e.key == 'E') {
				extrude();
			}

			if(e.key == ' ') {
				dir_match();
			}*/
		}

		if(e.key == 'A' && !e.control_down) { all_select(); }

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
		/*mouse_last = position;

		switch(state) {
			case GRAB:
			case GRAB_CORRECTION: {
				Vec translation = position - action_center;
				if(constrain_x) { translation.y = 0; }
				if(constrain_y) { translation.x = 0; }

				for(size_t i = 0; i < curvepoints.size(); ++i) {
					if(curvepoints[i].selected) {
						curvepoints[i].source->location = curvepoints_tfrm[i].location + translation;
					}
				}

				if(state == GRAB_CORRECTION) {
					 bool selected = curvepoints[0].selected;

					for(size_t i = 1; i < curvepoints.size(); ++i) {
						if(curvepoints[i].selected != selected) {
							selected = curvepoints[i].selected;
							*vecs[i * 2 - 1].source = curvepoints[i - 1].source->location + (curvepoints[i].source->location - curvepoints[i - 1].source->location) / 4;
							*vecs[i * 2].source = curvepoints[i].source->location + (curvepoints[i - 1].source->location - curvepoints[i].source->location) / 4;
						}
					}
				}

				for(size_t i = 0; i < vecs.size(); ++i) {
					if(vecs[i].selected) {
						*vecs[i].source = vecs_tfrm[i] + translation;
					}
				}
			}
			break;

			case GRAB_ALONG: {
				float time = (position.x - action_center.x) / 100;
				if(time > 1) time = 1;
				if(time < -1) time = -1;
				size_t offset = 1;
				if(time < 0) {
					time = 1 + time;
					offset = 0;
				}

				for(size_t i = 1; i < curvepoints.size() - 1; ++i) {
					if(curvepoints[i].selected) {
						size_t index = i - 1 + offset;

						Vec v0 = vecs_tfrm[2 * index + 1];
						Vec v1 = vecs_tfrm[2 * index + 2];

						InterpolatedCubic cubic(&curvepoints_tfrm[index], &curvepoints_tfrm[index + 1], &v0, &v1);
						cubic.calculate();
						CurvePoint pos = cubic.evaluate(time);
						Vec delta = cubic.eval_ease_delta(time);
						*curvepoints[i].source = pos;
						*vecs[2 * i].source = pos.location - delta;
						*vecs[2 * i + 1].source = pos.location + delta;
					}
				}
			}
			break;

			case ROTATE: {
				Vec direction = position.rotate_inverse(action_center - action_pivot, action_pivot) - action_pivot;

				for(size_t i = 0; i < curvepoints.size(); ++i) {
					if(curvepoints[i].selected) {
						curvepoints[i].source->location = curvepoints_tfrm[i].location.rotate(direction, action_pivot);
					}
				}
				for(size_t i = 0; i < vecs.size(); ++i) {
					if(vecs[i].selected) {
						*vecs[i].source = vecs_tfrm[i].rotate(direction, action_pivot);
					}
				}
			}
			break;

			case SCALE: {
				/* This is less efficient than it could be (two square roots, action_center - action_pivot could be cached)
				 * but it doesn't really matter right now.
				 *
				float magnitude = (position - action_pivot).len() / (action_center - action_pivot).len();

				Vec scale(magnitude);
				if(constrain_x) scale.y = 1;
				if(constrain_y) scale.x = 1;

				for(size_t i = 0; i < curvepoints.size(); ++i) {
					if(curvepoints[i].selected) {
						curvepoints[i].source->location = curvepoints_tfrm[i].location.scale(scale, action_pivot);
					}
				}
				for(size_t i = 0; i < vecs.size(); ++i) {
					if(vecs[i].selected) {
						*vecs[i].source = vecs_tfrm[i].scale(scale, action_pivot);
					}
				}
			}
			break;

			case THICKEN: {
				float magnitude = (position - action_center).len() / 10;

				for(size_t i = 0; i < curvepoints.size(); ++i) {
					if(curvepoints[i].selected) {
						curvepoints[i].source->width = curvepoints_tfrm[i].width * magnitude;
					}
				}
			}
			break;

			default: { }
		}*/
	}

	//void ShapeEditor::dir_match() {
		/*for(size_t i = 0; i < curvepoints.size(); ++i) {
			if(curvepoints[i].selected) {
				Vec pos = curvepoints[i].source->location;

				Vec delta0 = *vecs[i * 2].source - pos;
				Vec delta1 = *vecs[i * 2 + 1].source - pos;

				if(abs(delta0.cross_len(delta1)) > 0.0001f) {
					Vec bisec = delta0.bisector(delta1);
					Vec n_delta = bisec.ortho();

					*vecs[i * 2].source = pos + delta0.project(n_delta);
					*vecs[i * 2 + 1].source = pos + delta1.project(n_delta);
				}
			}
		}*/
	//}

	//void ShapeEditor::split() {
		/*std::vector<size_t> split_points;

		for(size_t i = 0; i < curvepoints.size() - 1; ++i) {
			if(curvepoints[i].selected && curvepoints[i + 1].selected) split_points.push_back(i);
		}

		std::vector<CurvePoint> new_points;
		std::vector<Vec> new_deltas;

		for(size_t i = 0; i < split_points.size(); ++i) {
			new_points.push_back(source->curves[split_points[i]].evaluate(0.5f));

			/* TODO: Figure out why dividing handle length by 2 maintains original shape *
			new_deltas.push_back(source->curves[split_points[i]].eval_ease_delta(0.5f) / 2);

			source->handles[i * 2 + 1] = (source->handles[i * 2 + 1] - source->points[i].location) / 2 + source->points[i].location;
			source->handles[i * 2 + 2] = (source->handles[i * 2 + 2] - source->points[i + 1].location) / 2 + source->points[i + 1].location;
		}

		size_t offset = 1;

		for(size_t i = 0; i < split_points.size(); ++i) {
			size_t x = split_points[i] + offset;

			source->points.insert(source->points.begin() + x, new_points[i]);

			size_t vp = x * 2;

			source->handles.insert(source->handles.begin() + vp, new_points[i].location + new_deltas[i]);
			source->handles.insert(source->handles.begin() + vp, new_points[i].location - new_deltas[i]);

			source->generate();

			++offset;
		}

		generate();

		/* Deselect all points. *
		for(size_t i = 0; i < curvepoints.size(); ++i) {
			 curvepoints[i].selected = false;
		}
		for(size_t i = 0; i < vecs.size(); ++i) {
			 vecs[i].selected = false;
		}

		/* Select all new points and their handles. *
		offset = 1;
		for(size_t i = 0; i < split_points.size(); ++i) {
			size_t x = split_points[i] + offset;

			curvepoints[x].selected = true;

			size_t vp = x * 2;

			vecs[vp].selected = true;
			vecs[vp + 1].selected = true;

			++offset;
		}

		select_state = SOME;

		/* Set action to GRAB_ALONG.
		 * TODO: Make setting action into a function so that it's never
		 * in an inconsistent state.
		 *
		state = GRAB_ALONG;
		action_center = mouse_last;

		init_action();*/
	//}

	//void ShapeEditor::extrude() {
		/*std::vector<size_t> insert_locations;

		bool is_extrusion = false;

		for(size_t i = 0; i < curvepoints.size(); ++i) {
			if(curvepoints[i].selected != is_extrusion) {
				is_extrusion = curvepoints[i].selected;
				insert_locations.push_back(is_extrusion ? i : i - 1);
			}
		}

		if(is_extrusion) {
			insert_locations.push_back(curvepoints.size() - 1);
		}

		/* Used to offset the insertion point - because all insertion points past the nth insertion point
		 * are shifted over by n. This is because there are n new points inserted before what was previously
		 * thought to be the insertion point.
		 *
		size_t offset = 0;
		/* This variable offsets based on whether the next point is the beginning or end of an extrusion sub-section.
		 * Basically, for beginnings of extrusion sub-sections, we want to insert before the *next* point, because
		 * the recorded insert location is the point that we are duplicating, and we want to insert points that are between
		 * it and the endpoint. However, at the end of extrusion sub-sections, we want to insert before the actual point, because
		 * at the end of the extrusion sub-section, before the recorded point is going to be in the direction of inside of the
		 * subsection.
		 * So this variable alternates between 1 and 0 every loop.
		 *
		size_t secondary_offset = 1;

		for(size_t i = 0; i < insert_locations.size(); ++i) {
			size_t x = insert_locations[i] + offset;

			source->points.insert(source->points.begin() + x + secondary_offset, source->points[x]);

			size_t vp = x * 2;

			/* This handle code is not quite correct - it doesn't insert the ease_out handle of the endpoint correctly. But
			 * it doesn't matter, because we're doing a correction grab, so that handle will be modified anyways.
			 *
			 * In the future, when this does matter, this will need to be fixed.
			 *
			source->handles.insert(source->handles.begin() + vp + 2 * secondary_offset, source->handles[vp + secondary_offset]);
			source->handles.insert(source->handles.begin() + vp + 2 * secondary_offset, source->handles[vp + (1 - secondary_offset)]);

			source->generate();

			++offset;
			secondary_offset = 1 - secondary_offset;
		}

		generate();

		/* First, deselect all points. *
		for(size_t i = 0; i < curvepoints.size(); ++i) {
			 curvepoints[i].selected = false;
		}
		for(size_t i = 0; i < vecs.size(); ++i) {
			 vecs[i].selected = false;
		}

		/* The offset and secondary_offset variables work here in the same way as they
		 * work above.
		 *
		offset = 0;
		secondary_offset = 1;

		for(size_t i = 0; i < insert_locations.size(); ++i) {
			size_t x = insert_locations[i] + offset + secondary_offset;

			curvepoints[x].selected = true;

			size_t vp = x * 2;

			vecs[vp + secondary_offset].selected = true;

			source->generate();

			/* Select all points in between extrusion sub-section endpoints.
			 * This is only done if we are currently at the start of an extrusion sub-section:
			 * that is, secondary_offset = 1.
			 *
			if(secondary_offset) {
				for(size_t j = x + 1; j < insert_locations[i + 1] + offset + 1; ++j) {
					curvepoints[j].selected = true;
					vecs[j * 2].selected = true;
					vecs[j * 2 + 1].selected = true;
				}
			}

			++offset;
			secondary_offset = 1 - secondary_offset;
		}

		select_state = SOME;

		state = GRAB_CORRECTION;
		action_center = mouse_last;

		init_action();*/
	//}

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