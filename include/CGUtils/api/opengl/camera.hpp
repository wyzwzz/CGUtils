#pragma once
#include "common.hpp"
namespace wzz::gl{


class Camera
{
public:

	struct UpdateParams
	{
		bool front = false;
		bool left  = false;
		bool right = false;
		bool back  = false;

		bool up   = false;
		bool down = false;

		float cursor_rel_x = 0;
		float cursor_rel_y = 0;
	};

	Camera() noexcept;

	void set_position(const vec3f &position) noexcept;

	void set_direction(float horiRad, float vertRad) noexcept;

	void set_move_speed(float speed) noexcept;

	void set_view_rotation_speed(float speed) noexcept;

	void update(const UpdateParams &params) noexcept;

	void set_perspective(float fovDeg, float nearZ, float farZ) noexcept;

	void set_w_over_h(float wOverH) noexcept;

	void recalculate_matrics() noexcept;

	float get_near_z() const noexcept;

	float get_far_z() const noexcept;

	const vec3f &get_position() const noexcept;

	vec2f get_direction() const noexcept;

	const mat4 &get_view() const noexcept;

	const mat4 &get_proj() const noexcept;

	const mat4 &get_view_proj() const noexcept;

private:

	vec3f compute_direction() const;

	vec3f pos;
	float vert_rad;
	float hori_rad;

	float fov_deg;
	float near_z;
	float far_z;

	float w_over_h;

	float move_speed;
	float cursor_speed;

	mat4 view;
	mat4 proj;
	mat4 view_proj;
};
}