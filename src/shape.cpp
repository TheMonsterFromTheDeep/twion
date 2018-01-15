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

	Point::Point(Vec v) : stored(v), active(v) { }

	Vec Point::position() { return active; }

	bool Point::should_select(Vec pos) {
		return (pos - stored).lensqr() < 5 * 5;
	}

	void Point::on_transform_changed() {
		active = get_transform() * stored;
	}

	void Point::on_transform_applied() {
		stored = get_transform() * stored;
		active = stored;
	}

	void Point::on_transform_canceled() {
		active = stored;
	}

	Handle::Handle(Vec v) : Point(v) { }

	Line::Line() {
		ease_in = new Handle(Vec());
		ease_out = new Handle(Vec());
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
		CurvePoint s(start->position(), 1);
		CurvePoint e(end->position(), 1);
		Vec ei = ease_in->position();
		Vec eo = ease_out->position();

		InterpolatedCubic ic(&s, &e, &ei, &eo);
		ic.calculate();
		Curve c = ic.generate(0.01f);
		c.line(g);
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

	void Shape::add_point(Vec v) {
		Point *p = new Point(v);
		points.push_back(p);
	}

	void Shape::connect(std::size_t a, std::size_t b) {
		/* TODO: Don't connect if they're already connected */
		Line *l = new Line(points[a], points[b]);
		lines.push_back(l);
	}

	void Shape::connect(Point *a, Point *b) {
		auto a_it = std::find(points.begin(), points.end(), a);
		auto b_it = std::find(points.begin(), points.end(), b);
		if (a_it == points.end() || b_it == points.end()) {
			throw std::domain_error("Points to connect must exist in this shape!");
		}
		/* TODO: Don't connect if they're already connected
		 * - Each point should probably store a list of Lines that it is in
		 * - Each point should probably store a reference to the shape, so
		 *   checking parent is O(1)
		 */

		Line *l = new Line(a, b);
		lines.push_back(l);
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