#pragma once

#include "vinpch.hpp"

#include "renderer/buffer.hpp"

namespace Vin {
	class VertexArray {
	public:
		virtual ~VertexArray() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void AddVertexBuffer(const std::unique_ptr<VertexBuffer>& vertexBuffer) = 0;

		static std::unique_ptr<VertexArray> Create();
	};
}
