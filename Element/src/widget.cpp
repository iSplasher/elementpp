#include "element/widget.h"
//#include "element/window.h"
//#include "element/core/painter.h"


USING_NAMESPACE

Widget::Widget(Widget* parent) : PRIV_NAMESPACE::Layoutable(parent) {
	objectName = "Widget";
	setType(ElementType::Widget);
}

Widget::~Widget() {
	// TODO: delete paint context
}

void Widget::paint(Painter& painter) {
}

void Widget::update() {
	//auto &painter = *parent_window->painter;
	//painter.save();
	//painter.origin = PointF(mapToWindow(Point(0, 0)));
	//painter.current_widget = this;
	//paint(painter);
	//painter.restore();
}

Point Widget::mapToParent(Point _p) const {
	return _p + position;
}

Point Widget::mapFromParent(Point _p) const {
	return _p - position;
}

Point Widget::mapFromGlobal(Point p) {
	auto w = this;

	while (w) {
		p -= w->position;
		w = w->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}

Point Widget::mapToGlobal(Point p) {
	auto w = this;

	while (w) {
		p += w->position;
		w = w->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}

Point Widget::mapFromWindow(Point p) {
	if (type == ElementType::Window) {
		return p;
	}
	auto w = this;

	while (w) {
		p -= w->position;
		w = w->parent_widget && w->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}

Point Widget::mapToWindow(Point p) {
	if (type == ElementType::Window) {
		return p;
	}
	auto w = this;

	while (w) {
		p += w->position;
		w = w->parent_widget && w->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}
