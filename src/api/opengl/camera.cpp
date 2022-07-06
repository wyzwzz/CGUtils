//
// Created by wyz on 2022/7/6.
//
#include <CGUtils/api/opengl/camera.hpp>

namespace wzz::gl{

fps_camera_t::fps_camera_t() noexcept
{

}
void fps_camera_t::set_position( const vec3f &position ) noexcept
{
	this->pos = position;
}
void fps_camera_t::set_direction( float horiRad, float vertRad ) noexcept
{
	this->hori_rad = horiRad;
	this->vert_rad = vertRad;
}
void fps_camera_t::set_move_speed( float speed ) noexcept
{
	this->move_speed = speed;
}
void fps_camera_t::set_view_rotation_speed( float speed ) noexcept
{
	this->cursor_speed = speed;
}
void fps_camera_t::update( const fps_camera_t::UpdateParams &params ) noexcept
{
	vert_rad -= cursor_speed * params.cursor_rel_y;
	hori_rad += cursor_speed * params.cursor_rel_x;

	const float PI = math::PI_f;
	while(hori_rad < 0) hori_rad += 2 * PI;
	while(hori_rad >= 2 * PI) hori_rad -= 2 * PI;
	vert_rad = std::clamp(vert_rad,-PI * 0.5f + 0.01f,PI * 0.5f + 0.01f);

	const auto dir = get_xyz_direction();
	const auto front = vec3(dir.x,0,dir.z).normalized();
	const auto right = cross(dir,{0,1,0}).normalized();

	const int front_step = params.front - params.back;
	const int right_step = params.right - params.left;

	pos += move_speed * ((float)front_step * front + (float)right_step * right);


	if(params.up)
		pos.y += move_speed;
	if(params.down)
		pos.y -= move_speed;

}
void fps_camera_t::set_perspective( float fovDeg, float nearZ, float farZ ) noexcept
{
	this->fov_deg = fovDeg;
	this->near_z = nearZ;
	this->far_z = farZ;
}
void fps_camera_t::set_w_over_h( float wOverH ) noexcept
{
	this->w_over_h = wOverH;

}
void fps_camera_t::recalculate_matrics() noexcept
{
	const auto dir = get_xyz_direction();
	view = transform::look_at(pos,pos + dir,{0,1,0});
	proj = transform::perspective(math::deg2rad(fov_deg),w_over_h,near_z,far_z);
	view_proj = proj * view;
}
float fps_camera_t::get_near_z() const noexcept
{
	return near_z;
}
float fps_camera_t::get_far_z() const noexcept
{
	return far_z;
}
const vec3f &fps_camera_t::get_position() const noexcept
{
	return pos;
}
vec2f fps_camera_t::get_direction() const noexcept
{
	return {hori_rad,vert_rad};
}
vec3f fps_camera_t::get_xyz_direction() const noexcept
{
	float y = std::sin(vert_rad);
	float x = std::cos(vert_rad) * std::cos(hori_rad);
	float z = std::cos(vert_rad) * std::sin(hori_rad);
	return {x,y,z};
}
const mat4 &fps_camera_t::get_view() const noexcept
{
	return view;
}
const mat4 &fps_camera_t::get_proj() const noexcept
{
	return proj;
}
const mat4 &fps_camera_t::get_view_proj() const noexcept
{
	return view_proj;
}


}

