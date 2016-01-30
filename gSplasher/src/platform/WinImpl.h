#pragma once

#include <SFML/Window/WindowHandle.hpp>
using gWindowHandle = sf::WindowHandle;

class WinImpl
{
public:
	WinImpl();
	~WinImpl();

	gWindowHandle createHandle(int x, int y, int width, int height);
	static void processEvents();
	void setTransparency(gWindowHandle handle, unsigned char alpha);
	void redraw(gWindowHandle handle, int x, int y, int width, int height);
};

