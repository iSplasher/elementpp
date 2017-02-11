#pragma once

#include "global.h"
#include "core/primitives.h"

#include <vector>
#include <utility>
#include <functional>
#include <memory>
#include <list>
#ifdef _DEBUG
#include <string>
#endif

NAMESPACE_BEGIN
class Component;
class EventManager;


/// <summary>
/// Base event. Custom events should subclass this class.
/// </summary>
struct ELEMENT_API Event {
	enum class Type {
		None,
		MouseMove,
		MouseButtonPress,
		MouseButtonRelease,
		KeyPress,
		KeyRelease,

		// suitable for widgets
		Enter,
		Leave,
		Layout,
		Paint,
		Move,
		Resize,
		Show,
		Hide,
		Close,
		Quit,
	};


	explicit Event( Type t ) : m_type( t ) {}

	Event() = default;

	Event( const Event& );

	virtual ~Event() = default;

	// methods
	Type type() const { return static_cast< Type >( m_type ); };
	void ignore() { ignored = true; }

#ifdef _DEBUG
	void printEvent() const {
		std::string s_type( "" );

		switch( m_type ) {
			case Type::None:
				s_type = "None";
				break;
			case Type::MouseMove:
				s_type = "MouseMove";
				break;
			case Type::MouseButtonPress:
				s_type = "MouseButtonPress";
				break;
			case Type::MouseButtonRelease:
				s_type = "MouseButtonRelease";
				break;
			case Type::KeyPress:
				s_type = "KeyPress";
				break;
			case Type::KeyRelease:
				s_type = "KeyRelease";
				break;
			case Type::Enter:
				s_type = "Enter";
				break;
			case Type::Leave:
				s_type = "Leave";
				break;
			case Type::Layout:
				s_type = "Layout";
				break;
			case Type::Paint:
				s_type = "Paint";
				break;
			case Type::Move:
				s_type = "Move";
				break;
			case Type::Resize:
				s_type = "Resize";
				break;
			case Type::Show:
				s_type = "Show";
				break;
			case Type::Hide:
				s_type = "Hide";
				break;
			case Type::Close:
				s_type = "Close";
				break;
			case Type::Quit:
				s_type = "Quit";
				break;
			default:
				s_type = "Unnamed";
				break;
		}

		std::cout << s_type << " event was generated!" << std::endl;
	}
#endif

protected:
	Type m_type;
	Component* receiver = nullptr;

	friend class EventManager;
	friend class Component;

private:
	bool ignored = false;
};


using EventPtr = std::shared_ptr< Event >;


// Input Events

struct ELEMENT_API InputEvent : Event {
	InputEvent( Type t, KeyModifier m ) : Event( t ),
	                                      modifiers( m ) {}

	KeyModifier modifiers;
};


using InputEventPtr = std::shared_ptr< InputEvent >;


struct ELEMENT_API MouseEvent : InputEvent {
	MouseEvent( Type t, const Point local_pos, MouseButton b, MouseButton bs, KeyModifier modifiers ) :
		InputEvent( t, modifiers ),
		pos( local_pos ),
		button( b ),
		buttons( bs ) {}

	const Point pos;

	/// <summary>
	/// State of button which caused the event
	/// </summary>
	MouseButton button;

	/// <summary>
	/// State of all mouse buttons
	/// </summary>
	MouseButton buttons;
};


using MouseEventPtr = std::shared_ptr< MouseEvent >;


struct ELEMENT_API KeyEvent : InputEvent {
	//KeyEvent(Type t, int k, std::string txt = std::string());
	//KeyEvent(sf::Event);

	//// data members
	//int key;
	//std::string text;
};


using KeyEventPtr = std::shared_ptr< KeyEvent >;


// Widget Events

struct ELEMENT_API MoveEvent : Event {
	MoveEvent( Type t, const int new_x, const int new_y, const int old_x, const int old_y ) :
		Event( t ),
		pos( new_x, new_y ),
		old_pos( old_x, old_y ) {}

	MoveEvent( Type t, const Point new_p, const Point old_p ) :
		MoveEvent( t, new_p.x, new_p.y, old_p.x, old_p.y ) {}

	// data members
	const Point pos;
	const Point old_pos;
};


using MoveEventPtr = std::shared_ptr< MoveEvent >;


struct ELEMENT_API ResizeEvent : Event {
	ResizeEvent( Type t, const Size nsize, const Size osize ) : Event( t ),
	                                                            size( nsize ),
	                                                            old_size( osize ) {}

	const Size size;
	const Size old_size;
};


using ResizeEventPtr = std::shared_ptr< ResizeEvent >;


/// <summary>
/// Manages events
/// </summary>
class EventManager {
	using EventPair = std::pair< Component*, EventPtr >;
	using EventQueue = std::vector< EventPair >;


public:

	EventManager() : eventqueue() {}

	~EventManager() = default;

	// methods
	/// <summary>
	/// Resets head and tail for our ring buffer
	/// </summary>
	static void init();

	/// <summary>
	/// Dispatch event to the event loop
	/// </summary>
	/// <param name="receiver">a pointer to a Component object</param>
	/// <param name="event">a shared pointer to an Event or its deratives</param>
	void dispatchEvent( Component*, EventPtr );

	/// <summary>
	/// Processes events in the event queue
	/// </summary>
	void processEv();

private:
	// data members
	EventQueue eventqueue;
	static int head;
	static int tail;

};


NAMESPACE_END