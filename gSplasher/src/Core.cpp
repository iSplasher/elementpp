#include "../include/Core.h"
#include "../include/Widget.h"

USING_NAMESPACE

std::atomic<unsigned> gCore::id_counter;
gApplication* gApplication::self = nullptr;

gCore::gCore(gCore* parent) :
	is_widget(false),
	core_parent(parent),
	core_id(++id_counter) {
	if (gApp != nullptr) {
		gApp->insertCore(this);
	}

}

void gCore::event(EventPtr) {}

gApplication::gApplication() :
	gCore(), core_objects(std::make_unique<CoreList>()),
	event_manager(),
	should_quit(false) {
	assert(self == nullptr);
	self = this;
	event_manager.init();
}

int gApplication::run() {
	while (processEv()) {
		event_manager.processEv();
	}
	return 0;
}

void gApplication::event(EventPtr ev) {
	// TODO: handle quit event here
}

void gApplication::sendEvent(gCore* reciever, EventPtr) {
	// TODO: send events here
}

gApplication* gApplication::instance() {
	return self;
}

bool gApplication::processEv() const {
	for (auto core : *core_objects) {

		if (core->is_widget) {
			static_cast<gCoreWidget*>(core)->update();
		}
	}
	return should_quit ? false : true;
}

void gApplication::insertCore(gCore* core) const {
	CoreList::iterator iter;

	// if there is a parent
	if (core->parentCore()) {
		// using breadth first search
		auto main_t = core_objects->tree_find_breadth(core->parentCore());
		assert(main_t != core_objects->end());
		
		// insert in the parent branch
		 main_t.push_back(core);
	} else {
		// else just insert it at root
		iter = core_objects->push_back(core);
	}
}

void gApplication::removeCore(gCore *core) const {
	
	auto main_t = core_objects->find(core);
	assert(main_t != core_objects->end());

	main_t.clear_tree();
}
