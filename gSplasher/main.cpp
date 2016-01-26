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
	gCoreWidget widget5{};
	gCoreWidget widget6(&widget5);
	gCoreWidget widget7(&widget5);
	gCoreWidget widget8(&widget7);
	gCoreWidget widget9(&widget7);
	gCoreWidget widget10(&widget9);
	gCoreWidget widget11(&widget4);

	app.print_tree(static_cast<gApplication::CoreList::const_iterator >(*app.core_objects));
	printf("%d", app.core_objects->tree_size());

	return app.run();
}

