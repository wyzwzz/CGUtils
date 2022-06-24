//
// Created by wyz on 2022/2/16.
//
#pragma once
#include "../math.hpp"
#include "../texture.hpp"
namespace wzz::model{

    struct vertex_t{
        math::vec3f pos;
        math::vec3f normal;
        math::vec2f tex_coord;
    };

    struct triangle_t{
        vertex_t vertices[3];
    };

    struct mesh_t{
        std::string name;
        std::vector<vertex_t> vertices;
        std::vector<uint32_t> indices;
    	int material = -1;
	};

	struct mesh_ext_t: mesh_t{
		//should have same count
		std::vector<math::vec3f> pos;
		std::vector<math::vec3f> normal;
		std::vector<math::vec2f> tex_coord;
	};

    /**
     * @brief just load all triangle objects from file without other information
     */
    std::vector<triangle_t> load_triangle_from_file(const std::string& filename);

	std::vector<triangle_t> load_triangle_from_obj_memory(const std::string& str);

    std::vector<triangle_t> load_triangle_from_obj_file(const std::string& filename);

    std::vector<triangle_t> load_triangle_from_gltf_file(const std::string& filename);

    /**
     * @brief just load meshes from file without material information
     */
    std::vector<mesh_t> load_mesh_from_file(const std::string& filename);

	std::vector<mesh_t> load_mesh_from_obj_memory(const std::string& str);

    std::vector<mesh_t> load_mesh_from_obj_file(const std::string& filename);

    std::vector<mesh_t> load_mesh_from_gltf_file(const std::string& filename);

	std::vector<mesh_ext_t> load_mesh_ext_from_file(const std::string& filename);

	std::vector<mesh_ext_t> load_mesh_ext_from_obj_memory(const std::string& str);

	std::vector<mesh_ext_t> load_mesh_ext_from_obj_file(const std::string& filename);

	std::vector<mesh_ext_t> load_mesh_ext_from_gltf_file(const std::string& filename);


	struct material_t{
		std::string name;

		//todo correct to linear space ?
		texture::image2d_t<math::color3b> albedo;

		texture::image2d_t<math::color3b> normal;

		texture::image2d_t<math::color2b> metallic_roughness;

		texture::image2d_t<math::color3b> emissions;

		texture::image2d_t<math::byte> ao;

		texture::image2d_t<math::color3b> specular;

		float shininess;

	};

    struct model_t{
		using mesh_t = mesh_ext_t;
        std::string name;
        std::vector<mesh_t> meshes;
        std::vector<material_t> materials;
    };

    /**
     * @brief load meshes and materials from file
     */
    std::unique_ptr<model_t> load_model_from_file(const std::string& filename);

	std::unique_ptr<model_t> load_model_from_obj_memory(const std::string& str,const std::string& mtl = "");

    std::unique_ptr<model_t> load_model_from_obj_file(const std::string& filename,const std::string& mtl = "");

    std::unique_ptr<model_t> load_model_from_gltf_file(const std::string& filename);
}
