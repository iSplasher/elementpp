#include "element/layout.h"

#include "catch.hpp"

USING_NAMESPACE

SCENARIO("Layouts", "[Layout]") {
	Application* app = Application::instance();
	if (!app)
		app = new Application();

	GIVEN("Two layouts are instatiated") {
		auto layout1 = app->create<Layout>();
		auto layout2 = app->create<Layout>();
	}


}
