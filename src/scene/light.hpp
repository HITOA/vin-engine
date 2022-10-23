#pragma once

#include "math/math.hpp"
#include "renderer/renderer.hpp"

namespace Vin {

	enum class LightType {
		None,
		Directional,
		Point,
		Spot,
		AreaRec,
		AreaDisc
	};

	struct ShadowSettings {
		float strength{};
		float distance{ 20.0f };
		Vector2<int> resolution{ 1024, 1024 };
		float bias{ 0.0001f };
		CullMode cullMode{ CullMode::Front };
	};

	struct Light {
		LightType type{ LightType::Directional }; //Light type (Directional, Point, Spot, AreaRec, AreaDisc)

		Vector3<float> position{}; //Usefull for point, spot and area
		Vector3<float> direction{ 0.0f, 1.0f, 0.0f }; //Usefull for directional and spot
		Vector3<float> scale{ 1.0f, 1.0f, 1.0f }; //Usefull for area

		Vector4<float> color{ 1.0f, 1.0f, 1.0f, 1.0f }; //Color of the light

		float range{ 1.0f }; //Point and Spot only
		//float spotangle{};
		float intensity{ 10.0f }; //Itensity of the light

		ShadowSettings shadow{};
	};

}
