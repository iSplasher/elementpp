#include "gSplasher/Widget.h"
#include "gSplasher/Window.h"
#include "gSplasher/Utils/Painter.h"


USING_NAMESPACE

gCoreWidget::gCoreWidget(gCoreWidget* parent) : gLayoutable(parent) {
	is_widget = true;
	setParent(parent);
}

gCoreWidget::~gCoreWidget() {
	// TODO: delete paint context
}

void gCoreWidget::paint(gPainter& painter) {
}

void gCoreWidget::update() {
	auto &painter = *parent_window->painter;
	painter.save();
	painter.origin = gPointF(mapToWindow(pos()));
	paint(painter);
	painter.restore();
	updateChildren();
}

void gCoreWidget::event(EventPtr ev) {
	// TODO: send events to children here
	switch (ev->type()) {
	case gEvent::MouseMove:
		mouseMoveEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	case gEvent::MouseButtonPress:
		mousePressEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	case gEvent::MouseButtonRelease:
		mouseReleaseEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	}
}

gPoint gCoreWidget::pos() const {

	if (is_widget && !parent_widget) {
		return gPoint();
	}
	return gLayoutable::pos();
}

void gCoreWidget::setParent(gCoreWidget* new_parent) {
	parent_widget = new_parent;
	if (new_parent) {
		parent_window = new_parent->parent_window;
		move(0, 0);
	} else {
		parent_window = nullptr;
	}
	gLayoutable::setParent(new_parent);
}

void gCoreWidget::setLayout(gLayout& new_layout) {
	new_layout.setWigdet(this);
}

gPoint gCoreWidget::mapToParent(gPoint _p) const {
	return _p + pos();
}

gPoint gCoreWidget::mapFromParent(gPoint _p) const {
	return _p - pos();
}

gPoint gCoreWidget::mapFromGlobal(gPoint p) const {
	auto w = this;

	while (w) {
		p -= w->pos();
		w = w->is_window ? nullptr : w->parent_widget;
	}
	return p;
}

gPoint gCoreWidget::mapToGlobal(gPoint p) const {
	auto w = this;

	while (w) {
		p += w->pos();
		w = w->is_window ? nullptr : w->parent_widget;
	}
	return p;
}

gPoint gCoreWidget::mapFromWindow(gPoint p) const {
	auto w = this;

	while (w) {
		p -= w->pos();
		w = w->parent_widget && w->parent_widget->is_window ? nullptr : w->parent_widget;
	}
	return p;
}

gPoint gCoreWidget::mapToWindow(gPoint p) const {
	auto w = this;

	while (w) {
		p += w->pos();
		w = w->parent_widget && w->parent_widget->is_window ? nullptr : w->parent_widget;
	}
	return p;
}

void gCoreWidget::mousePressEvent(MouseEventPtr ev) {
	printf("A button was pressed!");
	move_state = MoveState::Moving;
	//move_offset = ev->pos();
	//resize(style.size.width+5, style.size.height+5);
}

void gCoreWidget::mouseMoveEvent(MouseEventPtr ev) {
	//printf("x=%d y=%d\n", ev->global_x, ev->global_y);
	//if (ev->x >= 0 && ev->x < width() && ev->y >= 0 && ev->y < height()) {
	//	under_mouse = true;
	//} else {
	//	under_mouse = false;
	//}

	//if (move_state == MoveState::Moving) {
	//	move(mapToGlobal(ev->pos()) - move_offset);
	//}
}

void gCoreWidget::mouseReleaseEvent(MouseEventPtr ev) {
	printf("A button was released!");
	move_state = MoveState::Normal;
}

void gCoreWidget::updateChildren() {
	for (auto &c : children()) {
		if (c->is_widget) {
			static_cast<gCoreWidget*>(c)->update();
		}
	}
}

