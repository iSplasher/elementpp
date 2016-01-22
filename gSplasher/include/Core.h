#pragma once

#include "Global.h"

#include <memory>
#include <vector>

NAMESPACE_BEGIN

class gCoreWidget;

/// <summary>
/// Core object of the whole library. 
/// </summary>
class GSPLASHER_API gCore {
public:
	gCore(gCore *parent = nullptr) : core_parent(parent) {}
	virtual ~gCore() = default;

protected:
	gCore* core_parent;
};

/// <summary>
/// Main instance of the whole application. Manages events and widgets. Only one instance is allowed.
/// </summary>
class GSPLASHER_API gApplication final : public gCore {

	friend class gCoreWidget;

	using CoreWidgetList = std::vector<gCoreWidget*>;
	using CoreWidgetListPtr = std::unique_ptr<CoreWidgetList>;
public:

	// member methods
	int run() const;
	static gApplication *instance();
	// data members
private:
	// *structers
	gApplication() : gCore(), widgets(std::make_unique<CoreWidgetList>()) {}
	gApplication(const gApplication&) {}

	// member methods
	bool processEv() const;
	void addWidget(gCoreWidget* widget) const;
	gApplication &operator=(const gApplication&) {}

	// data members
	static gApplication *singleton;
	CoreWidgetListPtr widgets;
};


NAMESPACE_END

