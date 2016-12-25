#pragma once

#include "Global.h"
#include "Event.h"
#include "Utils/Tree.h" // TODO: move this to cpp and forward declare tree class

#include <atomic>


NAMESPACE_BEGIN

class Window;
class Application;

/// <summary>
/// Core object of the whole library. 
/// </summary>
class GSPLASHER_API Core {
public:
	Core(Core *parent = nullptr);
	virtual ~Core();

	//logD(std::string);
	//logE(std::string);
	//logW(std::string);
	//logI(std::string);

	std::string objectName() const { return object_name; }
	void setObjectName(std::string name) { object_name = name; }

	/// <summary>
	/// Events are received here
	/// </summary>
	/// <param name="ev">A Event object.</param>
	virtual void event(EventPtr ev);

	/// <summary>
	/// Retrieve a pointer to parent Core
	/// </summary>
	/// <returns>Core*</returns>
	Core* parentCore() const { return core_parent; }

	/// <summary>
	/// Change parent of Core object
	/// </summary>
	/// <param name="new_parent">The new parent object</param>
	virtual void setParent(Core*);

	std::vector<Core*> children();

	// data members
	bool is_widget = false;
	bool is_window = false;

private:
	// methods
	//log(LogLevel, std::string);

	// data members
	Core* core_parent;
	unsigned core_id;
	std::string object_name = "Core";
	static std::atomic<unsigned> id_counter;
	tree<Core*>::iterator internal_tree;
	/// <summary>
	/// Used by parent to let us know if we should remove ourselves
	/// from the internal tree container
	/// </summary>
	bool parent_is_deleting = false;

	friend class Application;

};

// A pointer to the application instance
#define App Application::instance()

/// <summary>
/// Main instance of the whole application. Manages events and widgets. Only one instance is allowed.
/// </summary>
class GSPLASHER_API Application final : private Core{
public:
	using CoreList = tree<Core*>;
	using CoreListPtr = std::unique_ptr<CoreList>;
	Application();
	~Application();

	// member methods
	int exec();
	void event(EventPtr) override;
	void sendEvent(Core* reciever, EventPtr);
	void dispatchEvent(Core* reciever, EventPtr);
	static Application *instance();
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
	Application(const Application&) {}

	// member methods
	/// <summary>
	/// Processes the next events in the loop
	/// </summary>
	/// <returns>false if application should quit else true</returns>
	bool processEv() const;

	/// <summary>
	/// Inserts Core pointer to keep list in sync
	/// </summary>
	/// <param name="Core*">a pointer to the Core object</param>
	void insertCore(Core*) const;

	// data members
	static Application *self;
	EventManager event_manager;
	bool should_quit = false;
	bool is_running = false;

	friend class Window;
	friend class Core;
};


NAMESPACE_END

