#include "element/window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#ifdef OS_WINDOWS
//#define GLFW_EXPOSE_NATIVE_WIN32
//#define GLFW_EXPOSE_NATIVE_WGL
//#include <GLFW/glfw3native.h>
//#endif

#include "element/core/painter.h"

USING_NAMESPACE

// Callbacks start

// helper functions

static Window* getWindow( GLFWwindow* r_window ) {
	return static_cast< Window* >( glfwGetWindowUserPointer( r_window ) );
}

static MouseButton getMouseButtons( GLFWwindow* r_window ) {
	MouseButton buttons = MouseButton::None;

	auto left = glfwGetMouseButton( r_window, GLFW_MOUSE_BUTTON_LEFT );
	auto right = glfwGetMouseButton( r_window, GLFW_MOUSE_BUTTON_RIGHT );
	auto middle = glfwGetMouseButton( r_window, GLFW_MOUSE_BUTTON_MIDDLE );

	if( left == GLFW_PRESS ) {
		buttons |= MouseButton::Left;
	}

	if( right == GLFW_PRESS ) {
		buttons |= MouseButton::Right;
	}

	if( middle == GLFW_PRESS ) {
		buttons |= MouseButton::Middle;
	}

	if( left == GLFW_PRESS && left == right == middle ) {
		buttons = MouseButton::All;
	}

	return buttons;
}

static KeyModifier getKeyModifiers( GLFWwindow* r_window ) {
	KeyModifier modifiers = KeyModifier::None;

	auto shift = glfwGetKey( r_window, GLFW_MOD_SHIFT );
	auto control = glfwGetKey( r_window, GLFW_MOD_CONTROL );
	auto alt = glfwGetKey( r_window, GLFW_MOD_ALT );
	auto meta = glfwGetKey( r_window, GLFW_MOD_SUPER );

	if( shift == GLFW_PRESS ) {
		modifiers |= KeyModifier::Shift;
	}

	if( control == GLFW_PRESS ) {
		modifiers |= KeyModifier::Control;
	}

	if( alt == GLFW_PRESS ) {
		modifiers |= KeyModifier::Alt;
	}

	if( meta == GLFW_PRESS ) {
		modifiers |= KeyModifier::Meta;
	}

	return modifiers;
}

Window::Window( SizeF win_size, Window* parent ) : Widget( parent ) {
#ifndef OS_WINDOWS
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	glfwWindowHint( GLFW_DEPTH_BITS, 16 );
	glfwWindowHint( GLFW_ALPHA_BITS, 8 );
	glfwWindowHint( GLFW_TRANSPARENT, true );
	glfwWindowHint( GLFW_DECORATED, false );
	glfwWindowHint( GLFW_SAMPLES, 12 );
	r_window = glfwCreateWindow( win_size.width, win_size.height, "Element++", nullptr, nullptr );

	if( !r_window ) {
		throw std::runtime_error( "Could not create window" );
	}

	glfwSetWindowUserPointer( r_window, this );
	glfwSetCursorPosCallback( r_window, mouseMovedCb );
	glfwSetMouseButtonCallback( r_window, mousePressCb );

	setType( ElementType::Window );
	parent_window = this;

	setActive(); // needed to init glew properly
	if( !_inited ) {
#ifdef OS_WINDOWS
		glewExperimental = GL_TRUE;
		auto glew_result = glewInit();
		if( glew_result != GLEW_OK ) {
			throw std::runtime_error( "Failed to init glew" );
		}
		// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
		glGetError();
#endif
		_inited = true;
	}

	objectName = "Window";
	painter = std::make_unique< Painter >( this );
	position.changed( [&](PointF p) {
		                 glfwSetWindowPos( r_window, p.x, p.y );
	                 } );
	size = win_size;
	size.changed( [&](SizeF s) { glfwSetWindowSize( r_window, s.width, s.height ); } ); // TODO: confirm size
	backgroundColor = Color( 250, 250, 250, 200 );
	borderLeft = borderTop = borderRight = borderBottom = 0;
	marginLeft = marginTop = marginRight = marginBottom = 0;
	paddingLeft = paddingTop = paddingRight = paddingBottom = 0;
	glfwSetWindowPos( r_window, 700, 450 );
}

Window::~Window() {
	if( r_window ) {
		glfwDestroyWindow( r_window );
	}
}

void Window::update() {
	int fb_width;
	int fb_height;
	SizeF s = size;

	glfwGetFramebufferSize( r_window, &fb_width, &fb_height );
	glViewport( 0, 0, fb_width, fb_height );
	glClearColor( 0, 0, 0, 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_CULL_FACE );

	float px_ratio = static_cast< float >( fb_width ) / static_cast< float >( s.width );

	painter->begin( px_ratio );
	Widget::update();
	painter->end();
	if( r_window ) {
		glfwSwapBuffers( r_window );
	}
}

void Window::setActive() const {
	glfwMakeContextCurrent( r_window );
}

void Window::mouseMovedCb( _privRWindow* r_window, double xpos, double ypos ) {
	PointF p = PointF( xpos, ypos );
	auto buttons = getMouseButtons( r_window );

	auto contains = [](Widget* a, PointF b) { a->geometry.get().contains(b) && !a->blockEvents; };

	// wigdets that contains point receives
	Widget* w_p = getWindow(r_window);
	while(w_p) {
		for( auto x : w->children() ) {
			if( x->type == ElementType::Widget ) {
				auto w = static_cast< Widget* >( x );
				if( w->geometry.get().contains( p ) && !w->blockEvents ) {
					w->mouseMoved = MouseEvent{ w->mapFromWindow( p ), buttons, w };
				}
			}
		}
	}

}

void Window::mousePressCb( _privRWindow* r_window, int button, int action, int mods ) {
	auto btn_press = action == GLFW_PRESS ? true : false;
	auto m_button = MouseButton::None;
	auto modifiers = KeyModifier::None;

	switch( button ) {
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

	if( mods & GLFW_MOD_SHIFT ) {
		modifiers |= KeyModifier::Shift;
	}

	if( mods & GLFW_MOD_CONTROL ) {
		modifiers |= KeyModifier::Control;
	}

	if( mods & GLFW_MOD_ALT ) {
		modifiers |= KeyModifier::Alt;
	}

	if( mods & GLFW_MOD_SUPER ) {
		modifiers |= KeyModifier::Meta;
	}

	PointD m_pos;
	glfwGetCursorPos( r_window, &m_pos.x, &m_pos.y );

	// window receives
	auto window = getWindow( r_window );

	// wigdets that contains point receives
	for (auto x : window->children()) {
		if (x->type == ElementType::Widget) {
			auto w = static_cast< Widget* >(x);
			if (w->geometry.get().contains(m_pos) && !w->blockEvents) {
			}
		}
	}

}
