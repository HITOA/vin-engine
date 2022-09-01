#pragma once

#include "vinpch.hpp"

#include "renderer/buffer.hpp"

namespace Vin {
	class VertexArray {
	public:
		virtual ~VertexArray() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const eastl::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const eastl::shared_ptr<IndexBuffer>& indexBuffer) = 0;
		virtual void IgnoreAttribute(usize n) = 0;

		virtual const eastl::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

		static eastl::shared_ptr<VertexArray> Create();
	};
}
