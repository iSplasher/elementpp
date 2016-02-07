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
	auto s = size();
	gPen p(painter);
	gBrush b;
	p.setColor(200, 40, 40, 255);
	painter.setBrush(b);
	b.setColor(200, 40, 40, 255);
	painter.drawRect(gRect(1,1, 497, 50));
	painter.drawEllipse(gPointF(s.width/2, s.height/2), gSizeF(65, 60));
	painter.drawCircle(gPointF(s.width/2, s.height/2+100), 10);
}
