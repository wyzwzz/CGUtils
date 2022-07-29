#include <CGUtils/model/model.hpp>
#include <CGUtils/string/std_string.hpp>
#include <CGUtils/file/raw_file_io.hpp>
#include <CGUtils/console/logger.hpp>
#include <CGUtils/image.hpp>

#include <unordered_set>

#include "../ext/tiny_obj_loader.h"

#include "../ext/tiny_gltf.h"


namespace wzz::model{

std::unique_ptr<model_t> _load_model_from_obj_memory(const std::string& str,const std::string& mtl,
											   bool load_materials,bool compute_mesh_ext){
	auto model = std::make_unique<model_t>();
	auto& meshes = model->meshes;
	tinyobj::ObjReader reader;
	if(!reader.ParseFromString(str,mtl)){
		throw std::runtime_error(reader.Error());
	}
	if(!reader.Warning().empty()){
		std::cerr<<reader.Warning()<<std::endl;
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	auto& materials = reader.GetMaterials();

	for(auto& shape:shapes){
		for (auto face_vertex_count : shape.mesh.num_face_vertices)
		{
			if (face_vertex_count != 3)
			{
				throw std::runtime_error("invalid obj face vertex count: " + std::to_string(+face_vertex_count));
			}
		}

		if (shape.mesh.indices.size() % 3 != 0)
		{
			throw std::runtime_error("invalid obj index count: " + std::to_string(shape.mesh.indices.size()));
		}

		const size_t triangle_count = shape.mesh.indices.size() / 3;
		const size_t vertex_count = attrib.vertices.size() / 3;

		LOG_INFO("triangle count {0}", triangle_count);
		LOG_INFO("vertex count {0}", vertex_count);

		meshes.emplace_back();
		auto &mesh = meshes.back();
		uint32_t vertex_index = 0;
		auto& vertices = mesh.vertices;
		for(size_t i = 0; i < triangle_count; ++i){
			for(int k = 0; k < 3; ++k){
				auto& index = shape.mesh.indices[i * 3 + k];
				vertices.emplace_back();
				auto& vertex = vertices.back();
				vertex.pos = { attrib.vertices[3 * index.vertex_index],
							   attrib.vertices[3 * index.vertex_index + 1],
							   attrib.vertices[3 * index.vertex_index + 2]};
				vertex.normal = { attrib.normals[3 * index.normal_index],
								  attrib.normals[3 * index.normal_index + 1],
								  attrib.normals[3 * index.normal_index + 2]};
				vertex.tex_coord = {attrib.texcoords[2 * index.texcoord_index],
									attrib.texcoords[2 * index.texcoord_index + 1]};
				mesh.indices.emplace_back(vertex_index++);
			}
		}
		if(compute_mesh_ext){
			auto& pos_array = mesh.pos;
			auto& normal_array = mesh.normal;
			auto& uv_array = mesh.tex_coord;
			for(size_t i = 0; i < triangle_count; ++i){
				for(int k = 0; k < 3; ++k){
					auto& index = shape.mesh.indices[i * 3 + k];

					pos_array.emplace_back( attrib.vertices[3 * index.vertex_index],
								   attrib.vertices[3 * index.vertex_index + 1],
								   attrib.vertices[3 * index.vertex_index + 2]);
					normal_array.emplace_back( attrib.normals[3 * index.normal_index],
									  attrib.normals[3 * index.normal_index + 1],
									  attrib.normals[3 * index.normal_index + 2]);
					uv_array.emplace_back(attrib.texcoords[2 * index.texcoord_index],
										 attrib.texcoords[2 * index.texcoord_index + 1]);
				}
			}
			assert(pos_array.size() == vertices.size());
		}
		if(load_materials){
			std::unordered_set<int> ms;
			for(auto& m:shape.mesh.material_ids){
				ms.insert(m);
			}
			if(ms.size() > 1){
				throw std::runtime_error("mesh has more than one material");
			}
			mesh.material = *ms.begin();
		}
	}
	LOG_INFO("total load mesh count: {}",model->meshes.size());
	if(load_materials){
		auto load_and_map = [](const std::string& filename,texture::image2d_t<math::color3b>& image,const math::vec3f s){
			image = texture::image2d_t<math::color3b>(image::load_rgb_from_file(filename));
			image.map_inplace([&](math::color3b& c){
				c.r *= s.x;
				c.g *= s.y;
				c.b *= s.z;
			});
		};
		for(auto& m:materials){
			model->materials.emplace_back();
			auto& material = model->materials.back();
			material.name = m.name;
			if(!m.diffuse_texname.empty()){
				load_and_map(m.diffuse_texname,material.albedo,{m.diffuse[0],m.diffuse[1],m.diffuse[2]});
				if(m.diffuse[0] == 0 && m.diffuse[1] == 0 && m.diffuse[2] == 0){
					LOG_ERROR("diffuse is zero but has texture");
				}
			}
			else if(m.diffuse[0] != 0 && m.diffuse[1] != 0 && m.diffuse[2] != 0){
				math::vec3f d = {m.diffuse[0],m.diffuse[1],m.diffuse[2]};
				d.x = std::clamp<float>(d.x,0,1);
				d.y = std::clamp<float>(d.y,0,1);
				d.z = std::clamp<float>(d.z,0,1);
				material.albedo = texture::image2d_t<math::color3b>(2,2,math::color3b(d.x * 255,d.y * 255,d.z * 255));
			}
			material.albedo.flip_vertically_inplace();
			if(!m.normal_texname.empty()){
				material.normal = texture::image2d_t<math::color3b>(image::load_rgb_from_file(m.normal_texname));
			}
			material.normal.flip_vertically_inplace();
			//todo handle other

		}
		LOG_INFO("total load materials count: {}",materials.size());
	}
	return std::move(model);
}

std::vector<triangle_t> extract_triangle_from_model(const model_t& model){
	std::vector<triangle_t> triangles;
	for(const auto& mesh : model.meshes){
		size_t index_count = mesh.indices.size();
		size_t triangle_count = index_count / 3;
		const auto& vertices = mesh.vertices;
		const auto& indices = mesh.indices;
		for(size_t i = 0; i < triangle_count; ++i){
			triangles.emplace_back();
			auto& triangle = triangles.back();
			triangle.vertices[0] = vertices[indices[i * 3 + 0]];
			triangle.vertices[1] = vertices[indices[i * 3 + 1]];
			triangle.vertices[2] = vertices[indices[i * 3 + 2]];
		}
	}
	return std::move(triangles);
}


std::vector<triangle_t> load_triangle_from_obj_memory(const std::string& str){
	return extract_triangle_from_model(
	  *_load_model_from_obj_memory( str, "", false, false ));
}


std::vector<triangle_t> load_triangle_from_file(const std::string& filename){
	if(stdstr::ends_with(filename,".obj")){
		return load_triangle_from_obj_file(filename);
	}
	else if(stdstr::ends_with(filename,".gltf")){
		return load_triangle_from_gltf_file(filename);
	}
	else{
		throw std::runtime_error("unsupported file format");
	}
}

std::vector<triangle_t> load_triangle_from_obj_file(const std::string& filename){
	return load_triangle_from_obj_memory(file::read_txt_file(filename));
}

std::vector<triangle_t> load_triangle_from_gltf_file(const std::string& filename){
	if(stdstr::ends_with(filename,".gltf")){
//		return load_from_gltf_memory(file::read_txt_file(filename));
	}
	//todo load from .bin
	return {};
}



std::vector<mesh_t> load_mesh_from_file(const std::string& filename){
	if(stdstr::ends_with(filename,".obj")){
		return load_mesh_from_obj_file(filename);
	}
	else if(stdstr::ends_with(filename,".gltf")){
		return load_mesh_from_gltf_file(filename);
	}
	else{
		throw std::runtime_error("unsupported file format");
	}
}

std::vector<mesh_t> load_mesh_from_obj_memory(const std::string& str){
	auto model = _load_model_from_obj_memory( str, "", false, false );
	std::vector<mesh_t> meshes;
	for(const auto& mesh : model->meshes){
		meshes.emplace_back(static_cast<mesh_t>(mesh));
	}
	return meshes;
}

std::vector<mesh_t> load_mesh_from_obj_file(const std::string& filename){
	return load_mesh_from_obj_memory(file::read_txt_file(filename));
}

std::vector<mesh_t> load_mesh_from_gltf_file(const std::string& filename){
	return {};
}

std::vector<mesh_ext_t> load_mesh_ext_from_file(const std::string& filename){
	if(stdstr::ends_with(filename,".obj")){
		return load_mesh_ext_from_obj_file(filename);
	}
	else if(stdstr::ends_with(filename,".gltf")){
		return load_mesh_ext_from_gltf_file(filename);
	}
	else{
		throw std::runtime_error("unsupported file format");
	}
}

std::vector<mesh_ext_t> load_mesh_ext_from_obj_memory(const std::string& str){
	return _load_model_from_obj_memory( str, "", false, true )->meshes;
}

std::vector<mesh_ext_t> load_mesh_ext_from_obj_file(const std::string& filename){
	return load_mesh_ext_from_obj_memory(file::read_txt_file(filename));
}

std::vector<mesh_ext_t> load_mesh_ext_from_gltf_file(const std::string& filename){
	return {};
}

std::unique_ptr<model_t> load_model_from_file(const std::string& filename){
	if(stdstr::ends_with(filename,".obj")){
		return load_model_from_obj_file(filename);
	}
	else if(stdstr::ends_with(filename,".gltf")){
		return load_model_from_gltf_file(filename);
	}
	else{
		throw std::runtime_error("unsupported file format");
	}
}

std::unique_ptr<model_t> load_model_from_obj_memory(const std::string& str,const std::string& mtl){
	return _load_model_from_obj_memory( str, mtl, true, true );
}

std::unique_ptr<model_t> load_model_from_obj_file(const std::string& filename,const std::string& mtl){
	return load_model_from_obj_memory(file::read_txt_file(filename),file::read_txt_file(mtl));
}

std::unique_ptr<model_t> load_model_from_gltf_file(const std::string& filename){
	return {};
}


}