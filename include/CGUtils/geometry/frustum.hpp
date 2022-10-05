#pragma once

#include "CGUtils/math/vec3.hpp"

namespace wzz::geometry{
	using namespace wzz::math;

	template<typename T>
	struct plane_t{
		tvec3<T> normal;
		T D;
	};
	using planef = plane_t<float>;
	using planed = plane_t<double>;

	enum PLANE_INDEX : uint32_t{
		LEFT_PLANE_INDEX   = 0,
		RIGHT_PLANE_INDEX  = 1,
		BOTTOM_PLANE_INDEX = 2,
		TOP_PLANE_INDEX    = 3,
		NEAR_PLANE_INDEX   = 4,
		FAR_PLANE_INDEX    = 5,
		NUM_PLANES         = 6
	};

	template <typename T>
	struct frustum_t{
		plane_t<T> left_plane;
		plane_t<T> right_plane;
		plane_t<T> bottom_plane;
		plane_t<T> top_plane;
		plane_t<T> near_plane;
		plane_t<T> far_plane;

		const plane_t<T>& get_plane(PLANE_INDEX index) const{
			auto planes = reinterpret_cast<const plane_t<T>*>(this);
			return planes[static_cast<size_t>(index)];
		}

		plane_t<T>& get_plane(PLANE_INDEX index) {
			auto planes = reinterpret_cast<plane_t<T>*>(this);
			return planes[static_cast<size_t>(index)];
		}
	};
	using frustumf = frustum_t<float>;
	using frustumd = frustum_t<double>;

	template <typename T>
	struct frustum_ext_t : public frustum_t<T>{
		tvec3<T> frustum_corners[8];
	};

	using frustum_extf = frustum_ext_t<float>;
	using frustum_extd = frustum_ext_t<double>;

}