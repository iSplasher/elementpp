#include "catch.hpp"

#include <GLFW/glfw3.h>


SCENARIO("Window objects can be created and destroyed", "[Window][Widget]") {

	GIVEN("Window objects are created") {
		glfwInit();
		glfwWindowHint(GLFW_DECORATED, 0);
		GLFWwindow *window = glfwCreateWindow(640, 480, "Undecorated Resizable", 0, 0);
		glfwMakeContextCurrent(window);


		int win_x, win_y;
		int mouse_x, mouse_y;
		bool dragging = false;

		while (!glfwWindowShouldClose(window)) {
			bool left_button_pressed = GLFW_PRESS == glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

			if (!left_button_pressed)
				dragging = false;
			else
			{
				if (!dragging)
				{
					// When we start dragging, we need to set us up the data.
					dragging = true;
					glfwGetWindowPos(window, &win_x, &win_y);

					double m_x, m_y;
					glfwGetCursorPos(window, &m_x, &m_y);
					mouse_x = int(m_x), mouse_y = int(m_y);
				}
				else        // We're already dragging, so we must reposition
				{
					double m_x, m_y;
					glfwGetCursorPos(window, &m_x, &m_y);

					win_x = win_x + int(m_x) - mouse_x;
					win_y = win_y + int(m_y) - mouse_y;

					glfwSetWindowPos(window, win_x, win_y);
				}
			}

			glfwSwapBuffers(window);
			glfwWaitEvents();
		}
	}
}