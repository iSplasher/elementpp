#include "../include/Window.h"
#include "../include/Event.h"


USING_NAMESPACE

gWindow::gWindow(gWindow* parent) :
	gCoreWidget(parent) {
	is_widget = false;
	is_window = true;
	setActive();
}

gWindow::~gWindow() {
}

void gWindow::update() {
	

continue_exec:
	generateMouseMove();
	//r_window->clear(style.bg_color);
	////paint();
	//r_window->display();
}

//Point gWindow::pos() {
//	return r_window->getPosition();
//}

//void gWindow::move(Point new_p) {
//	r_window->setPosition(new_p);
//	style.pos = new_p;
//}
//
//void gWindow::resize(Size new_s) {
//	r_window->setSize(new_s);
//	style.size = new_s;
//}

void gWindow::setActive() const {
}

void gWindow::generateMouseMove() {
	// TODO: set a class instance switch? enableMouseTracking..?
	//if (r_window) {
	//	Point pos = Mouse::getPosition();
	//	if (pos.x != _old_mouse_x || pos.y != _old_mouse_y) {
	//		_old_mouse_x = pos.x;
	//		_old_mouse_y = pos.y;
	//		Point r_pos = Mouse::getPosition(*r_window);
	//		MouseEventPtr m_ev = std::make_shared<gMouseEvent>(gEvent::MouseMove, r_pos);
	//		gApp->dispatchEvent(this, m_ev); // TODO: make this instantous? 
	//	}
	//}
}