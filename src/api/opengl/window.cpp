#pragma once
#include <array>
#include <CGUtils/api/opengl/window.hpp>
#include <CGUtils/api/imgui/imgui.h>
#include <CGUtils/api/imgui/imgui_impl_glfw.h>
#include <CGUtils/api/imgui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

namespace wzz::gl{

struct window_t::Impl{

	GLFWwindow* glfw_window = nullptr;

	bool vsync = false;
	bool close = false;
	bool focus = false;

	vec2i framebuffer_size;

	keyboard_t keyboard;

	mouse_t mouse;

	bool imgui = false;
};

class WindowParser {
public:
	struct glfw_keycode_table{
		std::array<keycode_t,GLFW_KEY_LAST + 1> keys;
		glfw_keycode_table(){
			for(auto &k : keys)
				k = KEY_UNKNOWN;

			keys[GLFW_KEY_SPACE]      = KEY_SPACE;
			keys[GLFW_KEY_APOSTROPHE] = KEY_APOSTROPHE;
			keys[GLFW_KEY_COMMA]      = KEY_COMMA;
			keys[GLFW_KEY_MINUS]      = KEY_MINUS;
			keys[GLFW_KEY_PERIOD]     = KEY_PERIOD;
			keys[GLFW_KEY_SLASH]      = KEY_SLASH;

			keys[GLFW_KEY_SEMICOLON] = KEY_SEMICOLON;
			keys[GLFW_KEY_EQUAL]     = KEY_EQUAL;

			keys[GLFW_KEY_LEFT_BRACKET]  = KEY_LBRAC;
			keys[GLFW_KEY_BACKSLASH]     = KEY_BACKSLASH;
			keys[GLFW_KEY_RIGHT_BRACKET] = KEY_RBRAC;
			keys[GLFW_KEY_GRAVE_ACCENT]  = KEY_GRAVE_ACCENT;
			keys[GLFW_KEY_ESCAPE]        = KEY_ESCAPE;

			keys[GLFW_KEY_ENTER]     = KEY_ENTER;
			keys[GLFW_KEY_TAB]       = KEY_TAB;
			keys[GLFW_KEY_BACKSPACE] = KEY_BACKSPACE;
			keys[GLFW_KEY_INSERT]    = KEY_INSERT;
			keys[GLFW_KEY_DELETE]    = KEY_DELETE;

			keys[GLFW_KEY_RIGHT] = KEY_RIGHT;
			keys[GLFW_KEY_LEFT]  = KEY_LEFT;
			keys[GLFW_KEY_DOWN]  = KEY_DOWN;
			keys[GLFW_KEY_UP]    = KEY_UP;

			keys[GLFW_KEY_HOME] = KEY_HOME;
			keys[GLFW_KEY_END]  = KEY_END;

			keys[GLFW_KEY_F1]  = KEY_F1;
			keys[GLFW_KEY_F2]  = KEY_F2;
			keys[GLFW_KEY_F3]  = KEY_F3;
			keys[GLFW_KEY_F4]  = KEY_F4;
			keys[GLFW_KEY_F5]  = KEY_F5;
			keys[GLFW_KEY_F6]  = KEY_F6;
			keys[GLFW_KEY_F7]  = KEY_F7;
			keys[GLFW_KEY_F8]  = KEY_F8;
			keys[GLFW_KEY_F9]  = KEY_F9;
			keys[GLFW_KEY_F10] = KEY_F10;
			keys[GLFW_KEY_F11] = KEY_F11;
			keys[GLFW_KEY_F12] = KEY_F12;

			keys[GLFW_KEY_KP_0] = KEY_NUMPAD_0;
			keys[GLFW_KEY_KP_1] = KEY_NUMPAD_1;
			keys[GLFW_KEY_KP_2] = KEY_NUMPAD_2;
			keys[GLFW_KEY_KP_3] = KEY_NUMPAD_3;
			keys[GLFW_KEY_KP_4] = KEY_NUMPAD_4;
			keys[GLFW_KEY_KP_5] = KEY_NUMPAD_5;
			keys[GLFW_KEY_KP_6] = KEY_NUMPAD_6;
			keys[GLFW_KEY_KP_7] = KEY_NUMPAD_7;
			keys[GLFW_KEY_KP_8] = KEY_NUMPAD_8;
			keys[GLFW_KEY_KP_9] = KEY_NUMPAD_9;

			keys[GLFW_KEY_KP_DECIMAL]  = KEY_NUMPAD_DECIMAL;
			keys[GLFW_KEY_KP_DIVIDE]   = KEY_NUMPAD_DIV;
			keys[GLFW_KEY_KP_MULTIPLY] = KEY_NUMPAD_MUL;
			keys[GLFW_KEY_KP_SUBTRACT] = KEY_NUMPAD_SUB;
			keys[GLFW_KEY_KP_ADD]      = KEY_NUMPAD_ADD;
			keys[GLFW_KEY_KP_ENTER]    = KEY_NUMPAD_ENTER;

			keys[GLFW_KEY_LEFT_SHIFT]    = KEY_LSHIFT;
			keys[GLFW_KEY_LEFT_CONTROL]  = KEY_LCTRL;
			keys[GLFW_KEY_LEFT_ALT]      = KEY_LALT;
			keys[GLFW_KEY_RIGHT_SHIFT]   = KEY_RSHIFT;
			keys[GLFW_KEY_RIGHT_CONTROL] = KEY_RCTRL;
			keys[GLFW_KEY_RIGHT_ALT]     = KEY_RALT;

			for(int i = 0; i < 9; ++i)
				keys['0' + i] = KEY_D0 + i;

			for(int i = 0; i < 26; ++i)
				keys['A' + i] = KEY_A + i;
		}
	};

