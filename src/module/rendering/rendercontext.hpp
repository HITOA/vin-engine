#pragma once

#include "vinpch.hpp"

#include "renderqueue.hpp"
#include "scene/camera.hpp"
#include "scene/light.hpp"

#define VIN_RENDERCONTEXT_BASEPATH "//Core/RenderContext"
#define MAX_LIGHT_COUNT 16

namespace Vin {
	struct RenderContext {
		Light m_MainLight{}; //Main Directional Light
		Light m_Lights[MAX_LIGHT_COUNT]{}; //Additional Light
		RenderQueue m_Queue{}; //RenderQueue
	};
}
