//
// Created by wyz on 2022/7/5.
//
#include <CGUtils/api/opengl/mouse.hpp>
#include <GLFW/glfw3.h>
namespace wzz::gl{

void mouse_t::show_cursor(bool show)
{
	this->show = show;
	glfwSetInputMode(
	  glfw_window, GLFW_CURSOR,
	  show ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

}