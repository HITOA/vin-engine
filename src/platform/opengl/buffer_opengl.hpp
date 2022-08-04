#pragma once

#include "renderer/buffer.hpp"

namespace Vin {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(size_t size);
		~OpenGLVertexBuffer();

		void Bind() const;
		void Unbind() const;

		void SetBufferLayout(const BufferLayout& layout);
		const BufferLayout& GetBufferLayout() const;

		void SetData(void* data, size_t size, size_t offset);

	private:
		unsigned int m_BufferId;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(BufferIndexType m_Type);
		~OpenGLIndexBuffer();

		void Bind() const;
		void Unbind() const;

		size_t GetCount() const;
		BufferIndexType GetIndexType() const;

		void SetData(void* data, size_t count);

	private:
		unsigned int m_BufferId;
		size_t m_Count;
		BufferIndexType m_Type;
	};
}
