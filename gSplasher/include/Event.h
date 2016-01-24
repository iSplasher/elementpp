#pragma once

#include "Global.h"

#include <vector>
#include <SFML/Window/Event.hpp>

NAMESPACE_BEGIN

/// <summary>
/// Base event. Custom events should subclass this class.
/// </summary>
class GSPLASHER_API gEvent {
public:
	gEvent(){}
	gEvent(sf::Event ev) : sfevent(ev){}
	virtual ~gEvent() = default;

private:
	sf::Event sfevent;
};


/// <summary>
/// Manages events
/// </summary>
class GSPLASHER_API gEventManager {
	using EventQueue = std::vector<gEvent*>;


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
	/// <param name="event">A gEvent or its deratives</param>
	void dispatchEvent(gEvent&);
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


