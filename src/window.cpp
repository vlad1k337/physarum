#include "window.h"

#include <iostream>

#define GLAD_GL_IMPLEMENTATION
#include "glad/gl.h"

void Window::create()
{
	if(!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW\n";
		std::exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(window_width, window_height, "Mold", nullptr, nullptr);
	if(!window)
	{
		std::cerr << "Faild to create GLFW window\n";
		std::exit(-1);
	}

	glfwMakeContextCurrent(window);

	int version = gladLoadGL(glfwGetProcAddress);
	if(version == 0)
	{
		std::cerr << "Failed to initialize OpenGL context\n";
		std::exit(-1);
	}

	std::clog << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "." << GLAD_VERSION_MINOR(version) << "\n";

	glfwSetWindowUserPointer(window, reinterpret_cast<void*>(this));
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(opengl_message_callback, nullptr);

	glViewport(0, 0, window_width, window_height);
}

void Window::cleanup()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow* Window::get_window()
{
	return window;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Window* window_handle = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

	window_handle->window_width = width;
	window_handle->window_width = height;

	glViewport(0, 0, width, height);
}

void Window::opengl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, [[maybe_unused]] GLsizei length, GLchar const* message, [[maybe_unused]] void const* user_param)
{
	auto const src_str = [source]()
	{
		switch (source)
		{
			case GL_DEBUG_SOURCE_API: return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
			default: return "OTHER";
		}
	}();

	auto const type_str = [type]()
	{
		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR: return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER: return "MARKER";
			default: return "OTHER";
		}
	}();

	auto const severity_str = [severity]()
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			case GL_DEBUG_SEVERITY_LOW: return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM: return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH: return "HIGH";
			default: return "OTHER";
		}
	}();

	std::clog << src_str << ", " << type_str << ", " << severity_str << ", " << id << ": " << message << '\n';
}
