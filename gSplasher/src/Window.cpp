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
	int btm_x = x + width;
	int btm_y = y + height;

	HRGN window_shape = CreateRoundRectRgn(
		x,
		y,
		btm_x,
		btm_y,
		3,
		3);

	SetWindowRgn(glfwGetWin32Window(r_w), window_shape, true);
	DeleteObject(window_shape);
#endif
}

void alphaWindow(_RWindow *r_w, unsigned char alpha) {
#ifdef OS_WINDOWS
	auto handle = glfwGetWin32Window(r_w);
	SetWindowLong(handle, GWL_EXSTYLE, GetWindowLong(handle, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(handle, 0, alpha, LWA_ALPHA);
#endif
}

gWindow::gWindow(gWindow* parent) :
	gCoreWidget(parent) {
#ifndef OS_WINDOWS
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	glfwWindowHint(GLFW_DECORATED, false);
	glfwWindowHint(GLFW_SAMPLES, 12);
	auto s = gCoreWidget::size();
	auto p = pos();
	r_window = glfwCreateWindow(s.width, s.height, "gSplasher", nullptr, nullptr);
	shapeWindow(r_window, p.x, p.y, s.width, s.height);
	alphaWindow(r_window, 225);
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

	top_bar = gRect(1, 1, gCoreWidget::size().width - 3, 20);
	gWindow::move(gPoint(500, 300));
}

gWindow::~gWindow() {
	if (r_window) {
		glfwDestroyWindow(r_window);
	}
}

void gWindow::update() {
	gCoreWidget::update();
	if (r_window) {
		glfwSwapBuffers(r_window);
	}
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

void gWindow::paint(gPainter& painter) {
	gPen p(painter);
	gBrush b(painter);
	p.setColor(gColor(178, 178, 178));
	p.setWidth(3);
	b.setColor(gColor(250, 250, 250));

	painter.drawRect(top_bar);
}