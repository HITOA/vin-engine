#pragma once

#include "ecs/registry.hpp"
#include "material.hpp"
#include "mesh.hpp"

namespace Vin{
	template<ArchetypeMemoryLayout layout>
	class Scene {
	private:
		Registry<layout> m_Registry{};

		eastl::vector<Material> m_Materials{};
		eastl::vector<Primitive> m_Primitives{};
	};
}