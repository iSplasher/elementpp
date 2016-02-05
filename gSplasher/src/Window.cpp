#include "../include/Window.h"
#include "../include/Event.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifdef OS_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif

#include "nanovg/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg/nanovg_gl.h"


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
	r_window = glfwCreateWindow(300, 300, "gSplasher", nullptr, nullptr);
	shapeWindow(r_window, 0, 0, 300, 300);
	is_widget = false;
	is_window = true;
	setActive();
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Could not init glew.\n";
	}
	// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
	glGetError();
	auto vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	if (!vg) {
		std::cout << "Could not create Nano Context\n";
	}
}

gWindow::~gWindow() {
	if (r_window) {
		glfwDestroyWindow(r_window);
	}
}

void gWindow::update() {

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