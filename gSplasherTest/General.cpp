#include "gSplasher/Core.h"

#include "catch.hpp"

USING_NAMESPACE

SCENARIO("Application object can be created", "[Application]")
{
	Application *app = Application::instance();
	if (!app)
		app = new Application();

	GIVEN("An Application object is created")
	{
		REQUIRE(app);
	}
}

SCENARIO("Component objects can be created and destroyed", "[Comoponent]")
{
	Application *app = Application::instance();
	if (!app)
		app = new Application();

	GIVEN("Component objects are created")
	{
		auto &comp1 = app->create<Component>();
		auto &comp2 = app->create<Component>();

		REQUIRE(comp1);
		REQUIRE(comp2);

		WHEN("Children are created")
		{
			auto &child1 = app->create<Component>(&comp1);
			auto &child2 = app->create<Component>(&comp2);

			THEN("they get appended to their parents")
			{
				REQUIRE(comp1->children().size() == 1);
				REQUIRE(comp2->children().size() == 1);
			}

		}
	}
}


