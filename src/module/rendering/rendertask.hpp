#pragma once

#include "renderer/vertexarray.hpp"
#include "scene/material.hpp"

namespace Vin {

	struct RenderTask {
		std::shared_ptr<VertexArray> vao{};
		std::shared_ptr<Material> mat{};
	};

}
