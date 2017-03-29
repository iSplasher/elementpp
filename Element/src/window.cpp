#include "element/window.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "element/core/painter.h" // necessary to include AFTER glew

USING_NAMESPACE

USING_NAMESPACE_PRIV

// helper functions

static float getPixelRatio( SDL_Window* window ) {
	float dpi = 1;
	auto idx = SDL_GetWindowDisplayIndex(window);
	if (idx > -1) {
		if (SDL_GetDisplayDPI(idx, &dpi, nullptr, nullptr))
			SDL_Log("Failed to query for DPI: %s\n", SDL_GetError());
	} else {
		SDL_Log("Failed to query for DPI: %s\n", SDL_GetError());
	}
	return dpi;
}

static Window* getWindow(SDL_Window* r_window ) {
	return static_cast< Window* >( SDL_GetWindowData( r_window, _winpointer.c_str() ) );
}

Direction PRIV_NAMESPACE::windowHitTestHelper(Window* window, Point point) {
	return Window::inResizeRangeHelper(window, point);
}

static SDL_HitTestResult windowHitTest(SDL_Window* r_window, const SDL_Point* point, void* data) {
	auto window = getWindow(r_window);
	switch(windowHitTestHelper(window, Point(point->x, point->y))) {
		case Direction::Any: break;
		case Direction::Left:
			return SDL_HITTEST_RESIZE_LEFT;
		case Direction::Top: 
			return SDL_HITTEST_RESIZE_TOP;
		case Direction::Right: 
			return SDL_HITTEST_RESIZE_RIGHT;
		case Direction::Bottom: 
			return SDL_HITTEST_RESIZE_BOTTOM;
		case Direction::Default: 
			return SDL_HITTEST_DRAGGABLE;
		default: 
			return SDL_HITTEST_RESIZE_BOTTOMLEFT;
	}
}


//static MouseButton getMouseButtons( GLFWwindow* r_window ) {
//	MouseButton buttons = MouseButton::None;
//
//	auto left = glfwGetMouseButton( r_window, GLFW_MOUSE_BUTTON_LEFT );
//	auto right = glfwGetMouseButton( r_window, GLFW_MOUSE_BUTTON_RIGHT );
//	auto middle = glfwGetMouseButton( r_window, GLFW_MOUSE_BUTTON_MIDDLE );
//
//	if( left == GLFW_PRESS ) {
//		buttons |= MouseButton::Left;
//	}
//
//	if( right == GLFW_PRESS ) {
//		buttons |= MouseButton::Right;
//	}
//
//	if( middle == GLFW_PRESS ) {
//		buttons |= MouseButton::Middle;
//	}
//
//	if( left == GLFW_PRESS && left == right == middle ) {
//		buttons = MouseButton::Any;
//	}
//
//	return buttons;
//}

//static KeyModifier getKeyModifiers( GLFWwindow* r_window ) {
//	KeyModifier modifiers = KeyModifier::None;
//
//	auto shift = glfwGetKey( r_window, GLFW_MOD_SHIFT );
//	auto control = glfwGetKey( r_window, GLFW_MOD_CONTROL );
//	auto alt = glfwGetKey( r_window, GLFW_MOD_ALT );
//	auto meta = glfwGetKey( r_window, GLFW_MOD_SUPER );
//
//	if( shift == GLFW_PRESS ) {
//		modifiers |= KeyModifier::Shift;
//	}
//
//	if( control == GLFW_PRESS ) {
//		modifiers |= KeyModifier::Control;
//	}
//
//	if( alt == GLFW_PRESS ) {
//		modifiers |= KeyModifier::Alt;
//	}
//
//	if( meta == GLFW_PRESS ) {
//		modifiers |= KeyModifier::Meta;
//	}
//
//	return modifiers;
//}

