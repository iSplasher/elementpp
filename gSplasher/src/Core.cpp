#include "gSplasher/Core.h"
//#include "gSplasher/Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

USING_NAMESPACE

std::atomic<unsigned> Component::id_counter;
ComponentPtr Component::nulltype = ComponentPtr();
Application* Application::self = nullptr;

Component::Component() : objectName("Component" + std::to_string(++id_counter)), core_id(id_counter)
{
}

Component::Component(ComponentPtr& parent) : Component()
{
	this->parent = &parent;
}

Component::~Component() {
	// first we traverse the tree to tell our children that
	// they shouldn't touch their internal tree.
	// we also delete the cildrens

	if (!parent_is_deleting) {
		for (auto iter = internal_tree.begin(); iter != internal_tree.end(); ++iter)
		{
			(*(*iter))->parent_is_deleting = true;

			for (auto inner = iter.begin(); inner != iter.end(); ++inner)
			{
				(*(*inner))->parent_is_deleting = true;
				(*(*inner)).reset();
			}
			(*(*iter)).reset();
		}

		// then we clear our branch

		internal_tree.clear_tree();
	}
}

std::vector<ComponentPtr*> Component::children() {
	std::vector<ComponentPtr*> vec;
	for (auto i = internal_tree.begin(); i != internal_tree.end(); ++i) {
		vec.push_back(*i);
	}
	return vec;
}

ComponentPtr& Component::getParent() const
{
	if (parent)
		return *parent;
	return nulltype;
}

void Component::setParent(ComponentPtr& new_parent)
{
	if (App != nullptr) {
		if (new_parent) {
			this->internal_tree = new_parent->internal_tree.reinsert(internal_tree);
		}
		else
		{
			this->internal_tree = App->internal_tree.reinsert(internal_tree);
		}
	}
	this->parent = &new_parent;
}

static void errorCallback(int err, const char *descr) {
	std::cout << descr;
}

static void closeWindowCallback(GLFWwindow *r_window) {
	// TODO: delete RWindow object
	glfwDestroyWindow(r_window);
}

Application::Application() :
	Component(),
	component_objects(std::make_unique<ComponentContainer>()),
	component_tree(std::make_unique<ComponentTree>()) {
	internal_tree = component_tree->tree_iterator();

	assert(self == nullptr);
	self = this;

	glfwSetErrorCallback(errorCallback);
	if (!glfwInit()) {
		throw std::runtime_error("Could not init glfw");
	}

	glEnable(GL_MULTISAMPLE);
	objectName = "Application";
}

Application::~Application() {
	glfwTerminate();
	isRunning = false;
}

int Application::exec() {
	isRunning = true;
	glfwSwapInterval(0);
	do {} while (processEv());
	return 0;
}


Application* Application::instance() {
	return self;
}

bool Application::processEv() const {
	// TODO: optimize this so it doesn't check all
	//for (auto &core : *component_tree) {

	//	if (core->is_window) {
	//		static_cast<RWindow*>(core)->update();
	//	}
	//}
	glfwWaitEvents();

	return should_quit ? false : true;
}

