#pragma once

#include "Global.h"
#include "Utils/Primitives.h"
#include "Utils/CoreUtils.h"

#include <vector>
#include <utility>
#ifdef _DEBUG
#include <string>
#endif

NAMESPACE_BEGIN

class gCore;
class gEventManager;

/// <summary>
/// Base event. Custom events should subclass this class.
/// </summary>
struct GSPLASHER_API gEvent {
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

	explicit gEvent(Type t) : m_type(t) {}
	gEvent() = default;
	gEvent(const gEvent&);
	virtual ~gEvent() = default;

	// methods
	Type type() const { return static_cast<Type>(m_type); };
#ifdef _DEBUG
	void printEvent() const {
		std::string s_type("");

		switch (m_type) {
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
	gCore* receiver = nullptr;

	friend class gEventManager;
	friend class gCore;
};

using EventPtr = std::shared_ptr<gEvent>;

// Input Events

struct GSPLASHER_API gInputEvent : gEvent {
	gInputEvent(Type t, KeyModifier m) :gEvent(t), modifiers(m) {}

	KeyModifier modifiers;
};

using InputEventPtr = std::shared_ptr<gInputEvent>;

struct GSPLASHER_API gMouseEvent : gInputEvent {
	gMouseEvent(Type t, const gPoint local_pos, MouseButton b, MouseButton bs, KeyModifier modifiers) :
		gInputEvent(t, modifiers), pos(local_pos), button(b), buttons(bs) {}

	const gPoint pos;

	/// <summary>
	/// State of button which caused the event
	/// </summary>
	MouseButton button;

	/// <summary>
	/// State of all mouse buttons
	/// </summary>
	MouseButton buttons; 
};

using MouseEventPtr = std::shared_ptr<gMouseEvent>;

struct GSPLASHER_API gKeyEvent : gInputEvent {
	//gKeyEvent(Type t, int k, std::string txt = std::string());
	//gKeyEvent(sf::Event);

	//// data members
	//int key;
	//std::string text;
};

using KeyEventPtr = std::shared_ptr<gKeyEvent>;

struct GSPLASHER_API gMoveEvent : gEvent {
	gMoveEvent(Type t, const int new_x, const int new_y, const int old_x, const int old_y) :
		gEvent(t), new_pos(new_x, new_y), old_pos(old_x, old_y) {}
	gMoveEvent(Type t, const gPoint new_p, const gPoint old_p) :
		gMoveEvent(t, new_p.x, new_p.y, old_p.x, old_p.y) {}

	// data members
	const gPoint new_pos;
	const gPoint old_pos;
};

using MoveEventPtr = std::shared_ptr<gMoveEvent>;

// Widget Events

struct GSPLASHER_API gResizeEvent : gEvent {
	gResizeEvent(Type t, const gSize nsize, const gSize osize) : gEvent(t), new_size(nsize), old_size(osize) {}

	const gSize new_size;
	const gSize old_size;
};

using ResizeEventPtr = std::shared_ptr<gResizeEvent>;

/// <summary>
/// Manages events
/// </summary>
class gEventManager {
	using EventPair = std::pair<gCore*, EventPtr>;
	using EventQueue = std::vector<EventPair>;


public:

	gEventManager() : eventqueue() {}
	~gEventManager() = default;

	// methods
	/// <summary>
	/// Resets head and tail for our ring buffer
	/// </summary>
	static void init();

	/// <summary>
	/// Dispatch event to the event loop
	/// </summary>
	/// <param name="receiver">a pointer to a gCore object</param>
	/// <param name="event">a shared pointer to an gEvent or its deratives</param>
	void dispatchEvent(gCore*, EventPtr);

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


