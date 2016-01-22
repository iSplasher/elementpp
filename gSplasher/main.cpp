/// <summary>
/// Here is where stuff is tested!
/// </summary>
#include "include/Core.h"
#include "include/Widget.h"

USING_NAMESPACE

int main() {
	gApplication* app = gApplication::instance();

	gCoreWidget widget{};

	return app->run();
}

