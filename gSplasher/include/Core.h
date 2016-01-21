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
	using CoreWidgetList = std::vector<gCoreWidget*>;
	using CoreWidgetListPtr = std::unique_ptr<CoreWidgetList>;
public:
	// *structers
	gApplication();
	~gApplication() = default;

	// member methods
	int run() const;

	// data members
private:
	// member methods
	bool process_ev() const;

	// data members
	CoreWidgetListPtr widgets;
};


NAMESPACE_END

