#include "../include/Widget.h"
#include "../include/Event.h"

#include <Windows.h>
#include <iostream>

USING_NAMESPACE

gCoreWidget::gCoreWidget(gCoreWidget* parent) :
	gCore(parent),
	style(0, 0, 200, 200) {
	is_widget = true;
	parent_widget = parent;
}

void gCoreWidget::update() {
}

void gCoreWidget::event(EventPtr ev) {
	// TODO: send events to children here
	switch (ev->type()) {
	case gEvent::MouseMove:
		mouseMoveEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	case gEvent::MouseButtonPress:
		mousePressEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	case gEvent::MouseButtonRelease:
		mouseReleaseEvent(std::static_pointer_cast<gMouseEvent>(ev));
		break;
	}
}

Point gCoreWidget::pos() {
	return Point(0, 0);
}

void gCoreWidget::move(Point new_p) {
}

void gCoreWidget::resize(Size new_s) {
	style.size = new_s;
}

Point gCoreWidget::mapToGlobal(Point p) {
	auto c_pos = pos();
	Point n_pos(c_pos.x + p.x, c_pos.y + p.y);
	return n_pos;
}

void gCoreWidget::mousePressEvent(MouseEventPtr ev){
	printf("A button was pressed!");
	move_state = MoveState::Moving;
	move_offset = ev->pos();
}

void gCoreWidget::mouseMoveEvent(MouseEventPtr ev) {
	//printf("x=%d y=%d\n", ev->global_x, ev->global_y);
	if (ev->x >= 0 && ev->x < width() && ev->y >= 0 && ev->y < height()) {
		under_mouse = true;
	} else {
		under_mouse = false;
	}

	if (move_state == MoveState::Moving) {
		move(mapToGlobal(ev->pos()) - move_offset);
	}

	if (underMouse()) {
		style.bg_color = Color::Green;
	} else {
		style.bg_color = Color::Transparent;
	}
}

void gCoreWidget::mouseReleaseEvent(MouseEventPtr ev) {
	printf("A button was released!");
	move_state = MoveState::Normal;
}

unsigned window_count = 0;

LRESULT CALLBACK globalCallback(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Associate handle and Window instance when the creation message is received
	if (message == WM_CREATE)
	{
		// Get Gwindow instance (it was passed as the last argument of CreateWindow)
		LONG_PTR window = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams;

		// Set as the "user data" parameter of the window
		SetWindowLongPtrW(handle, GWLP_USERDATA, window);
	}

	//// Get the WindowImpl instance corresponding to the window handle
	//gWindow* window = handle ? reinterpret_cast<gWindow*>(GetWindowLongPtr(handle, GWLP_USERDATA)) : NULL;

	//// Forward the event to the appropriate function
	//if (window)
	//{
	//	window->processEvent(message, wParam, lParam);

	//	if (window->m_callback)
	//		return CallWindowProcW(reinterpret_cast<WNDPROC>(window->m_callback), handle, message, wParam, lParam);
	//}

	// We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
	if (message == WM_CLOSE)
		return 0;

	// Don't forward the menu system command, so that pressing ALT or F10 doesn't steal the focus
	if ((message == WM_SYSCOMMAND) && (wParam == SC_KEYMENU))
		return 0;

	return DefWindowProcW(handle, message, wParam, lParam);
}

