#include "shape.h"
#include <iostream>
#include <algorithm>

#include <GLFW/glfw3.h>

/* TODO: Properly put together global data situation
 * also handle memory leak
 */
std::vector<shape::Shape*> global_shapes;

namespace shape {

	static void diamond(Graphics g, Vec pos, float radius) {
		g.begin_quad();
		g.point(pos + Vec(radius, 0));
		g.point(pos + Vec(0, radius));
		g.point(pos - Vec(radius, 0));
		g.point(pos - Vec(0, radius));
		g.end();
	}

	Point::Point(Vec v) : stored(v), active(v), width_stored(1), width_active(1) { }

	Vec Point::position() { return active; }

	bool Point::should_select(Vec pos) {
		return (pos - stored).lensqr() < 5 * 5;
	}

	float Point::width() {
		return width_active;
	}

	void Point::set_width_multiplier(float m) {
		width_active = m * width_stored;
	}

	void Point::on_transform_changed() {
		active = get_transform() * stored;
	}

	void Point::on_transform_applied() {
		stored = get_transform() * stored;
		active = stored;
		width_stored = width_active;
	}

	void Point::on_transform_canceled() {
		active = stored;
		width_active = width_stored;
	}

	Handle::Handle(Vec v) : Point(v) { }

	Line::Line() {
		ease_in = new Handle(Vec());
		ease_out = new Handle(Vec());
	}

	Line::~Line() {
		delete ease_in;
		delete ease_out;
	}

	Line::Line(Point *start_, Point *end_) : start(start_), end(end_) {
		if (!start || !end) {
			/* TODO: Use non-nullable type */
			throw std::domain_error("Line cannot go between null points.");
		}

		Vec delta = (end->position() - start->position()) / 4;
		Vec ei = start->position() + delta;
		Vec eo = end->position() - delta;

		ease_in = new Handle(ei);
		ease_out = new Handle(eo);
	}

	void Line::straighten() {
		Vec delta = (end->position() - start->position()) / 4;
		Vec ei = start->position() + delta;
		Vec eo = end->position() - delta;

		ease_in->set_transform(Transform::project(ei));
		ease_out->set_transform(Transform::project(eo));
	}

	void Point::draw(Graphics g) {
		float width_0 = g.normalize(6);
		float width_1 = g.normalize(3);

		//g.rgb(1.f, 0.8f, 0.1f);
		if(is_selected()) g.rgb(1.f, 0.8f, 0.1f);
		else              g.rgb(0.0f, 0.0f, 0.0f);
		diamond(g, position(), width_0);

		diamond(g, position(), width_1);
	}

	void Line::draw(Graphics g) {
		CurvePoint s(start->position(), start->width(), start->get_color());
		CurvePoint e(end->position(), end->width(), end->get_color());
		Vec ei = ease_in->position();
		Vec eo = ease_out->position();

		if (ease_in->is_selected()) g.rgb(1.f, 0.8f, 0.1f);
		else              g.rgb(0.0f, 0.0f, 0.0f);
		g.line(start->position(), ease_in->position());
		g.draw_circle(ease_in->position(), g.normalize(5));

		if (ease_out->is_selected()) g.rgb(1.f, 0.8f, 0.1f);
		else              g.rgb(0.0f, 0.0f, 0.0f);
		g.line(end->position(), ease_out->position());
		g.draw_circle(ease_out->position(), g.normalize(5));

		if (start->is_selected() && end->is_selected()) g.rgb(1.f, 0.8f, 0.1f);
		else              g.rgb(0.0f, 0.0f, 0.0f);

		InterpolatedCubic ic(&s, &e, &ei, &eo);
		ic.calculate();
		Curve c = ic.generate(0.01f);
		c.stroke(g);
	}

	Shape::Shape() : points(), lines() {
		
	}

	Shape *Shape::square(float radius) {
		Shape *sqr = new Shape();

		sqr->add_point(Vec(-radius, -radius));
		sqr->add_point(Vec(-radius,  radius));
		sqr->add_point(Vec( radius,  radius));
		sqr->add_point(Vec( radius, -radius));

		sqr->connect(0, 1);
		sqr->connect(1, 2);
		sqr->connect(2, 3);
		sqr->connect(3, 0);

		return sqr;
	}

	Point* Shape::add_point(Vec v) {
		Point *p = new Point(v);
		p->parent = this;
		points.push_back(p);

		return p;
	}

