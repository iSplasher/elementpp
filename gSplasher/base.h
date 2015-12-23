#ifndef BASE_H
#define BASE_H

#include <SFML\Graphics.hpp>
#include <Windows.h>
#include <memory>
#include <vector>

inline void makeWindowTransparent(sf::RenderWindow &window) {
	HWND hwnd = window.getSystemHandle();
	SetWindowLongPtr(hwnd, GWL_EXSTYLE, GetWindowLongPtr(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
}

inline void setWindowAlpha(sf::RenderWindow &window, sf::Uint8 alpha = 255) {
	SetLayeredWindowAttributes(window.getSystemHandle(), 0, alpha, LWA_ALPHA);
}

namespace gspl
{

class WindowManager {
	friend class BaseWindow;
public:
	WindowManager();
	WindowManager(BaseWindow&);
	bool run();
private:
	bool process_events();

	std::shared_ptr<std::vector<BaseWindow*>> windows;
	static std::shared_ptr<WindowManager> base_manager;
};

class BaseWindow {
	friend class WindowManager;
public:
	BaseWindow();
	BaseWindow(const BaseWindow&);

	sf::Vector2i grabOffset;
	bool is_grabbed = false;
private:
	WindowManager const *manager = nullptr;
	std::unique_ptr<sf::RenderWindow> sfwindow;

	sf::Color base_window_color{ sf::Color(255, 0, 0, 192) };
	sf::Color base_window_color_grabbed{ sf::Color(255, 255, 0, 64) };
};
}

#endif