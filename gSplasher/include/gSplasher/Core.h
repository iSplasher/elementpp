#pragma once

#include "Global.h"
#include "Event.h"
#include "Property.h"
#include "Utils/Tree.h" // TODO: move this to cpp and forward declare tree class

#include <atomic>


NAMESPACE_BEGIN

class Component;
using ComponentPtr = std::unique_ptr<Component>;
class RWindow;
class Application;

/// <summary>
/// Component object of the whole library. 
/// </summary>
class GSPLASHER_API Component {
public:

	// META

	Component();
	explicit Component(ComponentPtr &parent);
	virtual ~Component();

	// move

	// not copyable
	Component(const Component&) = delete;
	Component& operator=(const Component&) = delete;

	// PROPERTIES

	Property<std::string> objectName;

	std::vector<ComponentPtr*> children();

	// FUNCTIONS

	ComponentPtr& getParent() const;
	void setParent(ComponentPtr&);

	// DATA

	bool is_widget = false;
	bool is_window = false;

private:
	using ComponentTree = tree<ComponentPtr*>;

	// FUNCTIONS

	//log(LogLevel, std::string);

	// DATA
	ComponentPtr *parent = nullptr;
	unsigned core_id;
	static std::atomic<unsigned> id_counter;
	static ComponentPtr nulltype;
	ComponentTree::iterator internal_tree;

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
class GSPLASHER_API Application final : private Component{
public:
	using ComponentContainer = std::vector<ComponentPtr>;
	using ComponentContainerPtr = std::unique_ptr<ComponentContainer>;
	using ComponentTreePtr = std::unique_ptr<ComponentTree>;

	/**
	 * \brief
	 */
	Application();
	~Application();

	// member methods
	template<class T, typename... Args>
	std::unique_ptr<T>& createItem(Args... args);
	int exec();
	static Application *instance();

	// data members
	Property<bool> isRunning; // TODO: Read-only property


	void print_tree() const {

		std::function<void(const ComponentTree::const_iterator &t)> pp = [&](const ComponentTree::const_iterator &t)
		{

			for (ComponentTree::const_iterator i = t.begin();
				i != t.end(); ++i)
			{
				for (int tabs = 1; tabs < i.level(); ++tabs)
					std::cout << "\t";

				std::string p = (*i.data())->objectName;

				std::cout << p << std::endl;

				pp(i);
			}};
		pp(*component_tree);
	}

private:
	Application(const Application&) {}

	// member methods
	/// <summary>
	/// Processes the next events in the loop
	/// </summary>
	/// <returns>false if application should quit else true</returns>
	bool processEv() const;

	// data members
	static Application *self;
	bool should_quit = false;
	bool is_running = false;

	ComponentContainerPtr component_objects;
	ComponentTreePtr component_tree;

	friend class RWindow;
	friend class Component;
};

template <class T, typename ... Args>
std::unique_ptr<T>& Application::createItem(Args... args)
{
	static_assert(std::is_base_of<Component, T>::value, "Must be same or inherited of Component");
	component_objects->push_back(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
	auto &item = component_objects->back();

	if (item->getParent())
	{
		item->internal_tree = item->getParent()->internal_tree.push_back(&item);
	} else
	{
		item->internal_tree = component_tree->push_back(&item);
	}

	return item;
}

NAMESPACE_END

