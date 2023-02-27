#pragma once
#include "aabb3.hpp"
#include "frustum.hpp"
#include "CGUtils/math/mat4_c.hpp"
namespace wzz::geometry{
	using namespace wzz::math;

	enum class BoxVisibility : uint32_t{
		Invisible = 0,
		Intersecting = 0b1,
		FullyVisible = 0b11
	};

	// extract view frustum from projection view matrix(col-major)
	template <typename T>
	inline void extract_frustum_from_matrix(const tmat4_c<T>& matrix, frustum_t<T>& frustum, bool is_OpenGL){
		// For more details, see Gribb G., Hartmann K., "Fast Extraction of Viewing Frustum Planes from the
		// World-View-Projection Matrix" (the paper is available at
		// http://gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf)

		// Left clipping plane
		frustum.left_plane.normal.x = matrix[0][3] + matrix[0][0];
		frustum.left_plane.normal.y = matrix[1][3] + matrix[1][0];
		frustum.left_plane.normal.z = matrix[2][3] + matrix[2][0];
		frustum.left_plane.D = matrix[3][3] + matrix[3][0];

		// Right clipping plane
		frustum.right_plane.normal.x = matrix[0][3] - matrix[0][0];
		frustum.right_plane.normal.y = matrix[1][3] - matrix[1][0];
		frustum.right_plane.normal.z = matrix[2][3] - matrix[2][0];
		frustum.right_plane.D = matrix[3][3] - matrix[3][0];

		// Top clipping plane
		frustum.top_plane.normal.x = matrix[0][3] - matrix[0][1];
		frustum.top_plane.normal.y = matrix[1][3] - matrix[1][1];
		frustum.top_plane.normal.z = matrix[2][3] - matrix[2][1];
		frustum.top_plane.D = matrix[3][3] - matrix[3][1];

		// Bottom clipping plane
		frustum.bottom_plane.normal.x = matrix[0][3] + matrix[0][1];
		frustum.bottom_plane.normal.y = matrix[1][3] + matrix[1][1];
		frustum.bottom_plane.normal.z = matrix[2][3] + matrix[2][1];
		frustum.bottom_plane.D = matrix[3][3] + matrix[3][1];

		// Near clipping plane
		if (is_OpenGL)
		{
			// -w <= z <= w
			frustum.near_plane.normal.x = matrix[0][3] + matrix[0][2];
			frustum.near_plane.normal.y = matrix[1][3] + matrix[1][2];
			frustum.near_plane.normal.z = matrix[2][3] + matrix[2][2];
			frustum.near_plane.D = matrix[3][3] + matrix[3][2];
		}
		else
		{
			// 0 <= z <= w
			frustum.near_plane.normal.x = matrix[0][2];
			frustum.near_plane.normal.y = matrix[1][2];
			frustum.near_plane.normal.z = matrix[2][2];
			frustum.near_plane.D = matrix[3][2];
		}

		// Far clipping plane
		frustum.far_plane.normal.x = matrix[0][3] - matrix[0][2];
		frustum.far_plane.normal.y = matrix[1][3] - matrix[1][2];
		frustum.far_plane.normal.z = matrix[2][3] - matrix[2][2];
		frustum.far_plane.D = matrix[3][3] - matrix[3][2];
	}

	template <typename T>
	inline void extract_frustum_from_matrix(const tmat4_c<T>& matrix, frustum_ext_t<T>& frustum_ext, bool is_OpenGL){
		extract_frustum_from_matrix(matrix, static_cast<frustum_t<T>&>(frustum_ext), is_OpenGL);

		auto inv_matrix = matrix.inverse();
		T near_clip_z = is_OpenGL ? T(-1) : T(0);
		tvec3<T> proj_space_corners[] = {
			tvec3<T>(-1, -1, near_clip_z),
			tvec3<T>(1, -1, near_clip_z),
			tvec3<T>(-1, 1, near_clip_z),
			tvec3<T>(1, 1, near_clip_z),

			tvec3<T>(-1, -1, 1),
			tvec3<T>(1, -1, 1),
			tvec3<T>(-1, 1, 1),
			tvec3<T>(1, 1, 1)
		};
		for(int i = 0; i < 8; i++){
			auto t = inv_matrix * tvec4(proj_space_corners[i].x,
							   proj_space_corners[i].y,
							   proj_space_corners[i].z, T(1));
			frustum_ext.frustum_corners[i] = t.homogenize();
		}
	}

	template <typename T>
	inline BoxVisibility get_box_visibility_against_plane(const plane_t<T>& plane, const taabb3<T>& box){
		const auto& normal = plane.normal;

		tvec3<T> max_point(
		  normal.x > 0 ? box.high.x : box.low.x,
		  normal.y > 0 ? box.high.y : box.low.y,
		  normal.z > 0 ? box.high.z : box.low.z);
		T d_max = dot(max_point, normal) + plane.D;
		if(d_max < 0) return BoxVisibility::Invisible;

		tvec3<T> min_point(
		  normal.x > 0 ? box.low.x : box.high.x,
		  normal.y > 0 ? box.low.y : box.high.y,
		  normal.z > 0 ? box.low.z : box.high.z);
		T d_min = dot(min_point, normal) + plane.D;
		if(d_min > 0) return BoxVisibility::FullyVisible;

		return BoxVisibility::Intersecting;
	}

	template <typename T>
	inline BoxVisibility get_box_visibility(const frustum_t<T>& frustum, const taabb3<T>& box){
		uint32_t num_planes_inside = 0;
		for(uint32_t plane_idx = 0; plane_idx < NUM_PLANES; plane_idx++){
			const auto& cur_plane = frustum.get_plane(static_cast<PLANE_INDEX>(plane_idx));
			auto visibility_against_plane = get_box_visibility_against_plane(cur_plane, box);
			if(visibility_against_plane == BoxVisibility::Invisible)
				return BoxVisibility::Invisible;
			if(visibility_against_plane == BoxVisibility::FullyVisible)
				num_planes_inside++;
		}
		return num_planes_inside == NUM_PLANES ? BoxVisibility::FullyVisible : BoxVisibility::Intersecting;
	}

	template <typename T>
	inline BoxVisibility get_box_visibility(const frustum_ext_t<T>& frustum_ext, const taabb3<T>& box){
		auto visibility = get_box_visibility(static_cast<const frustum_t<T>&>(frustum_ext), box);
		if(visibility == BoxVisibility::FullyVisible || visibility == BoxVisibility::Invisible)
			return visibility;

		// Additionally test if the whole frustum is outside one of
		// the the bounding box planes. This helps in the following situation:
		//
		//
		//       .
		//      /   '  .       .
		//     / AABB  /   . ' |
		//    /       /. '     |
		//       ' . / |       |
		//       * .   |       |
		//           ' .       |
		//               ' .   |
		//                   ' .

		// Test all frustum corners against every bound box plane
		for(int i_box_plane = 0; i_box_plane < 6; i_box_plane++){
			T cur_plane_coord = reinterpret_cast<const T*>(&box)[i_box_plane];
			int i_coord_order = i_box_plane % 3;
			T plane_sign = (i_box_plane >= 3) ? T(1) : T(-1);
			bool all_corners_outside = true;
			for(int i_corner = 0; i_corner < 8; i_corner++){
				T cur_corner_coord = frustum_ext.frustum_corners[i_corner][i_coord_order];
				if(plane_sign * (cur_plane_coord - cur_corner_coord) > 0){
					all_corners_outside = false;
					break;
				}
			}
			if(all_corners_outside)
				return BoxVisibility::Invisible;
		}
		return BoxVisibility::Intersecting;
	}

}