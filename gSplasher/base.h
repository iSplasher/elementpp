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

class BaseManager {
	friend class BaseWidget;
public:
	BaseManager();
	BaseManager(BaseWidget&);
	bool run() const;
private:
	bool process_events() const;

	std::shared_ptr<std::vector<BaseWidget*>> widgets;
	static std::shared_ptr<BaseManager> first_manager;
};

class BaseWidget {
	friend class BaseManager;
public:
	BaseWidget();
	BaseWidget(const BaseWidget&);

	sf::Vector2i grabOffset;
	bool is_grabbed = false;
private:
	BaseManager const *manager = nullptr;
	std::unique_ptr<sf::RenderWindow> sfwindow;

	sf::Color base_widget_color{ sf::Color(255, 0, 0, 192) };
	sf::Color base_widget_color_grabbed{ sf::Color(255, 255, 0, 64) };
};
}

#endif