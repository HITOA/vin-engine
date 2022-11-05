#pragma once

#include "scene/camera.hpp"
#include "ecs/registry.hpp"
#include "scene/mesh.hpp"

namespace Vin {

	/**
	 * Base class for creating render context.
	 */

	class RenderContext {
	public:
		virtual ~RenderContext() {};

		virtual void RenderScene(Registry& registry, std::shared_ptr<Camera> camera) = 0;
	};

}
