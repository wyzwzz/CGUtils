#pragma once
#include "window.hpp"
#include "camera.hpp"
namespace wzz::gl{

class gl_app_t:public misc::no_copy_t{
public:
	explicit gl_app_t(window_desc_t desc)
	:desc(std::move(desc)),keyboard( nullptr),mouse( nullptr)
	{

	}

	virtual ~gl_app_t() = default;

	void run(){
		try{
			window = std::make_unique<window_t>(desc);

			keyboard = window->get_keyboard();
			mouse = window->get_mouse();

			initialize();

			while(!window->is_window_should_close())
			{
				window->poll_events();
				window->new_imgui_frame();

				frame();

				window->render_imgui();
				window->swap_buffer();
			}

			destroy();
		}
		catch(const std::exception& err){
			destroy();
			std::cerr<<err.what()<<std::endl;
		}
	}
protected:
	virtual void initialize() { }

	virtual void frame() { }

	virtual void destroy() { }

	virtual void handle_events(){
		if(keyboard->is_down(KEY_ESCAPE))
			window->set_window_close(true);
		if(keyboard->is_down(KEY_LCTRL)){
			mouse->show_cursor(!mouse->is_cursor_visible());
		}
		//update camera
		{
			camera.set_w_over_h(window->get_window_w_over_h());
			if(!mouse->is_cursor_visible()){
				camera.update(
				  {
					.front = keyboard->is_pressed('W'),
					.left  = keyboard->is_pressed('A'),
					.right = keyboard->is_pressed('D'),
					.back  = keyboard->is_pressed('S'),
					.up    = keyboard->is_pressed(KEY_SPACE),
					.down  = keyboard->is_pressed(KEY_LSHIFT),
					.cursor_rel_x = static_cast<float>(mouse->get_delta_cursor_x()),
					.cursor_rel_y = static_cast<float>(mouse->get_delta_cursor_y())
				  });
			}
			camera.recalculate_matrics();
		}
	}
protected:
	std::unique_ptr<window_t> window;
	keyboard_t* keyboard;
	mouse_t* mouse;
	fps_camera_t camera;
private:
	window_desc_t desc;

};

}