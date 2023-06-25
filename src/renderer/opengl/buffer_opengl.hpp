#pragma once

#include "renderer/buffer.hpp"

namespace Vin {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(size_t size);
		~OpenGLVertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetBufferLayout(const VertexBufferLayout& layout);
		const VertexBufferLayout& GetBufferLayout() const;

		void SetData(void* data, size_t size, size_t offset, bool resize = false, BufferUsageType usage = BufferUsageType::Static);

	private:
		unsigned int m_BufferId;
		VertexBufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(BufferIndexType m_Type);
		~OpenGLIndexBuffer();

		void Bind() const;
		void Unbind() const;

		uint32_t GetCount() const;
		BufferIndexType GetIndexType() const;

		void SetData(void* data, uint32_t count, BufferUsageType usage = BufferUsageType::Static);

	private:
		unsigned int m_BufferId;
		uint32_t m_Count;
		BufferIndexType m_Type;
	};
}
