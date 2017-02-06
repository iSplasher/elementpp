#include "gSplasher/Core.h"

#include "catch.hpp"

USING_NAMESPACE

SCENARIO("Properties", "[Property]") {
	Application* app = Application::instance();
	if( !app )
		app = new Application();

	GIVEN("Simple read-write properties can be instantiated") {
		Property< std::string > prop1{};
		Property< std::string > prop2{ "hello world" };
		auto prop3 = Property< std::string >( "hello world 2" );

		WHEN("Property value is assigned") {

			std::string call1 = prop1;
			std::string call2 = prop2;
			std::string call3 = prop3;

			THEN("Same object is returned") {
				REQUIRE(call1 == "");
				REQUIRE(call2 == "hello world");
				REQUIRE(call3 == "hello world 2");
			}

		}

		WHEN("Property value is assigned to") {

			prop1 = "1";
			prop2 = "2";
			prop3 = "3";

			THEN("The assignment is successful") {
				REQUIRE(prop1 == "1");
				REQUIRE(prop2 == "2");
				REQUIRE(prop3 == "3");
			}

		}

		WHEN("Property value is compared to") {

			prop1 = "1";
			prop2 = "1";
			prop3 = "3";

			THEN("The comparison is successful") {
				REQUIRE(prop1 == prop2);
				REQUIRE(prop2 != prop3);
			}

		}

		WHEN("Property value is const-referenced") {

			const std::string& call1 = prop1;
			const std::string& call2 = prop2;
			const std::string& call3 = prop3;

			THEN("Same object is returned") {
				REQUIRE(call1 == "");
				REQUIRE(call2 == "hello world");
				REQUIRE(call3 == "hello world 2");
			}

		}

		WHEN("Property is connected to function") {
			std::string value1 = "";
			prop1.connect( [&value1](std::string s) { value1 = s; } );

			THEN("function is called on property change") {
				REQUIRE(value1 == "");
				prop1 = "new value";
				REQUIRE(value1 == "new value");
			}

		}

	}
}


SCENARIO("Component objects properties", "[Component][Property]") {}
