/// <summary>
/// Here is where stuff is tested!
/// </summary>
#include "gSplasher/Window.h"
#include "gSplasher/Utils/Primitives.h"
#include "gSplasher/Layouts/BoxLayout.h"
#include "gSplasher/Global.h"
#include "gSplasher/Widget.h"

USING_NAMESPACE

class MyWidget : public WidgetCore {
public:
	MyWidget(WidgetCore* parent = nullptr) : WidgetCore(parent){
		resize(50, 50);
		move(20, 20);
	}

	void paint(Painter &painter) override {
		gPen p(painter);
		p.setColor(gColor(0, 255, 0));
		p.setWidth(0.8);
		gBrush b(painter);
		b.setColor(gColor(255, 0, 0));
		painter.drawRect(gRect(0, 0, size()));
	}
};

class MyWidget2 : public WidgetCore {
public:
	MyWidget2(WidgetCore* parent = nullptr) : WidgetCore(parent) {
		resize(50, 50);
		move(20, 20);
	}

	void paint(Painter &painter) override {
		gPen p(painter);
		gBrush b(painter);
		p.setColor(gColor(255, 0, 0));
		p.setWidth(0.8);
		b.setColor(gColor(0, 255, 0));
		painter.drawRect(gRect(0, 0, size()));
	}
};

int main() {
	Application app;
	Window *window =  new Window{};
	//WidgetCore *window2 = new Window{};
	MyWidget *mywidget = new MyWidget(window);
	MyWidget2 *mywidget2 = new MyWidget2(window);
	MyWidget *mywidget3 = new MyWidget(window);
	MyWidget2 *mywidget4 = new MyWidget2();
	MyWidget *mywidget5 = new MyWidget();
	//MyWidget *mywidget6 = new MyWidget(window);
	//MyWidget *mywidget7 = new MyWidget(window);
	//MyWidget *mywidget8 = new MyWidget(window);

	gBoxLayout<Orientation::Horizontal> *layout = new gBoxLayout<Orientation::Horizontal>{window};
	gBoxLayout<Orientation::Horizontal> *layout2 = new gBoxLayout<Orientation::Horizontal>;
	layout->add(mywidget);
	layout->add(mywidget2);
	layout->add(mywidget3);
	layout->add(layout2);
	layout2->add(mywidget4);
	layout2->add(mywidget5);
	//layout.add(mywidget6);
	//layout.add(mywidget7);
	//layout.add(mywidget8);

	mywidget4->setFixedHeight(100);
	mywidget4->setFixedWidth(450);
	//std::cout << "mywidget to window " << mywidget->mapToWindow(Point(0, 0)) << std::endl;
	//std::cout << "mywidget2 to window " << mywidget2->mapToWindow(Point(0, 0)) << std::endl;
	//std::cout << "mywidget3 " << mywidget3->mapFromGlobal(Point(0, 0)) << std::endl;
	std::cout << "mywidget " << mywidget->geometry() << std::endl;
	std::cout << "mywidget2 " << mywidget2->geometry() << std::endl;
	std::cout << "mywidget3 " << mywidget3->geometry() << std::endl;
	std::cout << "window " << window->geometry() << std::endl;
	std::cout << "topbar " << window->topBar()->geometry() << std::endl;
	std::cout << "layout " << layout->geometry() << std::endl;
	//std::cout << "layout2" << layout2->geometry() << std::endl;
	//WidgetCore *window3 = new Window{};
	//WidgetCore *window4 = new Window{};
	//MyWidget* c_widget = new MyWidget(window);

	//WidgetCore *widget2 = new WidgetCore(widget);
	//WidgetCore *widget3 = new WidgetCore(widget);
	//WidgetCore *widget4 = new WidgetCore(widget);
	//WidgetCore *widget5 = new WidgetCore{};
	//WidgetCore *widget6 = new WidgetCore(widget5);
	//WidgetCore *widget7 = new WidgetCore(widget5);
	//WidgetCore *widget8 = new WidgetCore(widget7);
	//WidgetCore *widget9 = new WidgetCore(widget7);
	//WidgetCore *widget10 = new WidgetCore(widget9);
	//WidgetCore *widget11 = new WidgetCore(widget4);

	app.print_tree(static_cast<Application::CoreList::const_iterator >(*app.core_objects));
	//printf("%d", app.core_objects->tree_size());


	return app.exec();

	return 0;
}

