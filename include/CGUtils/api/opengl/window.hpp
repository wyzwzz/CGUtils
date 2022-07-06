#pragma once

#include "keyboard.hpp"
#include "mouse.hpp"
#include <string>

namespace wzz::gl{

struct window_desc_t{
	vec2i size = {1280,720};
	std::string title = "OpenGL Window";

	bool resizeable = true;
	bool fullscreen = false;
	bool vsync = true;
	bool imgui = true;
	int multisamples = 0;
	enum DepthStencilFormat{
		depth24_stencil8,
		depth32_stencil0,
		depth32_stencil8,
	};
	DepthStencilFormat depth_stencil_format = depth24_stencil8;
};

class window_t: public misc::no_copy_t{
public:
	explicit window_t(const window_desc_t& desc,bool maximazed = false);

	~window_t();

	void new_imgui_frame();

	void render_imgui();

	keyboard_t* get_keyboard() noexcept;

	mouse_t* get_mouse() noexcept;

	void poll_events();

	bool is_focused();

	void waiting_for_focus();

	bool is_window_should_close() const noexcept;

	void set_window_close(bool close) const noexcept;

	void set_maximized();

	int get_window_width() const noexcept;
	int get_window_height() const noexcept;
	vec2i get_window_size() const noexcept;
	double get_window_w_over_h() const noexcept;

	void swap_buffer();



	bool is_vsync() const noexcept;
	void set_vsync(bool vsync) noexcept;

	void use_default_viewport() const;

	bool is_render_imgui() const noexcept;

protected:
	//call by internal not user
	void resize();
	void set_key_event(keycode_t keycode, bool pressed);
	void set_mouse_button_event(mouse_button_t button,bool pressed);
	void set_char_input_event(uint32_t ch);
	void set_mouse_scroll_event(int offset);
	void set_focus(bool focus);
	void set_cursor_pos_event(double x,double y);
private:
	friend class WindowParser;
	struct Impl;
	std::unique_ptr<Impl> impl;

};

}