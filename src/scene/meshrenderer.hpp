#pragma once

#include "mesh.hpp"

namespace Vin {

	struct MeshRenderer {
		bool isDynamicMesh{ false };
		union {
			StaticMesh* staticmesh;
			DynamicMesh* dynamicmesh;
		};

		MeshRenderer(StaticMesh* mesh) : isDynamicMesh{ false }, staticmesh{ mesh } {};
		MeshRenderer(DynamicMesh* mesh) : isDynamicMesh{ true }, dynamicmesh{ mesh } {};
	};

}