Window::Window( Rect win_rect, Window* parent ) : Widget( parent ) {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 12);

	r_window = SDL_CreateWindow(
		"Element++",
		win_rect.pos() == 0 ? SDL_WINDOWPOS_CENTERED : win_rect.pos().x,
		win_rect.pos() == 0 ? SDL_WINDOWPOS_CENTERED : win_rect.pos().y,
		win_rect.width, win_rect.height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_BORDERLESS);

	if( !r_window ) {
		SDL_Log("Failed to create window: %s\n", SDL_GetError());
		return;
	}

	context = SDL_GL_CreateContext(r_window);

	SDL_SetWindowData( r_window, _winpointer.c_str(), this );
	SDL_SetWindowHitTest(r_window, windowHitTest, nullptr);

	setType( ElementType::Window );
	parent_window = this;

	setActive(); // needed to init glew properly
	if( !_inited ) {
#ifdef OS_WINDOWS
		glewExperimental = GL_TRUE;
		auto glew_result = glewInit();
		if( glew_result != GLEW_OK ) {
			SDL_Log("Failed to initialize glew");
		}
		// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
		glGetError();
#endif
		_inited = true;
	}

	pixelRatio.changed( [](float p) { setPixelRatio( p ); } );
	pixelRatio = getPixelRatio( r_window );
	hresize_cursor = std::make_unique< PRIV_NAMESPACE::_Cursor >( Cursor::HResize, r_window );
	vresize_cursor = std::make_unique< PRIV_NAMESPACE::_Cursor >( Cursor::VResize, r_window );
	objectName = "Window";
	isDraggable = true;
	isResizeable = true;
	painter = std::make_unique< Painter >( this );
	current_pos = position = win_rect.pos();
	size = win_rect.size();
	backgroundColor = Color( 54, 54, 54 );
	borderLeft = borderTop = borderRight = borderBottom = 0;
	marginLeft = marginTop = marginRight = marginBottom = 0;
	paddingLeft = paddingTop = paddingRight = paddingBottom = 0;
}

Window::~Window() {
	if( r_window ) {
		SDL_DestroyWindow( r_window );
	}
	if(context) {
		SDL_GL_DeleteContext(context);
	}
}

void Window::update() {
	if( App && !( App->threadInitedIn == std::this_thread::get_id() ) ) {
		std::cerr << "Window thread and Application thread differ, cannot update." << std::endl; // TODO: maybe log too?
		return;
	}

	updateGeometry();

	int fb_width;
	int fb_height;
	SDL_GL_GetDrawableSize(r_window, &fb_width, &fb_height);
	glViewport( 0, 0, fb_width, fb_height );
	glClearColor( 0, 0, 0, 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_CULL_FACE );

	pixelRatio = getPixelRatio( r_window );

	painter->begin( pixelRatio );
	Widget::update();
	painter->end();
	if( r_window ) {
		SDL_GL_SwapWindow( r_window );
	}
}

void Window::setActive() const {
	SDL_GL_MakeCurrent( r_window, context );
}

void Window::updateGeometry() {
	if( current_pos != position ) {
		current_pos = position;
		SDL_SetWindowPosition( r_window, std::floor( current_pos.x ), std::floor( current_pos.y ) );
	}

	if( current_size != size ) {
		current_size = size;
		SDL_SetWindowSize( r_window, std::floor( current_size.width ), std::floor( current_size.height ) );
	}

}

void Window::applyWidgetCursor( Widget* w ) {
	if( !w->_cursor )
		w->_cursor.reset( new PRIV_NAMESPACE::_Cursor( w->cursor, w->parent_window->r_window ) );
	w->_cursor->apply( w->cursor );
}

void Window::applyWidgetResizeCursor( Widget* w, Direction dir ) {
	switch( dir ) {
		case Direction::Right:
		case Direction::Left:
			w->parent_window->hresize_cursor->apply();
			break;
		case Direction::Top:
		case Direction::Bottom:
			w->parent_window->vresize_cursor->apply();
			break;
	}
}

bool Window::resizeHelper( Widget* w, Point p, MouseButton buttons ) {
	if( flags( buttons & MouseButton::Left ) && w->parent_window->grabbed_widget == w ) {
		p = w->mapFromWindow( p );

		auto new_rect = w->geometry.get();

		auto delta_pos = p - w->resize_pos;

		switch( w->resize_dir ) {
			case Direction::Left:
				new_rect.x += delta_pos.x;
				new_rect.width -= delta_pos.x;
				w->resize_pos = Point( 0, p.y );
				break;
			case Direction::Top:
				new_rect.y += delta_pos.y;
				new_rect.height -= delta_pos.y;
				w->resize_pos = Point( p.x, 0 );
				break;
			case Direction::Right:
				new_rect.width += delta_pos.x;
				w->resize_pos = p;
				break;
			case Direction::Bottom:
				new_rect.height += delta_pos.y;
				w->resize_pos = p;
				break;
		}

		//std::cout << "Current Pos: " << new_rect << " Delta Pos: " << delta_pos <<  std::endl;
		if( w->type == ElementType::Window || w->positionType == Position::Absolute ) {
			w->position = new_rect.pos(); // important this is assigned to first. Size triggeres resize event which fetches position.
		}
		if( w->type == ElementType::Window ) {
			w->size = new_rect.size();
		}
		else {
			w->maxSize = new_rect.size();
		}

	}
	return true;
}

