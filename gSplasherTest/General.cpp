#include <Catch/catch.hpp>
#include <gSplasher/Utils/Primitives.h>

USING_NAMESPACE

SCENARIO("Primitives Arithmetic") {
	GIVEN("primtives instantiations") {
		gPoint point;
		gSize size;
		gRect rect;

		WHEN("insantiated") {
			THEN("defaults values are zero with overload style") {
				REQUIRE(point == 0);
				REQUIRE(size == 0);
				REQUIRE(rect == 0);
			}

			THEN("defaults values are zero") {
				REQUIRE(point.x == 0);
				REQUIRE(point.y == 0);
				REQUIRE(size.width == 0);
				REQUIRE(size.height == 0);
				REQUIRE(rect.x == 0);
				REQUIRE(rect.y == 0);
				REQUIRE(rect.width == 0);
				REQUIRE(rect.height == 0);
			}
		}

	}
}