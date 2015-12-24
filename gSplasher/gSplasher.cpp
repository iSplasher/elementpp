#include "include/Base.h"

int main() {

	gspl::BaseWidget window;
	gspl::BaseManager app(window);

	while (app.run()) {
	}

	return 0;
}

