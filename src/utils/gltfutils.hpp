#pragma once

#include "vinpch.hpp"
#include "scene/mesh.hpp"

namespace Vin {
	std::shared_ptr<StaticMesh> LoadGLB(const char* path);
}
