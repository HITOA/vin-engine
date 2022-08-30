#pragma once

#include "vinpch.hpp"

namespace Vin {

	enum class RenderBufferFormat {
		RGBA32,
		RGB24,
		RG16,
		R8,
		R16,
		BGRA32,
		DEPTH_COMPONENT16,
		DEPTH_COMPONENT24,
		DEPTH_COMPONENT32F,
		DEPTH24_STENCIL8,
		DEPTH32F_STENCIL8
	};

	struct RenderBufferSpecification {
		RenderBufferFormat format;
		bool isTexture;
	};

	struct RenderTextureLayout {
		size_t width, height;
		size_t sample;
		eastl::vector<RenderBufferSpecification> attachements;
	};

	class RenderTexture {
	public:
		virtual ~RenderTexture() {};

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(int width, int height) = 0;

		static eastl::shared_ptr<RenderTexture> Create();
	};
}
