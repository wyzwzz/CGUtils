//
// Created by wyz on 2022/2/16.
//
#pragma once
#include "../math.hpp"

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
    };

    /**
     * @brief just load all triangle objects from file without other information
     */
    std::vector<triangle_t> load_triangle_from_file(const std::string& filename);

    std::vector<triangle_t> load_triangle_from_obj_file(const std::string& filename);

    std::vector<triangle_t> load_triangle_from_gltf_file(const std::string& filename);

    /**
     * @brief just load meshes from file without material information
     */
    std::vector<mesh_t> load_mesh_from_file(const std::string& filename);

    std::vector<mesh_t> load_mesh_from_obj_file(const std::string& filename);

    std::vector<mesh_t> load_mesh_from_gltf_file(const std::string& filename);

    class material_t;

    struct model_t{
        std::string name;
        std::shared_ptr<mesh_t> meshes;
        std::shared_ptr<material_t> materials;
    };

    /**
     * @brief load meshes and materials from file
     */
    std::unique_ptr<model_t> load_model_from_file(const std::string& filename);

    std::unique_ptr<model_t> load_model_from_obj_file(const std::string& filename);

    std::unique_ptr<model_t> load_model_from_gltf_file(const std::string& filename);
}
