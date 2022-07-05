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

	double get_cursor_lock_x() const noexcept;
	double get_cursor_lock_y() const noexcept;
	void set_cursor_lock(bool lock,double lock_x,double lock_y);
	bool is_cursor_locked() const noexcept;

	void show_cursor(bool show);
	bool is_cursor_visible() const noexcept;

	void clear();

	void set_window(GLFWwindow* window);

	void update();

	void set_mouse_button_down(mouse_button_t button);

	void set_mouse_button_up(mouse_button_t button);

	void set_scroll(int offset);

	void set_cursor_pos(double x,double y);

private:
	std::bitset<3> pre_frame_table;
	std::bitset<3> cur_frame_table;

	double cur_x = 0, cur_y = 0;
	double rel_x = 0, rel_y = 0;
	bool is_locked = false;
	double lock_x = 0, lock_y = 0;
	bool show = true;
	GLFWwindow* glfw_window;
};
mouse_t::~mouse_t()
{
	if(!show)
		show_cursor(true);
}
bool mouse_t::is_down( mouse_button_t button ) const noexcept
{
	return !pre_frame_table[button] && cur_frame_table[button];
}
bool mouse_t::is_up( mouse_button_t button ) const noexcept
{
	return pre_frame_table[button] && !cur_frame_table[button];
}
bool mouse_t::is_pressed( mouse_button_t button ) const noexcept
{
	return pre_frame_table[button] && cur_frame_table[button];
}
double mouse_t::get_cursor_x() const noexcept
{
	return cur_x;
}
double mouse_t::get_cursor_y() const noexcept
{
	return cur_y;
}
double mouse_t::get_delta_cursor_x() const noexcept
{
	return rel_x;
}
double mouse_t::get_delta_cursor_y() const noexcept
{
	return rel_y;
}
double mouse_t::get_cursor_lock_x() const noexcept
{
	return lock_x;
}
double mouse_t::get_cursor_lock_y() const noexcept
{
	return lock_y;
}
void mouse_t::set_cursor_lock( bool lock, double lock_x, double lock_y )
{
	is_locked = lock;
	this->lock_x = lock_x;
	this->lock_y = lock_y;
}
bool mouse_t::is_cursor_locked() const noexcept
{
	return is_locked;
}

bool mouse_t::is_cursor_visible() const noexcept
{
	return show;
}
void mouse_t::clear()
{
	pre_frame_table.reset();
	cur_frame_table.reset();
	cur_x = 0, cur_y = 0;
	rel_x = 0, rel_y = 0;
	is_locked = false;
	lock_x = 0, lock_y = 0;
	if(!show)
		show_cursor(true);
}
void mouse_t::set_window( GLFWwindow *window )
{
	glfw_window = window;
}
void mouse_t::update()
{
}
void mouse_t::set_mouse_button_down( mouse_button_t button )
{
	if(!is_pressed(button)){
		cur_frame_table[button] = true;
	}
}
void mouse_t::set_mouse_button_up( mouse_button_t button )
{
	if( is_pressed(button)){
		cur_frame_table[button] = false;
	}
}
void mouse_t::set_scroll( int offset )
{

}
void mouse_t::set_cursor_pos( double x, double y )
{
	rel_x = x - cur_x;
	rel_y = y - cur_y;
	cur_x = x;
	cur_y = y;

}

}