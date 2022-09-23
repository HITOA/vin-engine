#pragma once

#include "vinpch.hpp"
#include <assert.hpp>

namespace Vin {
	enum class BufferIndexType {
		None = 0,
		UnsignedInt16,
		UnsignedInt32
	};

	enum class VertexAttributeType {
		None,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4
	};

	enum class VertexAttribute {
		Position = 0,
		Normal,
		Tangent,
		Color,
		TextureCoord0,
		TextureCoord1,
		TextureCoord2,
		TextureCoord3,
		TextureCoord4,
		TextureCoord5,
		TextureCoord6,
		TextureCoord7,
		BlendWeight,
		BlendIndices
	};

	static size_t GetVertexAttributeTypeSize(VertexAttributeType type) {
		switch (type) {
		case VertexAttributeType::Float: return sizeof(float);
		case VertexAttributeType::Float2: return sizeof(float) * 2;
		case VertexAttributeType::Float3: return sizeof(float) * 3;
		case VertexAttributeType::Float4: return sizeof(float) * 4;

		case VertexAttributeType::Int: return sizeof(int);
		case VertexAttributeType::Int2: return sizeof(int) * 2;
		case VertexAttributeType::Int3: return sizeof(int) * 3;
		case VertexAttributeType::Int4: return sizeof(int) * 4;
		}

		return 0;
	}

	static size_t GetBufferIndexTypeSize(BufferIndexType type) {
		switch (type) {
		case BufferIndexType::UnsignedInt16: return sizeof(short);
		case BufferIndexType::UnsignedInt32: return sizeof(int);
		}

		return 0;
	}

	struct VertexBufferElement {
		VertexAttribute attribute;
		VertexAttributeType type;
		size_t offset;
		bool normalized;

		VertexBufferElement() = default;
		VertexBufferElement(VertexAttribute attribute, VertexAttributeType type, bool normalized = false) : 
			attribute{ attribute }, type { type }, offset{ 0 }, normalized{ normalized } {};

		inline friend bool operator==(const VertexBufferElement& lhs, const VertexBufferElement& rhs) {
			return lhs.attribute == rhs.attribute && lhs.type == rhs.type && lhs.normalized == rhs.normalized;
		}
	};

	class VertexBufferLayout {
	public:
		VertexBufferLayout() = default;

		VertexBufferLayout(std::initializer_list<VertexBufferElement> elements) : m_Layout{ elements } {
			m_Stride = 0;

			for (auto& element : m_Layout) {
				element.offset = m_Stride;
				m_Stride += GetVertexAttributeTypeSize(element.type);
			}
		}

		void AddVertexBufferElement(VertexBufferElement& element) {
			m_Layout.push_back(element);
			m_Layout[0].offset += m_Stride;
			m_Stride += GetVertexAttributeTypeSize(element.type);
		}

		size_t GetStride() const { return m_Stride; }

		eastl::vector<VertexBufferElement>::iterator begin() { return m_Layout.begin(); }
		eastl::vector<VertexBufferElement>::iterator end() { return m_Layout.end(); }
		eastl::vector<VertexBufferElement>::const_iterator begin() const { return m_Layout.begin(); }
		eastl::vector<VertexBufferElement>::const_iterator end() const { return m_Layout.end(); }
		eastl::vector<VertexBufferElement>::const_iterator cbegin() const { return m_Layout.cbegin(); }
		eastl::vector<VertexBufferElement>::const_iterator cend() const { return m_Layout.cend(); }

		inline friend bool operator==(const VertexBufferLayout& lhs, const VertexBufferLayout& rhs) {
			if (lhs.m_Stride != rhs.m_Stride || lhs.m_Layout.size() != rhs.m_Layout.size())
				return false;

			auto itend = lhs.cend();
			for (auto it1 = lhs.cbegin(), it2 = rhs.cbegin(); it1 != itend; ++it1, ++it2)
				if (it1 != it2)
					return false;
			return true;
		}

		inline friend bool operator!=(const VertexBufferLayout& lhs, const VertexBufferLayout& rhs) {
			return !(lhs == rhs);
		}

	private:
		eastl::vector<VertexBufferElement> m_Layout;
		size_t m_Stride = 0;
	};
	
	class Buffer {
	public:
		virtual ~Buffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

	class VertexBuffer : public Buffer{
	public:
		virtual ~VertexBuffer() {};

		virtual void SetBufferLayout(const VertexBufferLayout& layout) = 0;
		virtual const VertexBufferLayout& GetBufferLayout() const = 0;

		virtual void SetData(void* data, size_t size, size_t offset, bool resize = false) = 0;

		static eastl::shared_ptr<VertexBuffer> Create(size_t size);
	};

	class IndexBuffer : public Buffer{
	public:
		virtual ~IndexBuffer() {};

		virtual size_t GetCount() const = 0;
		virtual BufferIndexType GetIndexType() const = 0;

		virtual void SetData(void* data, size_t count) = 0;

		static eastl::shared_ptr<IndexBuffer> Create(BufferIndexType type);
	};
}
