//
// Created by wyz on 2022/7/6.
//
#include <CGUtils/api.hpp>
#include <CGUtils/model.hpp>
using namespace wzz::gl;
using namespace wzz::model;
using namespace wzz::math;
class OpenGLDemo:public gl_app_t{
public:

	using gl_app_t::gl_app_t;

private:
	void initialize() override {
		GL_EXPR(glEnable(GL_DEPTH_TEST));
		auto vshader = shader_t<GL_VERTEX_SHADER>::from_file("mesh.vert");
		auto fshader = shader_t<GL_FRAGMENT_SHADER>::from_file("mesh.frag");
		shader = program_t::build_from(vshader,fshader);
		shader.bind();
		shader.set_uniform_var("AlbedoMap",0);
		shader.unbind();
		auto model = load_model_from_obj_file("Marry.obj","Marry.mtl");
		for(auto& mesh:model->meshes){
			draw_models.emplace_back();
			auto& m = draw_models.back();
			m.vbo.initialize_handle();
			m.ebo.initialize_handle();
			m.vbo.reinitialize_buffer_data(mesh.vertices.data(),mesh.vertices.size(),GL_STATIC_DRAW);
			m.ebo.reinitialize_buffer_data(mesh.indices.data(),mesh.indices.size(),GL_STATIC_DRAW);
			m.vao.initialize_handle();
			m.vao.bind_vertex_buffer_to_attrib(attrib_var_t<vec3f>(0),m.vbo,&vertex_t::pos,0);
			m.vao.bind_vertex_buffer_to_attrib(attrib_var_t<vec3f>(1),m.vbo,&vertex_t::normal,1);
			m.vao.bind_vertex_buffer_to_attrib(attrib_var_t<vec2f>(2),m.vbo,&vertex_t::tex_coord,2);
			m.vao.enable_attrib(attrib_var_t<vec3f>(0));
			m.vao.enable_attrib(attrib_var_t<vec3f>(1));
			m.vao.enable_attrib(attrib_var_t<vec2f>(2));
			m.vao.bind_index_buffer(m.ebo);
			if(mesh.material != -1){
				m.albedo_tex.initialize_handle();
				m.albedo_tex.initialize_format_and_data(1,GL_RGB8,model->materials[mesh.material].albedo);
			}
		}
		camera.set_position({0.0,0.0,2.0});
		camera.set_direction( deg2rad(-90.0),0);
		camera.set_perspective(45.0,0.1,20.0);
	}

	void frame() override {
		handle_events();
		//gui
		static float x = 1.f;
		if(ImGui::Begin("Settings",nullptr,ImGuiWindowFlags_AlwaysAutoResize)){
			ImGui::Text("Press LCtrl to show/hide cursor");

			ImGui::Text("FPS: %.0f",ImGui::GetIO().Framerate);
			ImGui::SliderFloat("test x",&x,0,10);
		}
		ImGui::End();

		//render
		GL_EXPR(glClearColor(0,0,0,0));
		GL_EXPR(glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT));

		shader.bind();
		shader.set_uniform_var("model",mat4());
		shader.set_uniform_var("vp",camera.get_view_proj());
		for(auto& d:draw_models){
			d.vao.bind();

			d.albedo_tex.bind(0);
			GL_EXPR(glDrawElements(GL_TRIANGLES,d.ebo.index_count(),GL_UNSIGNED_INT,nullptr));

			d.vao.unbind();
		}

		shader.unbind();

	}

	void destroy() override {

	}
private:
	program_t shader;
	struct DrawModel{
		vertex_buffer_t<vertex_t> vbo;
		index_buffer_t<uint32_t> ebo;
		vertex_array_t vao;
		texture2d_t albedo_tex;
	};
	std::vector<DrawModel> draw_models;
};
int main(){
	OpenGLDemo(
	  window_desc_t
	  {
		.size = {1280,720},
		.title = "OpenGL Test",
		.multisamples = 8
	  }).run();
}