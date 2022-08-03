#pragma once

#include "vinpch.hpp"

namespace Vin {
	class Buffer {
	public:
		virtual ~Buffer() {};
	};

	class VertexBuffer : public Buffer{
	public:
		virtual ~VertexBuffer() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetData(void* data, size_t size, size_t offset) = 0;

		static std::unique_ptr<VertexBuffer> Create(size_t size);
	};

	class IndexBuffer : public Buffer{

	};
}
