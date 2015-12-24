#pragma once

#include <SFML/Graphics.hpp>
//#if defined (SFML_SYSTEM_WINDOWS)
#include <Windows.h>

class WidgetUtils {
public:
	WidgetUtils() = default;
	~WidgetUtils() = default;

	static bool setShape(HWND hWnd, const sf::Image& image);

	static bool setTransparency(HWND hWnd, unsigned char alpha);

//#elif defined (SFML_SYSTEM_LINUX)
//#include <X11/Xatom.h>
//#include <X11/extensions/shape.h>
//
//	static bool setShape(Window wnd, const sf::Image& image);
//
//	static bool setTransparency(Window wnd, unsigned char alpha);
//
//#undef None // None conflicts with SFML
//// MAC OSX SUPPORT
////#elif defined (SFML_SYSTEM_MACOS)
////	static bool setShape(sf::WindowHandle handle, const sf::Image& image);
////	static bool setTransparency(sf::WindowHandle handle, unsigned char alpha);
//#else
//	static bool setShape(sf::WindowHandle handle, const sf::Image& image);
//
//	static bool setTransparency(sf::WindowHandle handle, unsigned char alpha);
//#endif
//

};

