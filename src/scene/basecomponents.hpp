#pragma once

#include "math/math.hpp"

namespace Vin {

	template<typename T> //Float or Double
	struct Transform {
		Vector3<T> position{};
		Vector3<T> scale{};
		//Quaternion<T> rotation{};
	};

	struct MeshRenderer {

	};

	struct SpriteRenderer {

	};

}
