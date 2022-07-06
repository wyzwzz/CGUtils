#pragma once

#include <CGUtils/event.hpp>
#include "common.hpp"
#include <bitset>

struct GLFWwindow;

namespace wzz::gl{

enum mouse_button_t:int
{
	Mouse_Button_Left   = 0,
	Mouse_Button_Middle = 1,
	Mouse_Button_Right  = 2
};

class mouse_t{
public:
	~mouse_t();

	bool is_down(mouse_button_t button) const noexcept;
	bool is_up(mouse_button_t button) const noexcept;
	bool is_pressed(mouse_button_t button) const noexcept;

	double get_cursor_x() const noexcept;
	double get_cursor_y() const noexcept;

	double get_delta_cursor_x() const noexcept;
	double get_delta_cursor_y() const noexcept;

	void show_cursor(bool show);
	bool is_cursor_visible() const noexcept;

	void clear();

	void set_window(GLFWwindow* window);

	void update();

	void set_mouse_button_down(mouse_button_t button);

	void set_mouse_button_up(mouse_button_t button);

	void set_scroll(int offset);

private:
	std::bitset<3> pre_frame_table;
	std::bitset<3> cur_frame_table;

	double cur_x = 0, cur_y = 0;
	double rel_x = 0, rel_y = 0;
	bool show = true;
	GLFWwindow* glfw_window;
};
inline mouse_t::~mouse_t()
{
	if(!show)
		show_cursor(true);
}
inline bool mouse_t::is_down( mouse_button_t button ) const noexcept
{
	return !pre_frame_table[button] && cur_frame_table[button];
}
inline bool mouse_t::is_up( mouse_button_t button ) const noexcept
{
	return pre_frame_table[button] && !cur_frame_table[button];
}
inline bool mouse_t::is_pressed( mouse_button_t button ) const noexcept
{
	return  cur_frame_table[button];
}
inline double mouse_t::get_cursor_x() const noexcept
{
	return cur_x;
}
inline double mouse_t::get_cursor_y() const noexcept
{
	return cur_y;
}
inline double mouse_t::get_delta_cursor_x() const noexcept
{
	return rel_x;
}
inline double mouse_t::get_delta_cursor_y() const noexcept
{
	return rel_y;
}

inline bool mouse_t::is_cursor_visible() const noexcept
{
	return show;
}
inline void mouse_t::clear()
{
	pre_frame_table.reset();
	cur_frame_table.reset();
	cur_x = 0, cur_y = 0;
	rel_x = 0, rel_y = 0;

	if(!show)
		show_cursor(true);
}
inline void mouse_t::set_window( GLFWwindow *window )
{
	glfw_window = window;
}

inline void mouse_t::set_mouse_button_down( mouse_button_t button )
{
	if(!is_pressed(button)){
		cur_frame_table[button] = true;
	}
}
inline void mouse_t::set_mouse_button_up( mouse_button_t button )
{
	if( is_pressed(button)){
		cur_frame_table[button] = false;
	}
}
inline void mouse_t::set_scroll( int offset )
{

}


}