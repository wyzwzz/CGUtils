#include <CGUtils/api/opengl/window.hpp>
#include <CGUtils/api/imgui/imgui.h>
#include <CGUtils/api/imgui/imgui_impl_glfw.h>
#include <CGUtils/api/imgui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
namespace wzz::gl{

struct GLFWWindowDeleter
{
	void operator()(GLFWwindow *p) const
	{
		glfwDestroyWindow(p);
	}
};

struct window_t::Impl{
	std::unique_ptr<GLFWwindow,GLFWWindowDeleter> window;
};

}