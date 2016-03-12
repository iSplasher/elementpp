#pragma once

#include "Global.h"
#include "Utils/Primitives.h"
#include "Utils/CoreUtils.h"

#include <vector>
#include <utility>

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

protected:
	Type m_type;
	gCore* receiver = nullptr;

	friend class gEventManager;
	friend class gCore;
};

using EventPtr = std::shared_ptr<gEvent>;

// Input Events

struct GSPLASHER_API gInputEvent : gEvent {
	explicit gInputEvent(
		Type t,
		bool _alt=false,
		bool _control=false,
		bool _shift=false,
		bool _system=false) :
		gEvent(t),
		alt(_alt),
		control(_control),
		shift(_shift),
		system(_system){}

	gInputEvent(const gInputEvent&);

	bool alt;
	bool control;
	bool shift;
	bool system;
};

using InputEventPtr = std::shared_ptr<gInputEvent>;

struct GSPLASHER_API gMouseEvent : gInputEvent {
	//gMouseEvent(Type t, Point pos);
	//explicit gMouseEvent(sf::Event);
	//gMouseEvent(const gMouseEvent&);
	//// data members
	//// pos
	//int x, y, global_x = Mouse::getPosition().x, global_y = Mouse::getPosition().y;
	//Mouse::Button button;
	//Point pos() const { return Point(x, y); }
	//Point globalPos() const { return Point(global_x, global_y); }
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
	//gMoveEvent(Type t, int new_x, int new_y, int old_x, int old_y) :
	//	gEvent(t), new_point(new_x, new_y), old_point(old_x, old_y) {}
	//gMoveEvent(Type t, Point new_p, Point old_p) :
	//	gMoveEvent(t, new_p.x, new_p.y, old_p.x, old_p.y) {}

	//// data members
	//Point new_point;
	//Point old_point;
};

using MoveEventPtr = std::shared_ptr<gMoveEvent>;

// Widget Events

struct GSPLASHER_API gResizeEvent : gEvent {
	gResizeEvent(Type t, gSize nsize, gSize osize) : gEvent(t), new_size(nsize), old_size(osize) {}

	gSize new_size;
	gSize old_size;
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