gWindow::gWindow(gWindow* parent) :
	gCoreWidget(parent) {
	
	// TODO: move this windows specific code elsewhere
	if (window_count == 0) {
		WNDCLASSW window_class;
		window_class.style = CS_DROPSHADOW;
		window_class.lpfnWndProc = &globalCallback;
		window_class.cbClsExtra = 0;
		window_class.cbWndExtra = 0;
		window_class.hInstance = GetModuleHandleW(NULL);
		window_class.hIcon = NULL;
		window_class.hCursor = 0;
		window_class.hbrBackground = 0;
		window_class.lpszMenuName = NULL;
		window_class.lpszClassName = L"gSplasher_Window";
		RegisterClassW(&window_class);
	}

	DWORD win_style = WS_EX_LAYERED | WS_POPUP;

	RECT rect = { 0, 0, style.size.width, style.size.height };
	AdjustWindowRect(&rect, win_style, false);
	auto width = rect.right - rect.left;
	auto height = rect.bottom - rect.top;

	HWND w_hwnd = CreateWindowExW(
		WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		L"gSplasher_Window",
		nullptr,
		WS_POPUP | WS_VISIBLE,
		style.pos.x,
		style.pos.y,
		width,
		height,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		this
		);

	if (w_hwnd != nullptr) {
		printf("OK");
	} else {
		printf("%u", GetLastError());
	}

	r_window.reset(new sf::RenderWindow(w_hwnd));
	r_window->setVerticalSyncEnabled(true);
	setTransparency(style.base_alpha);
	redraw();
	is_widget = false;
	is_window = true;
	++window_count;
}

void gWindow::update() {
	MSG message;
	while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}

	using SE = sf::Event;
	SE sfev;
	while (r_window->pollEvent(sfev)) {
		EventPtr ev(nullptr);
		switch (sfev.type) {
		case SE::MouseButtonPressed:
		case SE::MouseButtonReleased:
			ev.reset(new gMouseEvent(sfev));
			break;
		case SE::KeyPressed:
		case SE::KeyReleased:
			ev.reset(new gKeyEvent(sfev));
			break;
		default:
			goto continue_exec;
			ev.reset(new gEvent(sfev));
			break;
		}
		gApp->dispatchEvent(this, ev);
	}

continue_exec:
	generateMouseMove();
	r_window->clear(style.bg_color);
	//paint();
	r_window->display();
}

Point gWindow::pos() {
	return r_window->getPosition();
}

void gWindow::move(Point new_p) {
	r_window->setPosition(new_p);
	style.pos = new_p;
}

void gWindow::resize(Size new_s) {
	r_window->setSize(new_s);
	style.size = new_s;
}

bool gWindow::setTransparency(unsigned char alpha) const {
	auto hWnd = r_window->getSystemHandle();
	SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
	return true;
}

void gWindow::redraw() {

	int m = 20;
	int btm_x = style.pos.x + style.size.width - (m+3);
	int btm_y = style.pos.y + style.size.height;
	int c_offset_x = style.pos.x + style.size.width - m;
	int c_offset_y = style.pos.y;

	HRGN window_shape = CreateRoundRectRgn(
		style.pos.x,
		style.pos.y,
		btm_x,
		btm_y,
		5,
		5);

	HRGN exit_shape = CreateEllipticRgn(
		c_offset_x,
		c_offset_y,
		c_offset_x + m,
		c_offset_y + m
		);

	c_offset_y += 10;
	HRGN minimize_shape = CreateEllipticRgn(
		c_offset_x,
		c_offset_y,
		c_offset_x + m,
		c_offset_y + m
		);

	CombineRgn(window_shape, exit_shape, window_shape, RGN_OR);


	SetWindowRgn(r_window->getSystemHandle(), window_shape, true);
	DeleteObject(window_shape);
	DeleteObject(exit_shape);
	DeleteObject(minimize_shape);
}

void gWindow::generateMouseMove() {
	// TODO: set a class instance switch? enableMouseTracking..?
	if (r_window) {
		Point pos = Mouse::getPosition();
		if (pos.x != _old_mouse_x || pos.y != _old_mouse_y) {
			_old_mouse_x = pos.x;
			_old_mouse_y = pos.y;
			Point r_pos = Mouse::getPosition(*r_window);
			MouseEventPtr m_ev = std::make_shared<gMouseEvent>(gEvent::MouseMove, r_pos);
			gApp->dispatchEvent(this, m_ev); // TODO: make this instantous? 
		}
	}
}

