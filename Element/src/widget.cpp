#include "element/widget.h"
#include "element/window.h"
#include "element/core/painter.h"


USING_NAMESPACE

Widget::Widget(Widget *parent) : PRIV_NAMESPACE::Layoutable(parent) {
	objectName = "Widget";
	setType(ElementType::Widget);

	marginLeft = marginTop = marginRight = marginBottom = 0.5;
	borderLeft = borderTop = borderRight = borderBottom = 0.5;
	borderColor = Color(27, 27, 27);

}

Widget::~Widget() {
	// TODO: delete paint context
}

void Widget::paint(Painter& painter) {
}

void Widget::update() {
	if (parent_window) {
		auto &painter = *parent_window->painter;
		painter.save();
		painter.origin = PointF(mapToWindow(PointF(0, 0)));
		painter.current_widget = this;
		paint(painter);
		painter.restore();
	}
	Layoutable::update();
}

PointF Widget::mapToParent(PointF _p) const {
	return _p + position;
}

PointF Widget::mapFromParent(PointF _p) const {
	return _p - position;
}

PointF Widget::mapFromGlobal(PointF p) {
	auto w = this;

	while (w) {
		p -= w->position;
		w = w->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}

PointF Widget::mapToGlobal(PointF p) {
	auto w = this;

	while (w) {
		p += w->position;
		w = w->type == ElementType::Window ? nullptr : w->parent_widget;
	}
	return p;
}

void Widget::setParent( Element* e ) {
	Layoutable::setParent(e);
	parent_widget = static_cast<Widget*>(e);
	if (e && e->type == ElementType::Window)
		parent_window = static_cast<Window*>(parent_widget);
	else if (e)
		parent_window = parent_widget->parent_window;
}

PointF Widget::mapFromWindow(PointF p) {
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

PointF Widget::mapToWindow(PointF p) {
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
