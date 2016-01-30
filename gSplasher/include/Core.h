#pragma once

#include "Global.h"
#include "Event.h"
#include "Utils/Tree.h" // TODO: move this to cpp and forward declare tree class

#include <atomic>

NAMESPACE_BEGIN

class gWindow;
class gApplication;

/// <summary>
/// Core object of the whole library. 
/// </summary>
class GSPLASHER_API gCore {
public:
	gCore(gCore *parent = nullptr);
	virtual ~gCore();

	//logD(std::string);
	//logE(std::string);
	//logW(std::string);
	//logI(std::string);

	/// <summary>
	/// Events are received here
	/// </summary>
	/// <param name="ev">A gEvent object.</param>
	virtual void event(EventPtr);

	/// <summary>
	/// Retrieve a pointer to parent gCore
	/// </summary>
	/// <returns>gCore*</returns>
	gCore* parentCore() const { return core_parent; }

	/// <summary>
	/// Change parent of gCore object
	/// </summary>
	/// <param name="new_parent">The new parent object</param>
	void changeParent(gCore*);

	// data members
	bool is_widget = false;
	bool is_window = false;

private:
	// methods
	//log(LogLevel, std::string);

	// data members
	gCore* core_parent;
	unsigned core_id;
	static std::atomic<unsigned> id_counter;
	tree<gCore*>::iterator internal_tree;
	/// <summary>
	/// Used by parent to let us know if we should remove ourselves
	/// from the internal tree container
	/// </summary>
	bool parent_is_deleting = false;

	friend class gApplication;

};

// A pointer to the application instance
#define gApp gApplication::instance()

/// <summary>
/// Main instance of the whole application. Manages events and widgets. Only one instance is allowed.
/// </summary>
class GSPLASHER_API gApplication final : private gCore{
public:
	using CoreList = tree<gCore*>;
	using CoreListPtr = std::unique_ptr<CoreList>;
	gApplication();
	~gApplication();

	// member methods
	int run();
	void event(EventPtr);
	void sendEvent(gCore* reciever, EventPtr);
	void dispatchEvent(gCore* reciever, EventPtr);
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
	}

	// data members
	CoreListPtr core_objects;
private:
	gApplication(const gApplication&) {}

	// member methods
	/// <summary>
	/// Processes the next events in the loop
	/// </summary>
	/// <returns>false if application should quit else true</returns>
	bool processEv() const;

	/// <summary>
	/// Inserts gCore pointer to keep list in sync
	/// </summary>
	/// <param name="gCore*">a pointer to the gCore object</param>
	void insertCore(gCore*) const;

	// data members
	static gApplication *self;
	gEventManager event_manager;
	bool should_quit = false;

	friend class gWindow;
	friend class gCore;
};


NAMESPACE_END

