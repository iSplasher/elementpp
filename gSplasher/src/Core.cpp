#include "gSplasher/Core.h"
#include "gSplasher/Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

USING_NAMESPACE

std::atomic<unsigned> gCore::id_counter;
gApplication* gApplication::self = nullptr;

gCore::gCore(gCore* parent) :
	core_parent(parent),
	core_id(++id_counter) {
	if (gApp != nullptr) {
		gApp->insertCore(this);
	}

}

gCore::~gCore() {
	// first we traverse the tree to tell our children that
	// they shouldn't touch their internal tree.
	// we also delete the cildrens

	if (!parent_is_deleting) {
		for (auto iter = internal_tree.begin(); iter != internal_tree.end(); ++iter)
		{
			(*iter)->parent_is_deleting = true;

			for (auto inner = iter.begin(); inner != iter.end(); ++inner)
			{
				(*inner)->parent_is_deleting = true;
				delete *inner;
			}
			delete *iter;
		}

		// then we clear our branch

		internal_tree.clear_tree();
	}
}

void gCore::event(EventPtr) {}

void gCore::changeParent(gCore* new_parent) {
	if (gApp != nullptr) {
		internal_tree = new_parent->internal_tree.reinsert(internal_tree);
		core_parent = new_parent;
	}
}

std::vector<gCore*> gCore::children() {
	std::vector<gCore*> vec;
	for (auto i = internal_tree.begin(); i != internal_tree.end(); ++i) {
		vec.push_back(i.data());
	}
	return vec;
}

void error_cb(int err, const char *descr) {
	//LOG_E << descr;
}

void closeWindow_cb(GLFWwindow *r_window) {
	// TODO: delete gWindow object
	glfwDestroyWindow(r_window);
}

gApplication::gApplication() :
	gCore(), core_objects(std::make_unique<CoreList>()), event_manager() {

	assert(self == nullptr);
	self = this;

	glfwSetErrorCallback(error_cb);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glEnable(GL_MULTISAMPLE);
	event_manager.init();
}

gApplication::~gApplication() {
	glfwTerminate();
}

int gApplication::run() {
	glfwSwapInterval(0);
	while (processEv()) {
		event_manager.processEv();
	}
	return 0;
}

void gApplication::event(EventPtr ev) {
	// TODO: handle quit event here
}

void gApplication::sendEvent(gCore* reciever, EventPtr ev) {
	if (reciever == parentCore()) {
		event(ev);
	}
	else {
		reciever->internal_tree.data()->event(ev);
	}
}

void gApplication::dispatchEvent(gCore* reciever, EventPtr ev) {
	event_manager.dispatchEvent(reciever, ev);
}

gApplication* gApplication::instance() {
	return self;
}

bool gApplication::processEv() const {
	// TODO: optimize this so it doesn't check all
	for (auto &core : *core_objects) {

		if (core->is_window) {
			static_cast<gWindow*>(core)->update();
		}
	}
	glfwWaitEvents();

	return should_quit ? false : true;
}



void gApplication::insertCore(gCore* core) const {
	CoreList::iterator iter;

	// if there is a parent
	if (core->parentCore()) {
		// insert in the parent branch
		core->internal_tree = core->parentCore()->internal_tree.push_back(core);
	} else {
		// else just insert it at root
		core->internal_tree = core_objects->push_back(core);
	}
}
