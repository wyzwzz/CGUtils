#pragma once
#include "common.hpp"
namespace wzz::gl{

enum event_action_t{
	Press = 0x01,
	Release = 0x02,
	Repeat = 0x03,
	Move = 0x04
};

enum mouse_button_t{
	Mouse_Left = 0x01,
	Mouse_Right = 0x02
};

enum key_button_t{
	Key_Unknown,
	Key_0,
	Key_1,
	Key_2,
	Key_3,
	Key_4,
	Key_5,
	Key_6,
	Key_7,
	Key_8,
	Key_9,

	Key_A,
	Key_B,
	Key_C,
	Key_D,
	Key_E,
	Key_F,
	Key_G,
	Key_H,
	Key_I,
	Key_J,
	Key_K,
	Key_L,
	Key_M,
	Key_N,
	Key_O,
	Key_P,
	Key_Q,
	Key_R,
	Key_S,
	Key_T,
	Key_U,
	Key_V,
	Key_W,
	Key_X,
	Key_Y,
	Key_Z,

	Key_Up,
	Key_Down,
	Key_Left,
	Key_Right,

	Key_Esc
};

using MouseEventCallback = std::function<void(void *, mouse_button_t, event_action_t, int, int)>;
using ScrollEventCallback = std::function<void(void *, int, int)>;
using KeyboardEventCallback = std::function<void(void *, key_button_t, event_action_t)>;
using FileDropEventCallback = std::function<void(void *, int, const char **)>;
using FramebufferResizeEventCallback = std::function<void(void *, int, int)>;

struct EventListenerTraits
{
	MouseEventCallback mouse_event;
	ScrollEventCallback scroll_event;
	KeyboardEventCallback keyboard_event;
	FileDropEventCallback file_drop_event;
	FramebufferResizeEventCallback framebuffer_resize_event;
};

struct window_desc_t{
	vec2i res = {1280,720};
	std::string title = "OpenGL Window";
	bool vsync = true;
};

class window_t: public misc::no_copy_t{
public:
	explicit window_t(const window_desc_t& desc);

	~window_t();

	void run();

	int width() const;

	int height() const;
protected:
	virtual void render_frame();

	virtual void render_imgui();

protected:
	EventListenerTraits event_listener;

private:
	struct Impl;
	std::unique_ptr<Impl> impl;

};

}