#include "gSplasher/Widget.h"
#include "gSplasher/Window.h"
#include "gSplasher/Utils/Painter.h"


USING_NAMESPACE

gCoreWidget::gCoreWidget(gCoreWidget* parent) : gCore(parent) {
	is_widget = true;
	parent_widget = parent;
	if (parent) {
		parent_window = parent->parent_window;
	}
}

gCoreWidget::~gCoreWidget() {
	// TODO: delete paint context
}

void gCoreWidget::paint(gPainter& painter) {
}

void gCoreWidget::update() {
	paint(*parent_window->painter);
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

//Point gCoreWidget::pos() {
//	return Point(0, 0);
//}
//
//void gCoreWidget::move(Point new_p) {
//}
//
void gCoreWidget::resize(gSize new_s) {
	_size = new_s;
}

//Point gCoreWidget::mapToGlobal(Point p) {
//	auto c_pos = pos();
//	Point n_pos(c_pos.x + p.x, c_pos.y + p.y);
//	return n_pos;
//}

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
	auto c_vec = children();
	for (auto &c : c_vec) {
		static_cast<gCoreWidget*>(c)->update();
	}
}

