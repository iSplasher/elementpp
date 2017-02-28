#include "element/core.h"
#include "element/window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

USING_NAMESPACE

std::atomic< unsigned > Element::id_counter;
Application* Application::self = nullptr;

Element::Element() :
	objectName( "Element" + std::to_string( ++id_counter ) ),
	parent(this,  std::mem_fn(&Element::getParent), std::mem_fn(&Element::setParent)), core_id( id_counter ) {
}

Element::Element( Element *parent ) : Element() { this->parent = parent; }

Element::~Element() {
	// first we traverse the tree to tell our children that
	// they shouldn't touch their internal tree.
	// we also delete the children

	// NOTE: Set child parent to nullparent if not deleting!!!

	if( !parent_is_deleting ) {
		for( auto iter = internal_tree.begin(); iter != internal_tree.end(); ++iter ) {
			( *iter )->parent_is_deleting = true;

			for( auto inner = iter.begin(); inner != iter.end(); ++inner ) {
				( *inner )->parent_is_deleting = true;
				(*( *inner )->object).reset();
			}
			( *( *iter )->object ).reset();
		}

		// then we clear our branch

		internal_tree.clear_tree();
	}
}

std::vector< Element* > Element::children() {
	std::vector< Element* > vec;
	for( auto i = internal_tree.begin(); i != internal_tree.end(); ++i ) { vec.push_back( *i ); }
	return vec;
}

Element* Element::getParent() const {
	return _parent;
}

void Element::setParent(Element* new_parent ) {
	if( App != nullptr && init ) {
		if( new_parent ) { this->internal_tree = new_parent->internal_tree.reinsert( internal_tree ); }
		else { this->internal_tree = App->internal_tree.reinsert( internal_tree ); }
	}
	_parent = new_parent;
}

static void errorCallback( int err, const char* descr ) { std::cout << descr; }

static void closeWindowCallback( GLFWwindow* r_window ) {
	// TODO: delete Window object
	glfwDestroyWindow( r_window );
}

Application::Application() :
	Element(),
	elementCount( this, std::mem_fn( &Application::getElementCount ) ),
	doubleClickInterval( this, std::mem_fn( &Application::getDoubleClickInterval ), std::mem_fn(&Application::setDoubleClickInterval)),
	clickInterval( this, std::mem_fn( &Application::getClickInterval ), std::mem_fn(&Application::setClickInterval)),
	component_objects( std::make_unique< ElementContainer >() ),
	component_tree( std::make_unique< ElementTree >() ){
	internal_tree = component_tree->tree_iterator();

	assert(self == nullptr);
	self = this;

	glfwSetErrorCallback( errorCallback );
	if( !glfwInit() ) { throw std::runtime_error( "Failed to initialize glfw" ); }

	glEnable( GL_MULTISAMPLE );
	objectName = "Application";
	setType(ElementType::Application);
}

Application::~Application() {
	glfwTerminate();
	isRunning = false;
}

ExitCode Application::exec() {
	isRunning = true;
	glfwSwapInterval( 0 );
	do {}
	while( processEv() );
	return ExitCode::Quit;
}


Application* Application::instance() { return self; }

bool Application::processEv() const {
	// TODO: optimize this so it doesn't check all
	for (auto &core : *component_tree) {

		if (core->type == ElementType::Window || core->type == ElementType::Layout) {
			static_cast<Layout*>(core)->update();
		}
	}
	glfwWaitEvents();

	return should_quit ? false : true;
}

std::size_t Application::getElementCount() const {
	return component_objects->size();
}

float Application::getDoubleClickInterval() const { return double_click_interval; }
void Application::setDoubleClickInterval(float i) { double_click_interval = i; }
float Application::getClickInterval() const { return click_interval; }
void Application::setClickInterval(float i) { click_interval = i; }


