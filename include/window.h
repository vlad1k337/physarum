#ifndef __WINDOW_H
#define __WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "glad/gl.h"

class Window {
public:
	void create();
	void cleanup();

	GLFWwindow* get_window();

	uint32_t window_width  {800};
	uint32_t window_height {600};

private:
	GLFWwindow* window {};

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void opengl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param);
};

#endif
