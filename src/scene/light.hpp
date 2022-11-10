#pragma once

#include "math/math.hpp"
#include "renderer/renderer.hpp"

namespace Vin {

	enum class LightType {
		None,
		Directional,
		Point,
		Spot
	};

	struct ShadowSettings {
		float strength{ 0.0f };
		float distance{ 20.0f };
		Vector2<int> resolution{ 1024, 1024 };
		float bias{ 0.0001f };
		CullMode cullMode{ CullMode::Front };
	};

	struct Light {
		LightType type{ LightType::Directional }; //Light type (Directional, Point, Spot, AreaRec, AreaDisc)
		bool mainLight{ false };

		Vector3<float> position{ 0.0f, 0.0f, 0.0f }; //Usefull for point, spot and area
		Vector3<float> direction{ 0.0f, 0.0f, 0.0f }; //Usefull for directional and spot

		Vector3<float> color{ 1.0f, 1.0f, 1.0f }; //Color of the light

		float range{ 100.0f }; //Point
		Vector2<float> spotangle{ 25, 10 }; //Inner angle & Outer angle
		float intensity{ 1.0f }; //Itensity of the light

		ShadowSettings shadow{};
	};

}
