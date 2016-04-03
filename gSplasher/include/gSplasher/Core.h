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

	std::string objectName() const { return object_name; }
	void setObjectName(std::string name) { object_name = name; }

	/// <summary>
	/// Events are received here
	/// </summary>
	/// <param name="ev">A gEvent object.</param>
	virtual void event(EventPtr ev);

	/// <summary>
	/// Retrieve a pointer to parent gCore
	/// </summary>
	/// <returns>gCore*</returns>
	gCore* parentCore() const { return core_parent; }

	/// <summary>
	/// Change parent of gCore object
	/// </summary>
	/// <param name="new_parent">The new parent object</param>
	virtual void setParent(gCore*);

	std::vector<gCore*> children();

	// data members
	bool is_widget = false;
	bool is_window = false;

private:
	// methods
	//log(LogLevel, std::string);

	// data members
	gCore* core_parent;
	unsigned core_id;
	std::string object_name = "gCore";
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
	int exec();
	void event(EventPtr) override;
	void sendEvent(gCore* reciever, EventPtr);
	void dispatchEvent(gCore* reciever, EventPtr);
	static gApplication *instance();
	bool isRunning() const { return is_running; }

	void print_tree(CoreList::const_iterator &t) const {
		for (CoreList::const_iterator i = t.begin();
		i != t.end(); ++i)
		{
			for (int tabs = 1; tabs < i.level(); ++tabs)
				std::cout << "\t";
			std::cout << i.data()->object_name << std::endl;

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
	bool is_running = false;

	friend class gWindow;
	friend class gCore;
};


NAMESPACE_END

