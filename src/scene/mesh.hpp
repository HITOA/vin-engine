#pragma once

#include <vinbase.hpp>
#include "vinpch.hpp"
#include "renderer/buffer.hpp"
#include "renderer/vertexarray.hpp"
#include "material.hpp"

namespace Vin {

	/**
	* Data required to render one geometry.
	*/
	struct Primitive {
		eastl::vector<eastl::shared_ptr<VertexBuffer>> vbos{};
		eastl::shared_ptr<IndexBuffer> ibo{};
		eastl::shared_ptr<VertexArray> vao{};
		bool indexed{ true }; //If the primitive is rendered using indices or not.
		//int m_Material{ -1 }; //Material used to render this primitive, index relative to the scene it belong
		Material* material{};
	};

	/**
	 * Implementing with static VBO. Used for mesh that don't change at runtime.
	 * Can contain any number of submesh and material. 
	 * submesh 0 will be rendered with material 0, submesh 1 with material 1, etc. with modulo for the index.
	 * Each submesh/material is one drawcall.
	 */
	class StaticMesh {
	public:
		StaticMesh() = default;

		void AddPrimitive(Primitive primitive);

		eastl::vector<Primitive>::iterator begin() { return m_Primitives.begin(); }
		eastl::vector<Primitive>::iterator end() { return m_Primitives.end(); }
		eastl::vector<Primitive>::const_iterator begin() const { return m_Primitives.begin(); }
		eastl::vector<Primitive>::const_iterator end() const { return m_Primitives.end(); }
		eastl::vector<Primitive>::const_iterator cbegin() const { return m_Primitives.cbegin(); }
		eastl::vector<Primitive>::const_iterator cend() const { return m_Primitives.cend(); }
	private:
		eastl::vector<Primitive> m_Primitives{};
	};

	/** 
	* Implementing with dynamic VBO. Used for procedural mesh etc. 
	* can contain only one submesh.Multiple material will just render the same mesh with all the material.
	*/
	class DynamicMesh {}; 
}
