#pragma once

#include "Global.h"
#include "Event.h"

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

	//logD(std::string);
	//logE(std::string);
	//logW(std::string);
	//logI(std::string);

	/// <summary>
	/// Events are received here
	/// </summary>
	/// <param name="sender">
	/// The one responsible for sending the event.
	/// Most of the time, this will be the parent.
	/// A gCore object.
	/// </param>
	/// <param name="ev">A gEvent object.</param>
	virtual void event(gCore&, gEvent);

protected:
	gCore* core_parent;

private:
	//log(LogLevel, std::string);

};

// A pointer to the application instance
#define gApp gApplication::instance()

/// <summary>
/// Main instance of the whole application. Manages events and widgets. Only one instance is allowed.
/// </summary>
class GSPLASHER_API gApplication final : public gCore {

	friend class gCoreWidget;

	using CoreWidgetList = std::vector<gCoreWidget*>;
	using CoreWidgetListPtr = std::unique_ptr<CoreWidgetList>;
public:
	gApplication();

	// member methods
	int run();
	static gApplication *instance();
	// data members
private:
	gApplication(const gApplication&) {}

	// member methods
	bool processEv() const;
	void addWidget(gCoreWidget* widget) const;

	// data members
	static gApplication *self;
	CoreWidgetListPtr widgets;
	gEventManager event_manager;
};


NAMESPACE_END

