#pragma once

#include "Global.h"
#include "Event.h"
#include "Utils/Tree.h"

#include <memory>
#include <atomic>

NAMESPACE_BEGIN

class gCoreWidget;

/// <summary>
/// Core object of the whole library. 
/// </summary>
class GSPLASHER_API gCore {
public:
	gCore(gCore *parent = nullptr);
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
	virtual void event(EventPtr);
	/// <summary>
	/// Retrieve a pointer to parent <gCore>
	/// </summary>
	/// <returns>gCore*</returns>
	gCore* parentCore() const { return core_parent; }

	// data members
	bool is_widget;

protected:
	// data members
	gCore* core_parent;

private:
	// methods
	//log(LogLevel, std::string);

	// data members
	unsigned core_id;
	static std::atomic<unsigned> id_counter;

};

// A pointer to the application instance
#define gApp gApplication::instance()

/// <summary>
/// Main instance of the whole application. Manages events and widgets. Only one instance is allowed.
/// </summary>
class GSPLASHER_API gApplication final : public gCore {

friend class gCoreWidget;
friend class gCore;

public:
	using CoreList = tree<gCore*>;
	using CoreListPtr = std::unique_ptr<CoreList>;
	gApplication();

	// member methods
	int run();
	void event(EventPtr);
	void sendEvent(gCore* reciever, EventPtr);
	static gApplication *instance();
	void print_tree(CoreList::const_iterator &t) {
		for (CoreList::const_iterator i = t.begin();
		i != t.end(); ++i)
		{
			for (int tabs = 1; tabs < i.level(); ++tabs)
				printf("\t");
			printf("object\n");

			print_tree(i);
		}
		//printf("%d", core_objects->tree_size());
	}

	// data members
	CoreListPtr core_objects;
private:
	gApplication(const gApplication&) {}

	// member methods
	bool processEv() const;
	/// <summary>
	/// Inserts gCore pointers to keep list in sync
	/// </summary>
	/// <param name="gCore*">a pointer to the gCore object</param>
	void insertCore(gCore*) const;
	/// <summary>
	/// Removes gCore pointers from the list
	/// </summary>
	/// <param name="gCore*">a pointer to the gCore object</param>
	void removeCore(gCore*) const;

	// data members
	static gApplication *self;
	gEventManager event_manager;
	bool should_quit;
};


NAMESPACE_END

