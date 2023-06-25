#pragma once

#include "core/base.hpp"
#include "vinpch.hpp"
#include "renderer/buffer.hpp"
#include "renderer/vertexarray.hpp"
#include "material.hpp"

namespace Vin {

	/**
	* Data required to render one geometry.
	*/
	struct Primitive {
		std::vector<std::shared_ptr<VertexBuffer>> vbos{};
		std::shared_ptr<IndexBuffer> ibo{};
		std::shared_ptr<VertexArray> vao{};
		std::shared_ptr<Material> material{};
		bool indexed{ true }; //If the primitive is rendered using indices or not.
		uint32_t vertexCount{ 0 };
	};

	class StaticMesh {
	public:
		StaticMesh() = default;

		void AddPrimitive(Primitive primitive);

		std::vector<Primitive>::iterator begin() { return m_Primitives.begin(); }
		std::vector<Primitive>::iterator end() { return m_Primitives.end(); }
		std::vector<Primitive>::const_iterator begin() const { return m_Primitives.begin(); }
		std::vector<Primitive>::const_iterator end() const { return m_Primitives.end(); }
		std::vector<Primitive>::const_iterator cbegin() const { return m_Primitives.cbegin(); }
		std::vector<Primitive>::const_iterator cend() const { return m_Primitives.cend(); }
	private:
		std::vector<Primitive> m_Primitives{};
	};

	/** 
	* Implementing with dynamic VBO. Used for procedural mesh etc. 
	* can contain only one submesh.Multiple material will just render the same mesh with all the material.
	*/
	class DynamicMesh {
	public:
		DynamicMesh(const VertexBufferLayout& layout = VertexBufferLayout{ {
			VertexBufferElement{ VertexAttribute::Position, VertexAttributeType::Float3 },
			VertexBufferElement{ VertexAttribute::Normal, VertexAttributeType::Float3 },
			VertexBufferElement{ VertexAttribute::Color, VertexAttributeType::Float4 }
			} }, BufferIndexType type = BufferIndexType::UnsignedInt16);
		
		void SetVertexBufferLayout(const VertexBufferLayout& layout);
		void SetIndexBufferType(BufferIndexType type);

		void SetVertexData(void* data, uint32_t vertexCount, BufferUsageType usage = BufferUsageType::Dynamic);
		void SetIndexData(void* data, uint32_t indexCount, BufferUsageType usage = BufferUsageType::Dynamic);

		void SetMaterialCount(size_t count);
		size_t GetMaterialCount();
		void SetMaterial(std::shared_ptr<Material> material, size_t idx);
		std::shared_ptr<Material> GetMaterial(size_t idx);

		std::vector<Primitive>::iterator begin() { return m_Primitives.begin(); }
		std::vector<Primitive>::iterator end() { return m_Primitives.end(); }
		std::vector<Primitive>::const_iterator begin() const { return m_Primitives.begin(); }
		std::vector<Primitive>::const_iterator end() const { return m_Primitives.end(); }
		std::vector<Primitive>::const_iterator cbegin() const { return m_Primitives.cbegin(); }
		std::vector<Primitive>::const_iterator cend() const { return m_Primitives.cend(); }
	private:
		std::shared_ptr<VertexBuffer> m_VBO{};
		std::shared_ptr<IndexBuffer> m_IBO{};
		std::shared_ptr<VertexArray> m_VAO{};
		std::vector<Primitive> m_Primitives{};
		uint32_t vertexCount{ 0 };
	}; 
}
