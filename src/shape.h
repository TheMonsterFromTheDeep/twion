#ifndef SHAPE_H
#define SHAPE_H

#include "vector.h"
#include "cubic.h"
#include "curve.h"
#include "graphics.h"
#include "event.h"
#include "editortype.h"
#include "transformedit.h"
#include <vector>

/* TODO: Tighten this and InterpolatedCubic; they're almost the exact
 * same thing */
/*class ShapeLine {
public:
    ShapePoint *start;
	ShapePoint *end;
	
	Vec start_handle;
	Vec end_handle;
	
	Vec start_ease();
	Vec end_ease();
	
	void draw_handles(Graphics);
};

class ShapePoint {
public:
    CurvePoint data;
	
	ShapePoint(const CurvePoint&);
	
	void draw(Graphics);
	std::vector<ShapeLine*> lines;
};

class Shape {
public:
    Vec position;

    Shape();
    
    void draw(Graphics);
    void line(Graphics);
	
	ShapePoint* add_point(const CurvePoint&);
	ShapeLine* add_line(const CurvePoint&, const CurvePoint&, Vec ae=Vec(), Vec be=Vec());
	void connect(ShapePoint*,ShapePoint*);
    
    ShapeEditor *get_editor();
private:
    std::vector<ShapePoint*> points;
	std::vector<ShapeLine*> lines;
    
    friend class ShapeEditor;
};*/

namespace shape {
	class Shape;

	class Point : public Transformable {
	public:
		Point(Vec);

		virtual bool should_select(Vec);
		void set_width_multiplier(float);
		Vec position();
		float width();

		void draw(Graphics);
	protected:
		virtual void on_transform_changed();
		virtual void on_transform_canceled();
		virtual void on_transform_applied();
	private:
		Vec stored;
		Vec active;

		float width_stored;
		float width_active;

		std::vector<Point*> connections;
		friend class Shape;
		friend class ShapeEditor;

		Shape* parent;
	};

	class Handle : public Point { public: Handle(Vec); };

	class Line {
	public:
		Line();
		~Line();
		Line(Point*, Point*);

		Point *start;
		Point *end;
		Handle *ease_in;
		Handle *ease_out;

		void straighten();

		void draw(Graphics);
	protected:
	private:
		
	};

	class ShapeEditor;

	class Shape {
	public:
		Shape();

		ShapeEditor *get_editor();

		static Shape *square(float);

		Point* add_point(Vec);
		void connect(std::size_t, std::size_t);
		void connect(Point*, Point*);
		void disconnect(Point*, Point*);
		void remove_point(Point*);
	protected:
	private:
		std::vector<Point*> points;
		std::vector<Line*> lines;

		friend class ShapeEditor;
	};

	class ShapeEditor : public TransformEditor {
	public:
		ShapeEditor(Shape*);

		Shape *source;

		virtual void draw(Graphics);
		virtual void key(KeyEvent, Vec);
		virtual void mouse_move(Vec, Vec);
		virtual void mouse(MouseEvent, Vec);

		virtual void init_left_menu(Control&);

		RGB edit_color;
	protected:
		virtual void do_transform_pass(TransformAction);
		virtual void do_select_pass(SelectableAction);
		virtual std::size_t children_count();
		virtual Vec get_pivot();
	private:
		void extrude();
		void remove();
		void connect();
	};
}

extern std::vector<shape::Shape*> global_shapes;

#endif