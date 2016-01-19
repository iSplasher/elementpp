#pragma once

#include "Global.h"
#include "Utils/Primitives.h"

#include <SFML\Graphics.hpp>
#include <memory>
#include <vector>

NAMESPACE_BEGIN

class gCoreWidget;

/// <summary>
/// Core object of the whole library. 
/// </summary>
class gCore {
public:
	gCore(gCore *parent = nullptr) : core_parent(parent) {}
	virtual ~gCore() = default;

protected:
	gCore* core_parent;
};

/// <summary>
/// Main instance of the whole application. Manages events and widgets. Only one instance is allowed.
/// </summary>
class gApplication final : public gCore {
public:
	// *structers
	gApplication() : gCore(){}
	~gApplication() = default;

	// member methods
	int run() const;

	// data members
	static std::unique_ptr<gApplication> gApp;
private:
	// member methods
	bool process_ev() const;

	// data members
	std::unique_ptr<std::vector<gCoreWidget*>> widgets;
};


NAMESPACE_END

