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

	is_widget = false;
	is_window = true;
	parent_window = this;

	setActive(); // needed to init glew properly
	if (!_inited) {
#ifdef OS_WINDOWS
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			std::cout << "Could not init glew.\n";
		}
		// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
		glGetError();
#endif
		_inited = true;
	}

	painter = std::make_unique<gPainter>(this);

	top_bar = std::make_unique<gTopBar>();
	top_bar->setWindow(this);

	gWindow::move(gPoint(500, 300));
}

gWindow::~gWindow() {
	if (r_window) {
		glfwDestroyWindow(r_window);
	}
}

void gWindow::update() {
	int fb_width;
	int fb_height;
	auto s = size();

	glfwGetFramebufferSize(r_window, &fb_width, &fb_height);
	glViewport(0, 0, fb_width, fb_height);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);

	float px_ratio = static_cast<float>(fb_width) / static_cast<float>(s.width);
	painter->begin(px_ratio);
	gCoreWidget::update();
	if (top_bar) {
		top_bar->update();
	}
	painter->end();
	if (r_window) {
		glfwSwapBuffers(r_window);
	}

	if (is_dragging) {
		double xpos, ypos;
		glfwGetCursorPos(r_window, &xpos, &ypos);

		if (xpos != _old_mouse_x || ypos != _old_mouse_y) {
			move(gPoint(xpos, ypos));
		}
	}

	int state = glfwGetMouseButton(r_window, GLFW_MOUSE_BUTTON_LEFT);
	is_dragging = state == GLFW_PRESS ? true : false;

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

void gWindow::paint(gPainter& painter) {
	gPen p(painter);
	gBrush b(painter);
	p.setColor(gColor(178, 178, 178));
	p.setWidth(3);
	b.setColor(gColor(250, 250, 250));

	painter.drawCircle(gPointF(200, 3), 20);
}

void gWindow::setActive() const {
	glfwMakeContextCurrent(r_window);
}

