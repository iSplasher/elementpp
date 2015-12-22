#include "base.h"

int main() {

	gspl::BaseWindow window;
	gspl::WindowManager app(window);

	while (app.run()) {
	}

	return 0;
}

