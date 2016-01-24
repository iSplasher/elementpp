#include "../include/Event.h"
#include "../include/Core.h"

USING_NAMESPACE

int gEventManager::head;
int gEventManager::tail;

void gEventManager::init() {
	head = 0;
	tail = 0;
}

void gEventManager::dispatchEvent(gEvent &ev) {
	eventqueue.push_back(&ev);
}

void gEventManager::processEv() {
	while (!eventqueue.empty()) {
		gEvent *ev = eventqueue.back();
		if (gApp) {
			gApp->event(*gApp, *ev);
		}
		eventqueue.pop_back();
	}
}
