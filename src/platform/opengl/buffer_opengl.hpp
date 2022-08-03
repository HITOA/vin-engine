#pragma once

#include "renderer/buffer.hpp"

namespace Vin {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(size_t size);
		~OpenGLVertexBuffer();

		void Bind();
		void Unbind();

		void SetData(void* data, size_t size, size_t offset);

	private:
		unsigned int m_BufferId;
	};
}
