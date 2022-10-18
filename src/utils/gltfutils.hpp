#pragma once

#include "vinpch.hpp"
#include "scene/scene.hpp"

namespace Vin {
	std::shared_ptr<Scene<ArchetypeMemoryLayout::Contiguous>> LoadGLTF(std::string_view path);
}
