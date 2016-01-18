// Contains base types
#pragma once

#include "Global.h"
#include "Wrapper.h"
#include "Utils/Primitives.h"

#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>

NAMESPACE_BEGIN

class gCore {
public:
	gCore(gCore *parent=nullptr);
	virtual ~gCore() = default;
	gCore* m_parent;
};

class BaseWidget;

class BaseFrame {
	friend class BaseWidget;
public:
	BaseFrame();
	BaseFrame(BaseWidget&);
	bool run() const;
	virtual ~BaseFrame() = default;
private:
	bool process_events() const;

	std::shared_ptr<std::vector<BaseWidget*>> widgets;
	static std::shared_ptr<BaseFrame> first_manager;
};

struct BaseType {
	BaseType() = default;
	~BaseType() = default;
};


struct BaseProperties {
	using Font = sf::Font;
	BaseProperties();

	BaseProperties(Color c);
	Color base_color;
	Font base_font;
	Size size;
	Point pos;
};

NAMESPACE_END

