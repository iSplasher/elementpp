#pragma once

#include "global.h"
#include "event.h"
#include "property.h"
#include "core/tree.h" // TODO: move this to cpp and forward declare tree class

#include <atomic>


NAMESPACE_BEGIN
class Element;
using ElementPtr = std::unique_ptr< Element >;
class RWindow;
class Application;


enum class ComponentType {
	Base, // TODO: new name
	Widget,
	Window,
	Layout
};


/**
 * \brief Base object.
 * A Element is created with Application.create<Element>(args...);
 */
class ELEMENT_API Element {
public:

	Element();

	explicit Element( ElementPtr& parent );

	virtual ~Element();

	// MOVE & COPY

	Element( const Element& other ) = delete;

	Element& operator=( const Element& other ) = delete;


	// PROPERTIES

public:
	Property< std::string > objectName;
	Property< ComponentType, Element > type{ ComponentType::Base };

	const Accessor< ElementPtr&, Element > parent;

	// FUNCTIONS

	std::vector< ElementPtr* > children();

	// DATA

protected:

	void setType( ComponentType t ) { type = t; }

private:
	using ComponentTree = tree< ElementPtr* >;


	Element(Element&& other) noexcept
		: objectName(std::move(other.objectName)),
		type(std::move(other.type)),
		parent(std::move(other.parent)),
		core_id(other.core_id),
		internal_tree(std::move(other.internal_tree)),
		_parent(other._parent),
		init(other.init),
		parent_is_deleting(other.parent_is_deleting) {}

	Element& operator=(Element&& other) noexcept {
		if (this == &other)
			return *this;
		objectName = std::move(other.objectName);
		type = std::move(other.type);
		parent = std::move(other.parent);
		core_id = other.core_id;
		internal_tree = std::move(other.internal_tree);
		_parent = other._parent;
		init = other.init;
		parent_is_deleting = other.parent_is_deleting;
		return *this;
	}

	// FUNCTIONS
	void setParent( ElementPtr& );

	ElementPtr& getParent() const;


	//log(LogLevel, std::string);

	// DATA

	unsigned core_id;
	static std::atomic< unsigned > id_counter;
	static ElementPtr nullparent;
	ComponentTree::iterator internal_tree;
	ElementPtr* _parent = &nullparent;

	bool init = false; // has this Element initialized?

	/**
	 * \brief Used by _parent to let us know if we should remove ourselves from the internal tree container
	 */
	bool parent_is_deleting = false;

	friend class Application;

};


// A pointer to the application instance
#define App Application::instance()


/**
 * \brief Main instance of the whole application. Only one instance is allowed.
 */
class ELEMENT_API Application final : private Element {
	using ComponentContainer = std::list< ElementPtr >;
	using ComponentContainerPtr = std::unique_ptr< ComponentContainer >;
	using ComponentTreePtr = std::unique_ptr< ComponentTree >;

public:

	Application();

	~Application();

	// FUNCTIONS

	/// <summary>
	/// Create Element objects
	/// </summary>
	template< class T, typename... Args >
	std::unique_ptr< T >& create( Args&& ... args );

	/// <summary>
	/// Delete Element objects
	/// </summary>
	template< class T >
	void destroy( T& );

	/// <summary>
	/// Start application, blocking
	/// </summary>
	/// <returns></returns>
	int exec();

	/// <summary>
	/// Get Application singleton instance
	/// </summary>
	/// <returns></returns>
	static Application* instance();

	// PROPERTIES

	Property< bool, Application > isRunning; // read only


	void print_tree() const {

		std::cout << objectName << std::endl;

		std::function< void( const ComponentTree::const_iterator& t ) > pp = [&](const ComponentTree::const_iterator& t) {

					for( ComponentTree::const_iterator i = t.begin();
					     i != t.end(); ++i ) {
						for( int tabs = 0; tabs < i.level(); ++tabs )
							std::cout << "\t";

						std::cout << ( *i.data() )->objectName << std::endl;

						pp( i );
					}
				};
		pp( *component_tree );
	}

private:
	Application( const Application& ) {}

	// member methods
	/// <summary>
	/// Processes the next events in the loop
	/// </summary>
	/// <returns>false if application should quit else true</returns>
	bool processEv() const;

	// data members
	static Application* self;
	bool should_quit = false;
	bool is_running = false;

	ComponentContainerPtr component_objects;
	ComponentTreePtr component_tree;

	friend class RWindow;
	friend class Element;
};


template< class T, typename ... Args >
std::unique_ptr< T >& Application::create( Args&& ... args ) {
	static_assert(std::is_base_of< Element, T >::value, "Must be same or inherited of Element");
	component_objects->push_back( std::make_unique< T >( std::forward< Args >( args )... ) );
	auto& item = component_objects->back();

	auto& item_parent = item->parent.get();

	if (item_parent && item_parent != Element::nullparent) { item->internal_tree = item_parent->internal_tree.push_back(&item); }
	else { item->internal_tree = component_tree->push_back(&item); }

	item->init = true;
	return item;
}

template< class T >
void Application::destroy( T& object ) {
	static_assert(std::is_convertible< T, ElementPtr >::value, "Must be same or inherited of Element");
	if( object )
		component_objects->remove( object );
}

NAMESPACE_END
