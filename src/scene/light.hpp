#pragma once

#include "math/math.hpp"

namespace Vin {

	enum class LightType {
		None,
		Directional,
		Point,
		Spot,
		AreaRec,
		AreaDisc
	};

	enum class ShadowType {
		None,
		Hard,
		Soft
	};

	struct ShadowSettings {
		ShadowType type{};
		float strength{};
		Vector2<float> resolution{};
		float bias{};
		float normalbias{};
		float nearplane{};
	};

	struct Light {
		LightType type{ LightType::None }; //Light type (Directional, Point, Spot, AreaRec, AreaDisc)

		Vector3<float> position{}; //Usefull for point, spot and area
		Vector3<float> rotations{}; //Usefull for directional and spot
		Vector3<float> scale{}; //Usefull for area

		Color color{}; //Color of the light

		float range{}; //Point and Spot only
		//float spotangle{};
		float intensity{}; //Itensity of the light

		ShadowSettings shadow{};
	};

}