	inline static std::unordered_map<GLFWwindow*,window_t*> glfw_window_mp;

	static bool get_window(GLFWwindow* glfw_window,window_t*& window){
		for(auto& item:glfw_window_mp){
			if(item.first == glfw_window && glfw_window){
				window = item.second;
				return true;
			}
		}
		return false;
	}

#define GET_WINDOW                            \
	window_t *window = nullptr;               \
	if ( !get_window( glfw_window, window ) ) \
		return;

	static void glfw_close_callback(GLFWwindow* glfw_window){
		GET_WINDOW
		window->set_window_close( glfwWindowShouldClose(glfw_window));
		glfwSetWindowShouldClose(glfw_window,GLFW_FALSE);
	}
	static void glfw_resize_callback(GLFWwindow* glfw_window,int width,int height){
		GET_WINDOW
		window->resize();
	}
	static void glfw_focus_callback(GLFWwindow* glfw_window,int focus){
		GET_WINDOW
		window->set_focus(focus);
	}
	inline static const glfw_keycode_table table;
	static void glfw_key_callback(GLFWwindow* glfw_window,int key,int scancode,int action,int mods){
		GET_WINDOW
		if(window->is_render_imgui())
			ImGui_ImplGlfw_KeyCallback(glfw_window,key,scancode,action,mods);
		if(key < 0 || key >= table.keys.size())
			return;
		const auto keycode = table.keys[key];
		if(keycode == KEY_UNKNOWN)
			return;
		if(action == GLFW_PRESS)
			window->set_key_event(keycode,true);
		else if(action == GLFW_RELEASE)
			window->set_key_event(keycode, false);
	}
	static void glfw_char_callback(GLFWwindow* glfw_window,uint32_t c){
		GET_WINDOW
		if(window->is_render_imgui())
			ImGui_ImplGlfw_CharCallback(glfw_window,c);
		window->set_char_input_event(c);
	}
	static void glfw_mouse_button_callback(GLFWwindow* glfw_window,int button,int action,int mods){
		GET_WINDOW
		if(window->is_render_imgui())
			ImGui_ImplGlfw_MouseButtonCallback(glfw_window,button,action,mods);
		mouse_button_t btn;
		if(button == GLFW_MOUSE_BUTTON_LEFT)
			btn = Mouse_Button_Left;
		else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
			btn = Mouse_Button_Middle;
		else if(button == GLFW_MOUSE_BUTTON_RIGHT)
			btn = Mouse_Button_Right;
		else
			return;

		if(action == GLFW_PRESS)
			window->set_mouse_button_event(btn,true);
		else if(action == GLFW_RELEASE)
			window->set_mouse_button_event(btn,false);
	}

