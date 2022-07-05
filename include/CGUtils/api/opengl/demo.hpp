#pragma once
#include "window.hpp"
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
			std::cerr<<err.what()<<std::endl;
		}
	}
protected:
	virtual void initialize() { }

	virtual void frame() { }

	virtual void destroy() { }
protected:
	std::unique_ptr<window_t> window;
	keyboard_t* keyboard;
	mouse_t* mouse;
private:
	window_desc_t desc;

};

}