	void Shape::remove_point(Point *p) {
		auto it = std::find(points.begin(), points.end(), p);
		if (it == points.end()) {
			/* Point not found */
			return;
		}

		points.erase(it);
		for (auto i = lines.begin(); i != lines.end();) {
			Line *l = *i;
			if (l->start == p || l->end == p) {
				lines.erase(i);
			}
			else ++i;
		}

		/* TODO: Use a better node structure (as well as smart pointers, although
		 * that would actually just hide the problem in this case)
		 */
		for (Point *c : p->connections) {
			auto place = std::find(c->connections.begin(), c->connections.end(), p);
			if (place != c->connections.end()) {
				c->connections.erase(place);
			}
		}

		delete p;
	}

	void Shape::connect(std::size_t a_index, std::size_t b_index) {
		Point *a = points[a_index];
		Point *b = points[b_index];

		/* Should only need to check one of the two for duplication */
		auto b_in_a = std::find(a->connections.begin(), a->connections.end(), b);
		if (b_in_a != a->connections.end()) {
			return;
		}

		a->connections.push_back(b);
		b->connections.push_back(a);

		Line *l = new Line(a, b);
		lines.push_back(l);
	}

	void Shape::connect(Point *a, Point *b) {
		if (a->parent != this || b->parent != this) {
			throw std::domain_error("Points to connect must exist in this shape!");
		}

		/* Should only need to check one of the two for duplication */
		auto b_in_a = std::find(a->connections.begin(), a->connections.end(), b);
		if (b_in_a != a->connections.end()) {
			return;
		}

		a->connections.push_back(b);
		b->connections.push_back(a);

		Line *l = new Line(a, b);
		lines.push_back(l);
	}

	void Point::set_color(RGB c) {
		color = c;
	}

	RGB Point::get_color() {
		return color;
	}

	/* TODO: Make this much less inefficient
	 * Basically needs a much better node system, but I'm not
	 * quite sure what...
	 */
	void Shape::disconnect(Point *a, Point *b) {
		if (a->parent != this || b->parent != this) {
			throw std::domain_error("Points to disconnect must exist in this shape!");
		}

		/* Should only need to check one of the two for non-connection */
		auto b_in_a = std::find(a->connections.begin(), a->connections.end(), b);
		if (b_in_a == a->connections.end()) {
			return;
		}

		auto a_in_b = std::find(b->connections.begin(), b->connections.end(), a);
		
		/* Erase both connections */
		a->connections.erase(b_in_a);
		b->connections.erase(a_in_b);

		Line *to_disconnect = nullptr;

		auto it = lines.begin();

		for (Line *l : lines) {
			if ((l->start == a && l->end == b) || (l->start == b && l->end == a)) {
				to_disconnect = l;
				break;
			}
			++it;
		}

		if (!to_disconnect) { return; }

		lines.erase(it);

		delete to_disconnect;
	}

	ShapeEditor *Shape::get_editor() {
		return new ShapeEditor(this);
	}

	/*void fill_unloop(Curve& points, Graphics g) {
		g.begin_quad_strip();

		size_t last = points.size() - 1;
		size_t i = 0;
		while (i < last - i) {
			g.rgb(points[i].fill);
			g.point(points[i].location);
			g.rgb(points[last - i].fill);
			g.point(points[last - i].location);
			++i;
		}

		g.end();
	}

	/*void Shape::line(Graphics g) {
		Curve calc;

		g.translate(position);

		for (ShapeLine *sl : lines) {
			InterpolatedCubic ic(&sl->start->data, &sl->end->data, &sl->start_handle, &sl->end_handle);
			ic.calculate();
			Curve c = ic.generate(0.01f);
			for (CurvePoint& p : c) {
				calc.push_back(p);
			}
		}

		calc.line(g);

		g.translate(position * -1);
	}

	void Shape::draw(Graphics g) {
		Curve calc;

		g.translate(position);

		for (ShapeLine *sl : lines) {
			InterpolatedCubic ic(&sl->start->data, &sl->end->data, &sl->start_handle, &sl->end_handle);
			ic.calculate();
			Curve c = ic.generate(0.01f);
			for (CurvePoint& p : c) {
				calc.push_back(p);
			}
		}

		g.rgb(1.f, 1.f, 1.f);

		//fill_unloop(calc, g);

		calc.stroke(g);

		g.translate(position * -1);
	}*/
}