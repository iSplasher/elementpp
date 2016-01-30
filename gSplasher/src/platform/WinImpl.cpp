#include "WinImpl.h"
#include <string>
#include <SFML/Graphics/Image.hpp>

#ifdef SFML_SYSTEM_WINDOWS
#include <Windows.h>
#endif


WinImpl::WinImpl()
{
}


WinImpl::~WinImpl()
{
}

// Windows Implementation

#if defined SFML_SYSTEM_WINDOWS

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

gWindowHandle WinImpl::createHandle(int x, int y, int width, int height) {
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

	RECT rect = { 0, 0, width, height };
	AdjustWindowRect(&rect, win_style, false);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	HWND w_hwnd = CreateWindowExW(
		WS_EX_LAYERED | WS_EX_TOOLWINDOW,
		L"gSplasher_Window",
		nullptr,
		WS_POPUP | WS_VISIBLE,
		x,
		y,
		width,
		height,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		this
		);
	++window_count;

	return w_hwnd;
}

void WinImpl::processEvents() {
	MSG message;
	while (PeekMessageW(&message, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
}

void WinImpl::setTransparency(gWindowHandle handle, unsigned char alpha) {
	SetLayeredWindowAttributes(handle, 0, alpha, LWA_ALPHA);
}

HRGN createShape(std::string path_to_file, int x, int y) {
	sf::Image image;
	image.loadFromFile(path_to_file) ? printf("true") : printf("false");

	int offset_x = x + image.getSize().x;
	int offset_y = y + image.getSize().y;

	const sf::Uint8* pixelData = image.getPixelsPtr();
	HRGN hRegion = CreateRectRgn(x, y, offset_x, offset_y);

	// Determine the visible region
	for (unsigned y2 = 0; y2 < image.getSize().y; y2++)
	{
		for (unsigned x2 = 0; x2 < image.getSize().x; x2++)
		{
			if (pixelData[y2 * image.getSize().x * 4 + x2 * 4 + 3] == 0)
			{
				HRGN hRegionPixel = CreateRectRgn(x+x2, y+y2, x+x2 + 1, y+y2 + 1);
				CombineRgn(hRegion, hRegion, hRegionPixel, RGN_XOR);
				DeleteObject(hRegionPixel);
			}
		}
	}

	// WARNING: remember to delete object!!!
	return hRegion;
}

void WinImpl::redraw(gWindowHandle handle, int x, int y, int width, int height) {
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

	HRGN exit_shape = createShape(
		"C:/Users/Autriche/Documents/Code/gSplasher/gSplasher/Reference/icon/add_normal.png",
		c_offset_x, c_offset_y);

	c_offset_y += 10;
	HRGN minimize_shape = CreateEllipticRgn(
		c_offset_x,
		c_offset_y,
		c_offset_x + m,
		c_offset_y + m
		);

	CombineRgn(window_shape, exit_shape, window_shape, RGN_OR);


	SetWindowRgn(handle, window_shape, true);
	DeleteObject(window_shape);
	DeleteObject(exit_shape);
	DeleteObject(minimize_shape);
}

#endif
