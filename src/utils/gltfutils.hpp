#pragma once

#include "vinpch.hpp"
#include "scene/mesh.hpp"

namespace Vin {
	eastl::shared_ptr<StaticMesh> LoadGLB(const char* path);
}
