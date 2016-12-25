#include "gSplasher/Core.h"
#include "gSplasher/Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

USING_NAMESPACE

std::atomic<unsigned> Core::id_counter;
Application* Application::self = nullptr;

Core::Core(Core* parent) :
	core_parent(parent),
	core_id(++id_counter) {
	if (App != nullptr) {
		App->insertCore(this);
	}

}

Core::~Core() {
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

void Core::event(EventPtr ev) {
#ifdef _DEBUG
	ev->printEvent();
#endif
	if ((ev->receiver == nullptr || ev->receiver != this) && !ev->ignored) {
		for (auto &c : children()) {
			c->event(ev);
		}
	}

	if (ev->ignored) {
		if (core_parent) {
			ev->ignored = false;
			core_parent->event(ev);
		}
	}

}

void Core::setParent(Core* new_parent) {
	if (App != nullptr) {
		if (new_parent) {
			internal_tree = new_parent->internal_tree.reinsert(internal_tree);
		}
		core_parent = new_parent;
	}
}

std::vector<Core*> Core::children() {
	std::vector<Core*> vec;
	for (auto i = internal_tree.begin(); i != internal_tree.end(); ++i) {
		vec.push_back(i.data());
	}
	return vec;
}

static void errorCallback(int err, const char *descr) {
	std::cout << descr;
}

static void closeWindowCallback(GLFWwindow *r_window) {
	// TODO: delete Window object
	glfwDestroyWindow(r_window);
}

Application::Application() :
	Core(), core_objects(std::make_unique<CoreList>()), event_manager() {
	internal_tree = core_objects->tree_iterator();

	assert(self == nullptr);
	self = this;

	glfwSetErrorCallback(errorCallback);
	if (!glfwInit()) {
		throw std::runtime_error("Could not init glfw");
	}

	glEnable(GL_MULTISAMPLE);
	event_manager.init();
	setObjectName("Application");
}

Application::~Application() {
	glfwTerminate();
	is_running = false;
}

int Application::exec() {
	is_running = true;
	sendEvent(nullptr, std::make_shared<Event>(Event::Type::Layout));
	glfwSwapInterval(0);
	while (processEv()) {
		event_manager.processEv();
	}
	return 0;
}

void Application::event(EventPtr ev) {
	// TODO: handle quit event here
	Core::event(ev);
}

void Application::sendEvent(Core* reciever, EventPtr ev) {
	if (!reciever || reciever == parentCore()) {
		event(ev);
	}
	else {
		reciever->event(ev);
	}
}

void Application::dispatchEvent(Core* reciever, EventPtr ev) {
	event_manager.dispatchEvent(reciever, ev);
}

Application* Application::instance() {
	return self;
}

bool Application::processEv() const {
	// TODO: optimize this so it doesn't check all
	for (auto &core : *core_objects) {

		if (core->is_window) {
			static_cast<Window*>(core)->update();
		}
	}
	glfwWaitEvents();

	return should_quit ? false : true;
}



void Application::insertCore(Core* core) const {
	// if there is a parent
	if (core->parentCore()) {
		// insert in the parent branch
		core->internal_tree = core->parentCore()->internal_tree.push_back(core);
	} else {
		// else just insert it at root
		core->internal_tree = core_objects->push_back(core);
	}

}
