#include <CGUtils/api/opengl/window.hpp>
#include <CGUtils/api/imgui/imgui.h>
#include <CGUtils/api/imgui/imgui_impl_glfw.h>
#include <CGUtils/api/imgui/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
namespace wzz::gl{

namespace {
	window_t* g_window = nullptr;

	struct GLFWWindowDeleter
	{
		void operator()( GLFWwindow *p ) const
		{
			glfwDestroyWindow( p );
		}
	};
	key_button_t transform_key(int key, int scancode, int mods){
		switch ( key )
		{
		case GLFW_KEY_0: return Key_0;
		case GLFW_KEY_1: return Key_1;
		case GLFW_KEY_2: return Key_2;
		case GLFW_KEY_3: return Key_3;
		case GLFW_KEY_4: return Key_4;
		case GLFW_KEY_5: return Key_5;
		case GLFW_KEY_6: return Key_6;
		case GLFW_KEY_7: return Key_7;
		case GLFW_KEY_8: return Key_8;
		case GLFW_KEY_9: return Key_9;
		case GLFW_KEY_A: return Key_A;
		case GLFW_KEY_B: return Key_B;
		case GLFW_KEY_C: return Key_C;
		case GLFW_KEY_D: return Key_D;
		case GLFW_KEY_E: return Key_E;
		case GLFW_KEY_F: return Key_F;
		case GLFW_KEY_G: return Key_G;
		case GLFW_KEY_H: return Key_H;
		case GLFW_KEY_I: return Key_I;
		case GLFW_KEY_J: return Key_J;
		case GLFW_KEY_K: return Key_K;
		case GLFW_KEY_L: return Key_L;
		case GLFW_KEY_M: return Key_M;
		case GLFW_KEY_N: return Key_N;
		case GLFW_KEY_O: return Key_O;
		case GLFW_KEY_P: return Key_P;
		case GLFW_KEY_Q: return Key_Q;
		case GLFW_KEY_R: return Key_R;
		case GLFW_KEY_S: return Key_S;
		case GLFW_KEY_T: return Key_T;
		case GLFW_KEY_U: return Key_U;
		case GLFW_KEY_V: return Key_V;
		case GLFW_KEY_W: return Key_W;
		case GLFW_KEY_X: return Key_X;
		case GLFW_KEY_Y: return Key_Y;
		case GLFW_KEY_Z: return Key_Z;

		case GLFW_KEY_RIGHT: return Key_Right;
		case GLFW_KEY_LEFT: return Key_Left;
		case GLFW_KEY_DOWN: return Key_Down;
		case GLFW_KEY_UP: return Key_Up;

		case GLFW_KEY_ESCAPE: return Key_Esc;

		case GLFW_KEY_KP_0: return Key_0;
		case GLFW_KEY_KP_1: return Key_1;
		case GLFW_KEY_KP_2: return Key_2;
		case GLFW_KEY_KP_3: return Key_3;
		case GLFW_KEY_KP_4: return Key_4;
		case GLFW_KEY_KP_5: return Key_5;
		case GLFW_KEY_KP_6: return Key_6;
		case GLFW_KEY_KP_7: return Key_7;
		case GLFW_KEY_KP_8: return Key_8;
		case GLFW_KEY_KP_9: return Key_9;
		default: std::cerr<<"unsupported key: "<<key<<std::endl;
		}
		return key_button_t::Key_Unknown;
	}
	bool check_valid_desc(const window_desc_t& desc){
		if(desc.res.x <= 0 || desc.res.y <= 0) return false;

		return true;
	}
}

struct window_t::Impl{
	std::unique_ptr<GLFWwindow,GLFWWindowDeleter> window;
	window_desc_t desc;
	Impl() = default;
	~Impl(){
		destroy();
	}
	bool init_window(){
		if(!init_window_context()){
			return false;
		}
		if(!init_opengl_context()){
			return false;
		}
		GL_EXPR(glEnable(GL_DEPTH_TEST));
		if(!init_imgui_context()){
			return false;
		}
		return true;
	}

	bool init_window_context(){
		if(glfwInit() == GLFW_FALSE){
			std::cerr<<"failed to init glfw"<<std::endl;
			return false;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, true);

		auto p_window = glfwCreateWindow(desc.res.x,desc.res.y,desc.title.c_str(),nullptr,nullptr);
		if(!p_window){
			std::cerr<<"create glfw window failed"<<std::endl;
			return false;
		}
		window.reset(p_window);
		//note in multi-threading context
		glfwMakeContextCurrent(window.get());

		glfwSwapInterval(desc.vsync);

		glfwSetKeyCallback(window.get(),glfwKeyCallback);
		glfwSetDropCallback(window.get(),glfwDropFileCallback);
		glfwSetScrollCallback(window.get(),glfwMouseScrollCallback);
		glfwSetCursorPosCallback(window.get(),glfwCursorPosCallback);
		glfwSetMouseButtonCallback(window.get(),glfwMouseButtonCallback);
		glfwSetFramebufferSizeCallback(window.get(),glfwFramebufferSizeCallback);
		return true;
	}

