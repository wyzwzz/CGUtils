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
	  show ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}
 void mouse_t::update()
{
	double new_x, new_y;
	glfwGetCursorPos(glfw_window, &new_x, &new_y);

	rel_x = new_x - cur_x;
	rel_y = new_y - cur_y;
	cur_x = new_x;
	cur_y = new_y;


	pre_frame_table = cur_frame_table;
}
}