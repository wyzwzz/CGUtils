#pragma once
#include <bitset>
#include "common.hpp"
#include <CGUtils/event.hpp>
namespace wzz::gl{

using namespace event::keycode_table;

using keycode_t = event::keycode_t;

class keyboard_t{
public:
	bool is_down(keycode_t key) const noexcept;
	bool is_up(keycode_t key) const noexcept;
	bool is_pressed(keycode_t key) const noexcept;
	void clear();
	void update_cur_frame();
	void set_key_down(keycode_t key);
	void set_key_up(keycode_t key);
	void set_char_input(uint32_t key);
private:
	std::bitset<KEY_MAX + 1> pre_frame_table;
	std::bitset<KEY_MAX + 1> cur_frame_table;
};
inline bool keyboard_t::is_down( keycode_t key ) const noexcept
{
	return !pre_frame_table[key] && cur_frame_table[key];
}
inline bool keyboard_t::is_up( keycode_t key ) const noexcept
{
	return pre_frame_table[key] && !cur_frame_table[key];
}
inline bool keyboard_t::is_pressed( keycode_t key ) const noexcept
{
	return cur_frame_table[key];
}
inline void keyboard_t::clear()
{
	pre_frame_table.reset();
	cur_frame_table.reset();
}
inline void keyboard_t::update_cur_frame()
{
	pre_frame_table = cur_frame_table;
}
inline void keyboard_t::set_key_down( keycode_t key )
{
	if(!is_pressed(key)){
		cur_frame_table[key] = true;
	}
}
inline void keyboard_t::set_key_up( keycode_t key )
{
	if( is_pressed(key)){
		cur_frame_table[key] = false;
	}
}
inline void keyboard_t::set_char_input( uint32_t key )
{

}

}