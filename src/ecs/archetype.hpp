#pragma once

#include <vinbase.hpp>

#include "vinpch.hpp"

#include "component.hpp"

namespace Vin {
	typedef usize ArchetypeId;
	
	struct Archetype {
		eastl::vector<ComponentArray> components;
	};

}