	static void glfw_cursor_callback(GLFWwindow* glfw_window,double x,double y){
		GET_WINDOW
		if(window->is_render_imgui())
			ImGui_ImplGlfw_CursorPosCallback(glfw_window,x,y);
		window->set_cursor_pos_event(x,y);
	}
	static void glfw_scroll_callback(GLFWwindow* glfw_window,double xoffset,double yoffset){
		GET_WINDOW
		if(window->is_render_imgui())
			ImGui_ImplGlfw_ScrollCallback(glfw_window,xoffset,yoffset);
		window->set_mouse_scroll_event(static_cast<int>(yoffset));
	}
};

window_t::window_t( const window_desc_t &desc, bool maximazed )
{
	impl = std::make_unique<Impl>();

	auto& glfw_windows_mp = WindowParser::glfw_window_mp;

	if(glfw_windows_mp.empty()){
		if(glfwInit() != GLFW_TRUE){
			throw std::runtime_error("failed to init glfw");
		}
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
	glfwWindowHint(GLFW_RESIZABLE,desc.resizeable ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_MAXIMIZED,maximazed ? GLFW_TRUE : GLFW_FALSE);

	switch ( desc.depth_stencil_format ) {
	case window_desc_t::depth24_stencil8:
		glfwWindowHint(GLFW_DEPTH_BITS,24);
		glfwWindowHint(GLFW_STENCIL_BITS,8);
		break ;
	case window_desc_t::depth32_stencil0:
		glfwWindowHint(GLFW_DEPTH_BITS,32);
		glfwWindowHint(GLFW_STENCIL_BITS,0);
		break ;
	case window_desc_t::depth32_stencil8:
		glfwWindowHint(GLFW_DEPTH_BITS,32);
		glfwWindowHint(GLFW_STENCIL_BITS,8);
		break ;
	}

	glfwWindowHint(GLFW_SAMPLES,desc.multisamples);

	impl->glfw_window = glfwCreateWindow(
	  desc.size.x,desc.size.y,desc.title.c_str(),
	  desc.fullscreen ? glfwGetPrimaryMonitor() : nullptr,nullptr);

	if(!impl->glfw_window){
		throw std::runtime_error("failed to create glfw window");
	}

	glfwMakeContextCurrent(impl->glfw_window);
	glfwFocusWindow(impl->glfw_window);
	impl->focus = true;

	set_vsync(desc.vsync);

	if(glfw_windows_mp.empty()){
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
			throw std::runtime_error("glad failed to load opengl");
		}
	}
	glfw_windows_mp.insert({impl->glfw_window,this});

	impl->mouse.set_window(impl->glfw_window);

	glfwSetWindowCloseCallback(impl->glfw_window,WindowParser::glfw_close_callback);
	glfwSetFramebufferSizeCallback(impl->glfw_window,WindowParser::glfw_resize_callback);
	glfwSetWindowFocusCallback(impl->glfw_window,WindowParser::glfw_focus_callback);
	glfwSetKeyCallback(impl->glfw_window,WindowParser::glfw_key_callback);
	glfwSetMouseButtonCallback(impl->glfw_window,WindowParser::glfw_mouse_button_callback);
	glfwSetCharCallback(impl->glfw_window,WindowParser::glfw_char_callback);
//	glfwSetCursorPosCallback(impl->glfw_window,WindowParser::glfw_cursor_callback);

	resize();

	impl->imgui = desc.imgui;
	if(desc.imgui){
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(impl->glfw_window,false);
		ImGui_ImplOpenGL3_Init("#version 460");
	}
}
window_t::~window_t()
{

	if(impl->imgui)
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext(ImGui::GetCurrentContext());
	}

