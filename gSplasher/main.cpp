 //<summary>
 //Here is where stuff is tested!
 //</summary>
#include "gSplasher/Core.h"

USING_NAMESPACE

int main() {

	Application app;

	auto &item2 = app.create<Component>();
	auto &item1 = app.create<Component>();
	auto &item3 = app.create<Component>(&item1);
	auto &item4 = app.create<Component>(&item2);

	app.print_tree();

	auto s = item4 ? "exist" : "destroyed";

	std::cout << s << std::endl;

	return app.exec();

}

//#include <yoga/Yoga.h>
//#include <iostream>
//
//void print(YGNodeRef n, char name[])
//{
//	std::cout << name << "{ " <<
//		YGNodeLayoutGetLeft(n) << ", " <<
//		YGNodeLayoutGetTop(n) << ", " <<
//		YGNodeLayoutGetWidth(n) << ", " <<
//		YGNodeLayoutGetHeight(n) <<
//		" }" << std::endl;
//
//}
//
//int main() {
//
//	YGNodeRef root = YGNodeNew();
//	YGNodeStyleSetFlexDirection(root, YGFlexDirectionRow);
//	//YGNodeStyleSetJustifyContent(root, YGJustifyFlexStart);
//	//YGNodeStyleSetAlignItems(root, YGAlignStretch);
//
//	//YGNodeStyleSetWidth(root, 500);
//	//YGNodeStyleSetHeight(root, 120);
//
//	YGNodeRef child1 = YGNodeNew();
//	YGNodeStyleSetFlexGrow(child1, 1);
//	YGNodeRef child2 = YGNodeNew();
//	YGNodeStyleSetFlexGrow(child2, 1);
//
//	YGNodeInsertChild(root, child1, 0);
//	YGNodeInsertChild(root, child2, 1);
//
//	YGNodeCalculateLayout(root, 500, 120, YGDirectionLTR);
//	print(root, "Root");
//	print(child1, "Child1");
//	print(child2, "Child2");
//
//	YGNodeFreeRecursive(root);
//	return 0;
//}
//
