#include "element/window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#ifdef OS_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

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
		buttons = MouseButton::Any;
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

Window::Window( Rect win_rect, Window* parent ) : Widget( parent ) {
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
	r_window = glfwCreateWindow( win_rect.width, win_rect.height, "Element++", nullptr, nullptr );

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
	isDraggable = true;
	painter = std::make_unique< Painter >( this );
	current_pos = position = win_rect.pos();
	size = win_rect.size();
	backgroundColor = Color( 250, 250, 250, 200 );
	borderLeft = borderTop = borderRight = borderBottom = 0;
	marginLeft = marginTop = marginRight = marginBottom = 0;
	paddingLeft = paddingTop = paddingRight = paddingBottom = 0;
	glfwSetWindowPos( r_window, win_rect.x, win_rect.y );
}

Window::~Window() {
	if( r_window ) {
		glfwDestroyWindow( r_window );
	}
}

void Window::update() {
	if(App && !(App->threadInitedIn == std::this_thread::get_id())) {
		std::cerr << "Window thread and Application thread differ, cannot update." << std::endl; // TODO: maybe log too?
		return;
	}

	updateGeometry();

	int fb_width;
	int fb_height;
	Size s = size;

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

void Window::updateGeometry() {
	if (current_pos != position) {
		current_pos = position;
		glfwSetWindowPos(r_window, std::floor(current_pos.x), std::floor(current_pos.y));
	}

	if (current_size != size) {
		current_size = size;
		glfwSetWindowSize(r_window, std::floor(current_size.width), std::floor(current_size.height));
	}
		
}

void Window::mouseMovedCb( _privRWindow* r_window, double xpos, double ypos ) {
	Point p = Point( xpos, ypos );
	 // It's necessary to get the point in screen space to avoid jittering when moving an undecorated window.
#ifdef OS_WINDOWS
	auto hwnd = glfwGetWin32Window(r_window);
	POINT pos;
	pos.x = p.x;
	pos.y = p.y;
	ClientToScreen(hwnd, &pos);
	p.x = pos.x;
	p.y = pos.y;
#else
	static_assert(false, "Coordinates in screen space has not yet been implemented on this platform")
#endif
	auto buttons = getMouseButtons( r_window );
	auto win = getWindow(r_window);
	p = win->mapFromScreen(p);
	mouseMovedHelper( win, p, buttons );
}

void Window::mouseMovedHelper( Widget* w, Point p, MouseButton buttons ) {
	if( w->blockEvents )
		return;


	w->mouseMoved = MouseEvent{ w->mapFromWindow( p ), buttons, w };

	for( auto c : w->children() ) { // go through all children
		if( c->type == ElementType::Widget ) { // we don't want child windows to get this event
			auto c_w = static_cast< Widget* >( c );
			if( c_w->geometry.get().contains( p ) ) { // only if widget contains point
				mouseMovedHelper( c_w, p, buttons ); // now repeat this for its children
				break; // since this widget contains this point, we don't need to go through its siblings
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

	PointD m_pos_d;
	glfwGetCursorPos( r_window, &m_pos_d.x, &m_pos_d.y );

	auto wind = getWindow( r_window );
	auto m_pos = Point( m_pos_d );

	auto now = std::chrono::steady_clock::now();
	auto click_delta = std::chrono::duration_cast< std::chrono::milliseconds >( now - wind->last_pressed ).count();
	auto release_delta = std::chrono::duration_cast< std::chrono::milliseconds >( now - wind->last_released ).count();

	auto click = false, d_click = false, d_press = false;
	if( wind->button_press_rect.contains( m_pos ) ) { // only if cursor is still within rect

		// if release, check if time since last press is within click interval
		if( !btn_press ) {

			if( click_delta > 20 && click_delta < App->clickInterval && flags( m_button & wind->last_pressed_buttons ) )
				click = true;

			if( release_delta > 20 && release_delta < App->doubleClickInterval && flags( m_button & wind->last_pressed_buttons ) )
				d_click = true;
		}
		else {
			if( release_delta > 20 && release_delta < App->doubleClickInterval && flags( m_button & wind->last_pressed_buttons ) )
				d_press = true;
		}
	}

	wind->last_pressed_buttons = m_button;
	if( btn_press ) {
		wind->button_press_rect = Rect( m_pos - 1.5f, 3.0f, 3.0f );
		wind->last_pressed = now;
	}
	else {
		wind->last_released = now;
	}

	mousePressedHelper( wind, btn_press, m_button, modifiers, m_pos, click, d_click, d_press );

}

void Window::mousePressedHelper( Widget* w, bool btn_press, MouseButton buttons, KeyModifier modifiers, Point p, bool click, bool d_click, bool d_press ) {
	if( w->blockEvents )
		return;

	auto m_event = MouseEvent( w->mapFromWindow( p ), buttons, w );
	if( btn_press ) {
		if( flags( buttons & MouseButton::Any ) )
			w->pressed = m_event;
		if( flags( buttons & MouseButton::Left ) )
			w->leftPress = p;
		if( flags( buttons & MouseButton::Right ) )
			w->rightPress = p;

	}
	else {
		if( flags( buttons & MouseButton::Any ) )
			w->release = m_event;
		if( flags( buttons & MouseButton::Left ) )
			w->leftRelease = p;
		if( flags( buttons & MouseButton::Right ) )
			w->rightRelease = p;

	}

	if( click ) {
		if( flags( buttons & MouseButton::Any ) )
			w->click = m_event;
		if( flags( buttons & MouseButton::Left ) )
			w->leftClick = p;
		if( flags( buttons & MouseButton::Right ) )
			w->rightClick = p;
	}

	if( d_click ) {
		if( flags( buttons & MouseButton::Any ) )
			w->doubleClick = m_event;
		if( flags( buttons & MouseButton::Left ) )
			w->leftDoubleClick = p;
		if( flags( buttons & MouseButton::Right ) )
			w->rightDoubleClick = p;

	}

	if( d_press ) {
		if( flags( buttons & MouseButton::Any ) )
			w->doublePress = m_event;
		if( flags( buttons & MouseButton::Left ) )
			w->leftDoublePress = p;
		if( flags( buttons & MouseButton::Right ) )
			w->rightDoublePress = p;
	}

	for( auto c : w->children() ) { // go through all children
		if( c->type == ElementType::Widget ) { // we don't want child windows to get this event
			auto c_w = static_cast< Widget* >( c );
			if( c_w->geometry.get().contains( p ) ) { // only if widget contains point
				mousePressedHelper( c_w, btn_press, buttons, modifiers, p, click, d_click, d_press ); // now repeat this for its children
				break; // since this widget contains this point, we don't need to go through its siblings
			}
		}
	}
}
