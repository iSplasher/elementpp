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

// Callbacks start

// helper functions

static Window* getWindow(GLFWwindow* r_window) {
	return static_cast<Window*>(glfwGetWindowUserPointer(r_window));
}

static MouseButton getMouseButtons(GLFWwindow* r_window) {
	MouseButton buttons = MouseButton::None;

	auto left = glfwGetMouseButton(r_window, GLFW_MOUSE_BUTTON_LEFT);
	auto right = glfwGetMouseButton(r_window, GLFW_MOUSE_BUTTON_RIGHT);
	auto middle = glfwGetMouseButton(r_window, GLFW_MOUSE_BUTTON_MIDDLE);

	if (left == GLFW_PRESS) {
		buttons |= MouseButton::Left;
	}

	if (right == GLFW_PRESS) {
		buttons |= MouseButton::Right;
	}

	if (middle == GLFW_PRESS) {
		buttons |= MouseButton::Middle;
	}

	if (left == GLFW_PRESS && left == right == middle) {
		buttons = MouseButton::All;
	}

	return buttons;
}

static KeyModifier getKeyModifiers(GLFWwindow* r_window) {
	KeyModifier modifiers = KeyModifier::None;

	auto shift = glfwGetKey(r_window, GLFW_MOD_SHIFT);
	auto control = glfwGetKey(r_window, GLFW_MOD_CONTROL);
	auto alt = glfwGetKey(r_window, GLFW_MOD_ALT);
	auto meta = glfwGetKey(r_window, GLFW_MOD_SUPER);

	if (shift == GLFW_PRESS) {
		modifiers |= KeyModifier::Shift;
	}

	if (control == GLFW_PRESS) {
		modifiers |= KeyModifier::Control;
	}

	if (alt == GLFW_PRESS) {
		modifiers |= KeyModifier::Alt;
	}

	if (meta == GLFW_PRESS) {
		modifiers |= KeyModifier::Meta;
	}

	return modifiers;
}

static void mouseMoveCallback(GLFWwindow* r_window, double xpos, double ypos)
{
	auto ev = std::make_shared<MouseEvent>(
		Event::Type::MouseMove,
		Point(xpos, ypos), MouseButton::None,
		getMouseButtons(r_window),
		getKeyModifiers(r_window));

	WidgetCore* widget = getWindow(r_window);
	auto found = false;

	while(!found) {
		if (widget->geometry().contains(ev->pos)) {
			
		}
	}
	App->dispatchEvent(
		widget,
		ev
		);
}

static void mousePressCallback(GLFWwindow* r_window, int button, int action, int mods) {
	auto ev_type = action == GLFW_PRESS ? Event::Type::MouseButtonPress : Event::Type::MouseButtonRelease;
	auto m_button = MouseButton::None;
	auto modifiers = KeyModifier::None;

	switch(button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		m_button = MouseButton::Left;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		m_button = MouseButton::Right;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		m_button = MouseButton::Middle;
		break;
	default:
		return;
	}

	if (mods & GLFW_MOD_SHIFT) {
		modifiers |= KeyModifier::Shift;
	}

	if (mods & GLFW_MOD_CONTROL) {
		modifiers |= KeyModifier::Control;
	}

	if (mods & GLFW_MOD_ALT) {
		modifiers |= KeyModifier::Alt;
	}

	if (mods & GLFW_MOD_SUPER) {
		modifiers |= KeyModifier::Meta;
	}

	auto window = getWindow(r_window);
	PointD m_pos;
	glfwGetCursorPos(r_window, &m_pos.x, &m_pos.y);
	App->dispatchEvent(window,
		std::make_shared<MouseEvent>(
			ev_type,
			Point(m_pos), m_button,
			getMouseButtons(r_window),
			modifiers));

}

// Callbacks end

Window::Window(Size s, Window* parent) : WidgetCore(parent) {
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
	glfwSetCursorPosCallback(r_window, mouseMoveCallback);
	glfwSetMouseButtonCallback(r_window, mousePressCallback);

	is_widget = false;
	is_window = true;
	parent_window = this;

	setActive(); // needed to init glew properly
	if (!_inited) {
#ifdef OS_WINDOWS
		glewExperimental = GL_TRUE;
		auto glew_result = glewInit();
		if (glew_result != GLEW_OK) {
			throw std::runtime_error("Failed to init glew");
		}
		// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
		glGetError();
#endif
		_inited = true;
	}

	painter = std::make_unique<Painter>(this);

	top_bar = std::make_unique<TopBar>();

	Window::move(Point(500, 300));
	Window::resizeEvent(std::make_shared<ResizeEvent>(Event::Type::Resize, s, LayoutCore::size()));
	top_bar->setWindow(this);
	setObjectName("Window");
}

Window::~Window() {
	if (r_window) {
		glfwDestroyWindow(r_window);
	}
}

void Window::update() {
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
	WidgetCore::update();
	if (top_bar) {
		top_bar->update();
	}
	painter->end();
	if (r_window) {
		glfwSwapBuffers(r_window);
	}
}

//Point Window::pos() {
//	return r_window->getPosition();
//}

void Window::move(Point new_p) {
	WidgetCore::move(new_p);
	auto p = pos();
	glfwSetWindowPos(r_window, p.x, p.y);
}

Rect Window::contentGeometry()
{
	auto r = geometry();
	r.height -= top_bar->size().height;
	return r;
}

void Window::resizeEvent(ResizeEventPtr ev) {
	LayoutCore::resizeEvent(ev);
	auto s = size();
	glfwSetWindowSize(r_window, s.width, s.height); // TODO: confirm size
}

void Window::paint(Painter& painter) {
	gBrush b(painter);
	b.setColor(gColor(250, 250, 250, 200));

	painter.drawRect(Rect(Point(0, 0), size()));
}

void Window::setActive() const {
	glfwMakeContextCurrent(r_window);
}

