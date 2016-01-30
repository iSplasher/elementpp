#include "../include/Window.h"
#include "../include/Event.h"

#include <GLFW/glfw3.h>

USING_NAMESPACE

gWindow::gWindow(gWindow* parent) :
	gCoreWidget(parent) {

	r_window = glfwCreateWindow(300, 300, "gSplasher", nullptr, nullptr);
	if (!r_window) {
		//LOG_C << "Window creation failed!";
	}
	is_widget = false;
	is_window = true;
	setActive();
}

gWindow::~gWindow() {
	if (r_window) {
		glfwDestroyWindow(r_window);
	}
}

void gWindow::update() {
	

continue_exec:
	generateMouseMove();
	//r_window->clear(style.bg_color);
	////paint();
	//r_window->display();
	glfwPollEvents();
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
	if (r_window) {
		glfwMakeContextCurrent(r_window);
		// TODO: activate window here
	}
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