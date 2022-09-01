#pragma once

#include <vinbase.hpp>
#include "vinpch.hpp"
#include "renderer/buffer.hpp"
#include "renderer/vertexarray.hpp"
#include "material.hpp"

namespace Vin {

	struct SubMeshData {
		eastl::shared_ptr<VertexBuffer> vbo{};
		eastl::shared_ptr<IndexBuffer> ibo{};
		eastl::shared_ptr<VertexArray> vao{};
	};

	/**
	 * Implementing with static VBO. Used for mesh that don't change at runtime.
	 * Can contain any number of submesh and material. 
	 * submesh 0 will be rendered with material 0, submesh 1 with material 1, etc. with modulo for the index.
	 * Each submesh/material is one drawcall.
	 */
	class StaticMesh {
	public:
		StaticMesh() = delete;
		StaticMesh(VertexBufferLayout layout);

	public:
		void AddSubMeshData(SubMeshData&& submesh);
		void AddSubMesh(void* vertices, usize vertexCount, void* indices, usize indexCount);

		void AddMaterial(eastl::shared_ptr<Material> material);

		VertexBufferLayout GetVertexLayout() const;
		bool HasVertexAttribute(VertexAttribute attribute) const;
		VertexAttributeType GetVertexAttributeType(VertexAttribute attribute) const;
	private:
		VertexBufferLayout m_VertexLayout{};

		eastl::fixed_vector<eastl::shared_ptr<Material>, 8, true> m_Materials{};
		eastl::fixed_vector<SubMeshData, 8, true> m_SubMeshes{};
	};

	/** 
	* Implementing with dynamic VBO. Used for procedural mesh etc. 
	* can contain only one submesh.Multiple material will just render the same mesh with all the material.
	*/
	class DynamicMesh {}; 
}
