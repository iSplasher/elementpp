#include "element/core.h"
#include "element/property.h"

#include "catch.hpp"

#include <chrono>
#include <thread>

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

		WHEN("Property connection is made") {
			std::string value1 = "";
			prop1.connect( [&value1](std::string s) { value1 = s; } );

			THEN("connection is called on property change") {
				REQUIRE(value1 == "");
				prop1 = "new value";
				REQUIRE(value1 == "new value");
			}
		}

		WHEN("Two properties dependency sync") {
			prop1.connect< ConnectionType::Temporary >( [&prop1, &prop2](std::string s) {
				                                           if( prop2 != prop1 ) {
					                                           prop2 = "changed";
				                                           };
			                                           } );

			prop2.connect< ConnectionType::Temporary >( [&prop1, &prop2](std::string s) {
				                                           if( prop2 != prop1 ) {
					                                           prop1 = "changed";
				                                           };
			                                           } );

			THEN("function is called sync on property change") {
				REQUIRE(prop1 == "");
				REQUIRE(prop2 == "hello world");
				prop1 = "test";
				REQUIRE(prop1 == "changed");
				REQUIRE(prop2 == "changed");

			}

		}

		WHEN("Two properties dependency async") {
			prop1.connect< ConnectionType::Temporary >( [&prop1, &prop2](std::string s) {
				                                           if( prop2 != prop1 ) {
					                                           prop2 = "changed";
				                                           };
			                                           } );

			prop2.connect< ConnectionType::Temporary >( [&prop1, &prop2](std::string s) {
				                                           if( prop2 != prop1 ) {
					                                           prop1 = "changed";
				                                           };
			                                           } );

			THEN("function is called async on property change") {
				REQUIRE(prop1 == "");
				REQUIRE(prop2 == "hello world");
				prop1 << "test";
				prop1.wait();
				REQUIRE(prop1 == "changed");
				REQUIRE(prop2 == "changed");

			}

		}

		WHEN("Two properties dependency async with sleep") {
			prop1.connect< ConnectionType::Temporary >( [&prop1, &prop2](std::string s) {
				                                           if( prop2 != prop1 ) {
					                                           std::cout << "testing async properties.. sleeping for 1 second.." << std::endl;
					                                           std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
					                                           prop2 = "changed";
				                                           };
			                                           } );

			prop2.connect< ConnectionType::Temporary >( [&prop1, &prop2](std::string s) {
				                                           if( prop2 != prop1 ) {
					                                           std::cout << "testing async properties.. sleeping for 1 second.." << std::endl;
					                                           std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
					                                           prop1 = "changed";
				                                           };
			                                           } );

			THEN("function is called async on property change") {
				REQUIRE(prop1 == "");
				REQUIRE(prop2 == "hello world");
				prop1 << "test";
				REQUIRE(prop2 == "hello world");
				prop1.wait();
				REQUIRE(prop1 == "changed");
				REQUIRE(prop2 == "changed");

			}

		}

		WHEN("Property is connected forever to function") {
			std::string value1 = "";
			prop1.connect( [&value1](std::string s) { value1 = s; } );

			THEN("function is called on every property change") {
				REQUIRE(value1 == "");
				prop1 = "value1";
				REQUIRE(value1 == "value1");
				prop1 = "value2";
				REQUIRE(value1 == "value2");
			}

		}

		WHEN("Property is connected once to function") {
			std::string value1 = "";
			prop1.connect< ConnectionType::Temporary >( [&value1](std::string s) {
				                                           value1 = s;
			                                           } );

			THEN("function is called on only one property change") {
				REQUIRE(value1 == "");
				prop1 = "value1";
				REQUIRE(value1 == "value1");
				prop1 = "value2";
				REQUIRE(value1 == "value1");
			}

		}

		WHEN("Property constructs a scoped connection") {
			std::string value1 = "";

			THEN("function is only called when connection is in scope") { {

					auto conn = prop1.connect< ConnectionType::Scoped >( [&value1](std::string s) {
						                                                    value1 = s;
					                                                    } );
					REQUIRE(value1 == "");
					prop1 = "value1";
					REQUIRE(value1 == "value1");
					prop1 = "value2";
					REQUIRE(value1 == "value2");
				}

				REQUIRE(value1 == "value2");
				prop1 = "value3";
				REQUIRE(value1 == "value2");
			}

		}

	}

	GIVEN("Simple read-only properties can be instantiated") {

		WHEN("When declared in class") {

			struct Test {
				Property< std::string, Test > prop1{};
				Property< std::string, Test > prop2{ "hello world" };

				void test( std::string s ) { prop1 = s; }
			};

			THEN( "Can read value" ) {
				auto t = Test();
				REQUIRE(t.prop1 == "");
				std::string v = t.prop2;
				REQUIRE(v == "hello world");
			}

			THEN("Class can change value") {
				auto t = Test();
				t.test( "prop1" );
				REQUIRE(t.prop1 == "prop1");
				std::string v = t.prop2;
				REQUIRE(v == "hello world");
			}

		}

	}

	GIVEN("Property views") {
		struct T {
			Property< std::string, T > third{ "!" };

		};
		Property< std::string > first{ "hello " };
		Property< std::string > second{ "world" };
		T t;

		WHEN("A simple std::string based property view is instantiated") {

			PropertyView< std::string > view1( [](std::string a, std::string b, std::string c) -> std::string { return a + b + c; },
			                                                first, second, t.third );

			THEN("PropertyView has the same value as returned by function") {
				REQUIRE(view1 == "hello world!");

				first = "new ";
				REQUIRE(view1 == "new world!");

				second = "element++";
				REQUIRE(view1 == "new element++!");

				first = "hello ";
				REQUIRE(view1 == "hello element++!");

			}

			THEN("PropertyView has the same value as returned by function async") {
				// async

				first << "new ";
				first.wait();
				REQUIRE(view1 == "new world!");

				second << "element++";
				second.wait();
				REQUIRE(view1 == "new element++!");

				first << "hello ";
				first.wait();
				REQUIRE(view1 == "hello element++!");
			}

			THEN("PropertyView connection can also refer to other properties") {

				view1.connect([&first](std::string s) {
						if(first != "changed ")
							first = "changed ";
					});

				REQUIRE(view1 == "hello world!");
				first << "async ";
				first.wait();
				REQUIRE(view1 == "changed world!");
				first = "new ";
				REQUIRE(view1 == "changed world!"); // reactive applies the value to itself before going through all connections
				
			}

		}

		WHEN("A PropertyView that differs in type is instantiated") {

			first = "0";
			second = "0";

			PropertyView< int > view2(
				[](std::string a, std::string b) -> int { return std::stoi(a) + std::stoi(b); },
				first, second);

			THEN("PropertyView has the same value as returned by function") {
				
				REQUIRE(view2 == 0);

				first = "1";
				REQUIRE(view2 == 1);

				second = "1";
				REQUIRE(view2 == 2);

				first = "2";
				REQUIRE(view2 == 3);

			}
			
		}

	}

}
