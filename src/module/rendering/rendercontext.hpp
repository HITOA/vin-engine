#pragma once

#include "vinpch.hpp"

#include "renderqueue.hpp"
#include "scene/camera.hpp"
#include "scene/light.hpp"
#include "renderer/framebuffer.hpp"

#define VIN_RENDERCONTEXT_BASEPATH "//Core/RenderContext"
#define MAX_LIGHT_COUNT 16

namespace Vin {
	struct RenderContext {
		Light mainLight{}; //Main Directional Light
		Light lights[MAX_LIGHT_COUNT]{}; //Additional Light
		RenderQueue queue{}; //RenderQueue
	};
}
