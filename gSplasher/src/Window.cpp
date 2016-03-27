#include "gSplasher/Window.h"
#include "gSplasher/Event.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#ifdef OS_WINDOWS
//#define GLFW_EXPOSE_NATIVE_WIN32
//#define GLFW_EXPOSE_NATIVE_WGL
//#include <GLFW/glfw3native.h>
//#endif

#include "gSplasher/Utils/Painter.h"

USING_NAMESPACE

gWindow::gWindow(gSize s, gWindow* parent) : gCoreWidget(parent) {
#ifndef OS_WINDOWS
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_TRANSPARENT, true);
	glfwWindowHint(GLFW_DECORATED, false);
	glfwWindowHint(GLFW_SAMPLES, 12);
	r_window = glfwCreateWindow(s.width, s.height, "gSplasher", nullptr, nullptr);

	if (!r_window) {
		throw std::runtime_error("Could not create window");
	}

	glfwSetWindowUserPointer(r_window, this);

	is_widget = false;
	is_window = true;
	parent_window = this;

	setActive(); // needed to init glew properly
	if (!_inited) {
#ifdef OS_WINDOWS
		glewExperimental = GL_TRUE;
		auto glew_result = glewInit();
		if (glew_result != GLEW_OK) {
			throw std::runtime_error("Could not init glew");
		}
		// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
		glGetError();
#endif
		_inited = true;
	}

	painter = std::make_unique<gPainter>(this);

	top_bar = std::make_unique<gTopBar>();

	gWindow::move(gPoint(500, 300));
	gWindow::resizeEvent( std::make_shared<gResizeEvent>(gEvent::Type::Resize, s, gLayoutable::size()));
	top_bar->setWindow(this);
	setObjectName("gWindow");
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
	glClearColor(0, 0, 0, 0);
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
	gCoreWidget::move(new_p);
	auto p = pos();
	glfwSetWindowPos(r_window, p.x, p.y);
}

void gWindow::resizeEvent(ResizeEventPtr ev) {
	std::cout << "window resize\n";
	gLayoutable::resizeEvent(ev);
	auto s = size();
	std::cout << s << "\n";
	glfwSetWindowSize(r_window, s.width, s.height);
}

void gWindow::paint(gPainter& painter) {
	gBrush b(painter);
	b.setColor(gColor(250, 250, 250, 100));

	painter.drawRect(gRect(0, 0, size()));
}

void gWindow::setActive() const {
	glfwMakeContextCurrent(r_window);
}

