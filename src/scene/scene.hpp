#pragma once

#include "ecs/registry.hpp"
#include "material.hpp"
#include "mesh.hpp"

namespace Vin{
	template<ArchetypeMemoryLayout layout>
	class Scene {
	public:
		void Render() {

		}

		Registry& operator->() {
			return m_Registry;
		}
	private:
		Registry<layout> m_Registry{};
	};
}