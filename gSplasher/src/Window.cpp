#include "gSplasher/Window.h"
#include "gSplasher/Event.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifdef OS_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif

#include "gSplasher/Utils/Painter.h"


USING_NAMESPACE

void shapeWindow(_RWindow *r_w, int x, int y, int width, int height) {
#ifdef OS_WINDOWS
	int m = 30;
	int btm_x = x + width - (m + 3);
	int btm_y = y + height;
	int c_offset_x = x + width - m;
	int c_offset_y = y;

	HRGN window_shape = CreateRoundRectRgn(
		x,
		y,
		btm_x,
		btm_y,
		3,
		3);

	//HRGN exit_shape = createShape(
	//	"C:/Users/Autriche/Documents/Code/gSplasher/gSplasher/Reference/icon/add_normal.png",
	//	c_offset_x, c_offset_y);

	c_offset_y += 10;
	HRGN minimize_shape = CreateEllipticRgn(
		c_offset_x,
		c_offset_y,
		c_offset_x + m,
		c_offset_y + m
		);

	//CombineRgn(window_shape, exit_shape, window_shape, RGN_OR);


	SetWindowRgn(glfwGetWin32Window(r_w), window_shape, true);
	DeleteObject(window_shape);
	//DeleteObject(exit_shape);
	DeleteObject(minimize_shape);
#endif
}

gWindow::gWindow(gWindow* parent) :
	gCoreWidget(parent) {
	glfwWindowHint(GLFW_DECORATED, false);
	auto s = size();
	auto p = pos();
	r_window = glfwCreateWindow(s.width, s.height, "gSplasher", nullptr, nullptr);
	shapeWindow(r_window, p.x, p.y, s.width, s.height);
	glfwSwapInterval(0);

	is_widget = false;
	is_window = true;
	parent_window = this;

	setActive(); // needed to init glew properly
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Could not init glew.\n";
	}
	// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
	glGetError();
	gWindow::move(gPoint(500, 300));
}

gWindow::~gWindow() {
	if (r_window) {
		glfwDestroyWindow(r_window);
	}
}

void gWindow::update() {

	generateMouseMove();
	//r_window->clear(style.bg_color);
	gCoreWidget::update();
	glfwSwapBuffers(r_window);
	//r_window->display();
}

//Point gWindow::pos() {
//	return r_window->getPosition();
//}

void gWindow::move(gPoint new_p) {
	p = new_p;
	glfwSetWindowPos(r_window, p.x, p.y);
}

//void gWindow::resize(Size new_s) {
//	r_window->setSize(new_s);
//	style.size = new_s;
//}

void gWindow::setActive() const {
	glfwMakeContextCurrent(r_window);
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