#pragma once

#include "vinpch.hpp"

#include "bindable.hpp"

namespace Vin {

	enum class RenderBufferFormat {
		RGBA16F, //HDR
		RGBA32F, //HDR
		RGBA32,
		RGB24,
		RG16,
		R8,
		R16,
		R32,
		BGRA32,
		DEPTH_COMPONENT16,
		DEPTH_COMPONENT24,
		DEPTH_COMPONENT32F,
		DEPTH24_STENCIL8,
		DEPTH32F_STENCIL8,
		STENCIL_INDEX8
	};

	struct RenderBufferSpecification {
		RenderBufferFormat format;
		bool isTexture; //is it a Texture or a RenderBuffer ? (opengl)

		RenderBufferSpecification(RenderBufferFormat format, bool isTexture);
	};

	struct RenderTargetSpecification {
		uint32_t width, height;
		uint32_t sample{};
		std::vector<RenderBufferSpecification> attachements{};

		RenderTargetSpecification();
		RenderTargetSpecification(uint32_t width, uint32_t height);
		RenderTargetSpecification(uint32_t width, uint32_t height, uint32_t sample);

		void AddRenderBuffer(RenderBufferSpecification spec);
	};

	class RenderTexture : public Bindable<unsigned short> {
	public:
		virtual uint32_t GetSampleCount() const = 0;
	};

	class RenderTarget {
	public:
		virtual ~RenderTarget() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual bool IsValid() const = 0;

		virtual std::shared_ptr<RenderTexture> GetTexture(size_t idx) = 0;
		virtual const RenderTargetSpecification& GetSpecification() = 0;

		static std::shared_ptr<RenderTarget> Create(const RenderTargetSpecification& spec);
	};
}