Direction Window::inResizeRangeHelper( Widget* w, Point p ) {
	auto r = w->geometry.get();
	if( w->type == ElementType::Window ) {
		r = Point();
	}
	Direction dir = Direction::None;
	auto resize_range = 6.0f;
	if( ( p.x - r.x ) < resize_range )
		dir = Direction::Left;
	else if( ( r.x + r.width - p.x ) < resize_range )
		dir = Direction::Right;
	else if( ( p.y - r.y ) < resize_range )
		dir = Direction::Top;
	else if( ( r.y + r.height - p.y ) < resize_range )
		dir = Direction::Bottom;

	return dir;
}

//void Window::windowResizedCb( _privRWindow* r_window, int width, int height ) {
//	auto win = getWindow( r_window );
//	if( !win->blockEvents ) {
//		//win->resized = Rect( win->position, Size( width, height ) );
//	}
//}
//
//void Window::windowMovedCb( _privRWindow* r_window, int xpos, int ypos ) {
//	auto win = getWindow( r_window );
//	if( !win->blockEvents ) {
//		win->windowMovedHelper( Point( xpos, ypos ) );
//	}
//}
//
//void Window::mouseMovedCb( _privRWindow* r_window, double xpos, double ypos ) {
//	Point p = Point( xpos, ypos );
//	// It's necessary to get the point in screen space to avoid jittering when moving an undecorated window.
//#ifdef OS_WINDOWS
//	auto hwnd = glfwGetWin32Window( r_window );
//	POINT pos;
//	pos.x = p.x;
//	pos.y = p.y;
//	ClientToScreen( hwnd, &pos );
//	p.x = pos.x;
//	p.y = pos.y;
//#else
//	static_assert(false, "Coordinates in screen space has not yet been implemented on this platform")
//#endif
//	auto buttons = getMouseButtons( r_window );
//	auto win = getWindow( r_window );
//	p = win->mapFromScreen( p );
//	mouseMovedHelper( win, p, buttons );
//}

void Window::mouseMovedHelper( Widget* w, Point p, MouseButton buttons ) {
	if( w->blockEvents )
		return;

	w->mouseMoved = MouseEvent{ w->mapFromWindow( p ), buttons, w };

	auto innermost = true; // this child is the innermost

	for( auto& c : w->children() ) { // go through all children
		if( c->type == ElementType::Widget ) { // we don't want child windows to get this event
			auto c_w = static_cast< Widget* >( c );
			if( c_w->geometry.get().contains( p ) ) { // only if widget contains point
				mouseMovedHelper( c_w, p, buttons ); // now repeat this for its children
				innermost = false;
				break; // since this widget contains this point, we don't need to go through its siblings
			}
		}
	}

	if( innermost ) {
		if( w->parent_window->grabbed_widget ) // we are still resizing, don't stop
			resizeHelper( w->parent_window->grabbed_widget, p, buttons );
		else if( w->isResizeable ) {
			auto dir = inResizeRangeHelper( w, p );
			if( dir != Direction::None ) {
				applyWidgetResizeCursor( w, dir );
			}
			else
				applyWidgetCursor( w );
		}
		else
			applyWidgetCursor( w );
	}

}