	bool init_opengl_context(){
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
			std::cerr<<"glad load opengl failed"<<std::endl;
			return false;
		}
		return true;
	}

	bool init_imgui_context(){
		IMGUI_CHECKVERSION();
		if(!ImGui::CreateContext()) return false;

		ImGui::StyleColorsDark();
		if(!ImGui_ImplGlfw_InitForOpenGL(window.get(), true)) return false;
		if(!ImGui_ImplOpenGL3_Init()) return false;
		return true;
	}

	void begin_imgui(){
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void end_imgui(){
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	bool get_close_flag(){
		return glfwWindowShouldClose(window.get());
	}

	void dispach_event(){
		glfwPollEvents();
	}

	void swap_buffer(){
		glfwSwapBuffers(window.get());
	}

	void destroy(){
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext(ImGui::GetCurrentContext());
		window.reset();
		glfwTerminate();
	}

	static void glfwCursorPosCallback(GLFWwindow* window,double xpos, double ypos){
		assert(g_window);
		if(!g_window->event_listener.mouse_event) return;
		g_window->event_listener.mouse_event(window,Mouse_Left,Move,xpos,ypos);
	}
	static void glfwMouseButtonCallback(GLFWwindow* window,int button,int action,int mods){
		assert(g_window);
		if(!g_window->event_listener.mouse_event) return;
		double xpos, ypos;
		glfwGetCursorPos( window, &xpos, &ypos );
		int buttons = 0;
		int ea = 0;
		if(action == GLFW_PRESS)
			ea = Press;
		else if(action == GLFW_RELEASE)
			ea = Release;
		else if(action == GLFW_REPEAT)
			ea = Repeat;
		if(button == GLFW_MOUSE_BUTTON_RIGHT)
			buttons |= Mouse_Right;
		if(button == GLFW_MOUSE_BUTTON_LEFT)
			buttons |= Mouse_Left;
		g_window->event_listener.mouse_event(window,(mouse_button_t)buttons,(event_action_t)ea,(int)xpos,(int)ypos);
	}
	static void glfwFramebufferSizeCallback(GLFWwindow* window,int width,int height){
		assert(g_window);
		if(!g_window->event_listener.framebuffer_resize_event) return;
		g_window->event_listener.framebuffer_resize_event(window,width,height);
	}
	static void glfwMouseScrollCallback(GLFWwindow* window,double xoffset,double yoffset){
		assert(g_window);
		if(!g_window->event_listener.scroll_event) return;
		g_window->event_listener.scroll_event(window,xoffset,yoffset);
	}
	static void glfwKeyCallback(GLFWwindow* window,int key,int scancode,int action,int mods){
		assert(g_window);
		if(!g_window->event_listener.keyboard_event) return;
		int ea = 0;
		if(action == GLFW_PRESS)
			ea = Press;
		else if(action == GLFW_RELEASE)
			ea = Release;
		else if(action == GLFW_REPEAT)
			ea = Repeat;
		g_window->event_listener.keyboard_event(window,transform_key(key,scancode,mods),(event_action_t)ea);
	}
	static void glfwDropFileCallback(GLFWwindow* window,int count, const char** df){
		assert(g_window);
		if(!g_window->event_listener.file_drop_event) return;
		g_window->event_listener.file_drop_event(window,count,df);
	}

};



window_t::window_t( const window_desc_t &desc )
:impl(std::make_unique<Impl>())
{
	if(!check_valid_desc(desc)){
		throw std::runtime_error("invalid window desc");
	}
	impl->desc= desc;

	g_window = this;

	if(!impl->init_window()){
		throw std::runtime_error("create opengl window failed");
	}

}

void window_t::run()
{
	try {
		while ( !impl->get_close_flag() ) {
			impl->dispach_event();

			render_frame();

			impl->begin_imgui();

			render_imgui();

			impl->end_imgui();

			impl->swap_buffer();
		}
	}
	catch(const std::exception& err){
		std::cerr<<err.what()<<std::endl;
	}
}

int window_t::width() const
{
	return impl->desc.res.x;
}

int window_t::height() const
{
	return impl->desc.res.y;
}

void window_t::render_frame()
{
	GL_EXPR(glClearColor(1,1,1,1));
	GL_EXPR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void window_t::render_imgui()
{

}

window_t::~window_t()
{

}

}