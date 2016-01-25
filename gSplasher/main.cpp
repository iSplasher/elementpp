/// <summary>
/// Here is where stuff is tested!
/// </summary>
#include "include/Core.h"
#include "include/Widget.h"

USING_NAMESPACE

int main() {
	gApplication app{};

	gCoreWidget widget{};
	
	gCoreWidget widget2(&widget);
	gCoreWidget widget3(&widget);
	gCoreWidget widget4(&widget);
	gCoreWidget widget5(&widget2);
	gCoreWidget widget6(&widget2);
	app.print_tree(static_cast<gApplication::CoreList::const_iterator >(*app.core_objects));

	return app.run();
}