//void Window::mousePressCb( _privRWindow* r_window, int button, int action, int mods ) {
//	auto btn_press = action == GLFW_PRESS ? true : false;
//	auto m_button = MouseButton::None;
//	auto modifiers = KeyModifier::None;
//
//	switch( button ) {
//		case GLFW_MOUSE_BUTTON_LEFT:
//			m_button = MouseButton::Left;
//			break;
//		case GLFW_MOUSE_BUTTON_RIGHT:
//			m_button = MouseButton::Right;
//			break;
//		case GLFW_MOUSE_BUTTON_MIDDLE:
//			m_button = MouseButton::Middle;
//			break;
//		default:
//			return;
//	}
//
//	if( mods & GLFW_MOD_SHIFT ) {
//		modifiers |= KeyModifier::Shift;
//	}
//
//	if( mods & GLFW_MOD_CONTROL ) {
//		modifiers |= KeyModifier::Control;
//	}
//
//	if( mods & GLFW_MOD_ALT ) {
//		modifiers |= KeyModifier::Alt;
//	}
//
//	if( mods & GLFW_MOD_SUPER ) {
//		modifiers |= KeyModifier::Meta;
//	}
//
//	PointD m_pos_d;
//	glfwGetCursorPos( r_window, &m_pos_d.x, &m_pos_d.y );
//
//	auto wind = getWindow( r_window );
//	auto m_pos = Point( m_pos_d );
//
//	auto now = std::chrono::steady_clock::now();
//	auto click_delta = std::chrono::duration_cast< std::chrono::milliseconds >( now - wind->last_pressed ).count();
//	auto release_delta = std::chrono::duration_cast< std::chrono::milliseconds >( now - wind->last_released ).count();
//
//	auto click = false, d_click = false, d_press = false;
//	if( wind->button_press_rect.contains( m_pos ) ) { // only if cursor is still within rect
//
//		// if release, check if time since last press is within click interval
//		if( !btn_press ) {
//
//			if( click_delta > 20 && click_delta < App->clickInterval && flags( m_button & wind->last_pressed_buttons ) )
//				click = true;
//
//			if( release_delta > 20 && release_delta < App->doubleClickInterval && flags( m_button & wind->last_pressed_buttons ) )
//				d_click = true;
//		}
//		else {
//			if( release_delta > 20 && release_delta < App->doubleClickInterval && flags( m_button & wind->last_pressed_buttons ) )
//				d_press = true;
//		}
//	}
//
//	wind->last_pressed_buttons = m_button;
//	if( btn_press ) {
//		wind->button_press_rect = Rect( m_pos - 1.5f, 3.0f, 3.0f );
//		wind->last_pressed = now;
//	}
//	else {
//		wind->last_released = now;
//	}
//
//	mousePressedHelper( wind, btn_press, m_button, modifiers, m_pos, click, d_click, d_press );
//
//}

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

	auto innermost = true; // this child is the innermost

	for( auto c : w->children() ) { // go through all children
		if( c->type == ElementType::Widget ) { // we don't want child windows to get this event
			auto c_w = static_cast< Widget* >( c );
			if( c_w->geometry.get().contains( p ) ) { // only if widget contains point
				mousePressedHelper( c_w, btn_press, buttons, modifiers, p, click, d_click, d_press ); // now repeat this for its children
				innermost = false;
				break; // since this widget contains this point, we don't need to go through its siblings
			}
		}
	}

	if( innermost ) {
		if( !btn_press )
			w->parent_window->grabbed_widget = nullptr;

		if( !w->parent_window->grabbed_widget && btn_press ) {
			w->resize_dir = inResizeRangeHelper( w, p );
			if( w->isResizeable && w->resize_dir != Direction::None ) {
				w->parent_window->grabbed_widget = w;
				if( flags( buttons & MouseButton::Left ) )
					w->resize_pos = w->mapFromWindow( p );
			}
		}
	}

}

PRIV_NAMESPACE::_Cursor::_Cursor( Cursor c, _privRWindow* r_window ) {
	this->r_window = r_window;
	_newCursor( c );
}

void priv::_Cursor::apply( Cursor c ) {
	if( c != old_cursor ) {
		_destroy();
		_newCursor( c );
	}
	//if( r_window ) {
	//	if( cursor )
	//		glfwSetCursor( r_window, cursor );
	//	else
	//		glfwSetCursor( r_window, nullptr );
	//}
}

void priv::_Cursor::apply() {
	apply( old_cursor );
}

void priv::_Cursor::_newCursor( Cursor c ) {
	old_cursor = c;
	if( !flags( c & Cursor::None ) ) {
		switch( c ) {

			case Cursor::None:
				cursor = nullptr;
				break;
			case Cursor::Arrow:
				break;
			case Cursor::IBeam:
				break;
			case Cursor::Cross:
				break;
			case Cursor::Hand:
				break;
			case Cursor::HResize:
				break;
			case Cursor::VResize:
				break;
			default:
				// implementation idea:
				/*
				* get cursor image and size like this, from application
				* App->registeredCursor(Cursor::User)
				*
				* User should register new cursor like this
				* App->registerCursor(Cursor::User+n, image, size);
				*/
				break;
		}
	}
}

void priv::_Cursor::_destroy() {
	/*if( cursor )
		glfwDestroyCursor( cursor );*/
	cursor = nullptr;
}

PRIV_NAMESPACE::_Cursor::~_Cursor() {
	_destroy();
}
