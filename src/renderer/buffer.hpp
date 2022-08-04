#pragma once

#include "vinpch.hpp"
#include "core/assert.hpp"

namespace Vin {

	enum class BufferElementType {
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4
	};

	static size_t GetBufferElementTypeSize(BufferElementType type) {
		switch (type) {
		case BufferElementType::Float: return sizeof(float);
		case BufferElementType::Float2: return sizeof(float) * 2;
		case BufferElementType::Float3: return sizeof(float) * 3;
		case BufferElementType::Float4: return sizeof(float) * 4;

		case BufferElementType::Int: return sizeof(int);
		case BufferElementType::Int2: return sizeof(int) * 2;
		case BufferElementType::Int3: return sizeof(int) * 3;
		case BufferElementType::Int4: return sizeof(int) * 4;
		}

		return 0;
	}

	struct BufferElement {
		BufferElementType type;
		size_t offset;
		bool normalized;

		BufferElement() = default;
		BufferElement(BufferElementType type, bool normalized = false) : 
			type{ type }, offset{ 0 }, normalized{ normalized } {};
	};

	class BufferLayout {
	public:
		BufferLayout() = default;

		BufferLayout(std::initializer_list<BufferElement> elements) : m_Layout{ elements } {
			m_Stride = 0;

			for (auto& element : m_Layout) {
				element.offset = m_Stride;
				m_Stride += GetBufferElementTypeSize(element.type);
			}
		}

		size_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Layout.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Layout.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Layout.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Layout.end(); }
		std::vector<BufferElement>::const_iterator cbegin() const { return m_Layout.cbegin(); }
		std::vector<BufferElement>::const_iterator cend() const { return m_Layout.cend(); }

	private:
		std::vector<BufferElement> m_Layout;
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

		virtual void SetBufferLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetBufferLayout() const = 0;

		virtual void SetData(void* data, size_t size, size_t offset) = 0;

		static std::unique_ptr<VertexBuffer> Create(size_t size);
	};

	class IndexBuffer : public Buffer{
	public:
		virtual ~IndexBuffer() {};
	};
}
