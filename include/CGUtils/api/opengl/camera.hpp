#pragma once
#include "common.hpp"
namespace wzz::gl{


class fps_camera_t
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

	fps_camera_t() noexcept;

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

	/**
	 * @return {hori_radians,vert_radians}
	 */
	vec2f get_direction() const noexcept;

	/**
	 * @return normalized direction in xyz-coord
	 */
	vec3f get_xyz_direction() const noexcept;

	const mat4 &get_view() const noexcept;

	const mat4 &get_proj() const noexcept;

	const mat4 &get_view_proj() const noexcept;

private:


	vec3f pos;
	float vert_rad = 0.f;
	float hori_rad = 0.f;

	float fov_deg = 60.f;
	float near_z = 0.1f;
	float far_z = 100.f;

	float w_over_h = 1;

	float move_speed = 0.02f;
	float cursor_speed = 0.003f;

	mat4 view;
	mat4 proj;
	mat4 view_proj;
};
}