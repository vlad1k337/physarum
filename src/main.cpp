#include "window.h"
#include "mold.h"

int main()
{
	Window window;
	window.window_width = 1920;
	window.window_height = 1080;

	window.create();

	TrailMap trail_map;
	trail_map.map_width  = 2550;
	trail_map.map_height = 1440;
	trail_map.decay_rate = 0.01;
	trail_map.create();

	Agents agents;
	agents.agents_count = 1000000;
	agents.map_width = 2550;
	agents.map_height = 1440;
	agents.create();

	while(!glfwWindowShouldClose(window.get_window()))
	{
		agents.dispatch();
		trail_map.draw();

		glfwSwapBuffers(window.get_window());
		glfwPollEvents();
	}

	agents.cleanup();
	trail_map.cleanup();
	window.cleanup();

	return 0;
}