	glfwSetWindowCloseCallback    (impl->glfw_window, nullptr);
	glfwSetFramebufferSizeCallback(impl->glfw_window, nullptr);
	glfwSetWindowFocusCallback    (impl->glfw_window, nullptr);
	glfwSetKeyCallback            (impl->glfw_window, nullptr);
	glfwSetMouseButtonCallback    (impl->glfw_window, nullptr);
	glfwSetScrollCallback         (impl->glfw_window, nullptr);
	glfwSetCharCallback           (impl->glfw_window, nullptr);

	auto& glfw_windows_mp = WindowParser::glfw_window_mp;
	glfw_windows_mp.erase(impl->glfw_window);

	glfwDestroyWindow(impl->glfw_window);

	if(glfw_windows_mp.empty())
		glfwTerminate();
}
void window_t::new_imgui_frame()
{
	assert(impl->imgui);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void window_t::render_imgui()
{
	assert(impl->imgui);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
keyboard_t *window_t::get_keyboard() noexcept
{
	return &impl->keyboard;
}
mouse_t *window_t::get_mouse() noexcept
{
	return &impl->mouse;
}
void window_t::poll_events()
{
	impl->keyboard.update_cur_frame();
	impl->mouse.update();
	glfwPollEvents();
}
bool window_t::is_focused()
{
	return impl->focus;
}
void window_t::waiting_for_focus()
{
}
bool window_t::is_window_should_close() const noexcept
{
	return impl->close;
}
void window_t::set_window_close( bool close ) const noexcept
{
	impl->close = true;
}
void window_t::set_maximized()
{
	glfwMaximizeWindow(impl->glfw_window);
}
int window_t::get_window_width() const noexcept
{
	return impl->framebuffer_size.x;
}
int window_t::get_window_height() const noexcept
{
	return impl->framebuffer_size.y;
}
vec2i window_t::get_window_size() const noexcept
{
	return impl->framebuffer_size;
}
double window_t::get_window_w_over_h() const noexcept
{
	return static_cast<double>(impl->framebuffer_size.x) /
	  static_cast<double>(impl->framebuffer_size.y);
}
void window_t::swap_buffer()
{
	glfwSwapBuffers(impl->glfw_window);
}
void window_t::resize()
{
	glfwGetFramebufferSize(
	  impl->glfw_window,
	  &impl->framebuffer_size.x,
	  &impl->framebuffer_size.y);

	use_default_viewport();
}
bool window_t::is_vsync() const noexcept
{
	return impl->vsync;
}
void window_t::set_vsync( bool vsync ) noexcept
{
	impl->vsync = vsync;
	glfwSwapInterval(vsync ? 1 : 0);
}
void window_t::use_default_viewport() const
{
	glViewport(0, 0, impl->framebuffer_size.x, impl->framebuffer_size.y);
}
bool window_t::is_render_imgui() const noexcept
{
	return impl->imgui;
}
void window_t::set_key_event( keycode_t keycode, bool pressed )
{
	if(pressed)
		impl->keyboard.set_key_down(keycode);
	else
		impl->keyboard.set_key_up(keycode);
}
void window_t::set_mouse_button_event( mouse_button_t button, bool pressed )
{
	if(pressed)
		impl->mouse.set_mouse_button_down(button);
	else
		impl->mouse.set_mouse_button_up(button);
}
void window_t::set_char_input_event( uint32_t ch )
{
	impl->keyboard.set_char_input(ch);
}
void window_t::set_mouse_scroll_event( int offset )
{
	impl->mouse.set_scroll(offset);
}
void window_t::set_focus( bool focus )
{
	impl->focus = focus;
}
void window_t::set_cursor_pos_event( double x, double y )
{
	impl->mouse.update();
}

}