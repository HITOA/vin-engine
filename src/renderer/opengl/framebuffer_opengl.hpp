#pragma once

#include "renderer/framebuffer.hpp"

namespace Vin {

	class OpenGLRenderTexture : public RenderTexture {
	public:
		OpenGLRenderTexture(unsigned int* textureId, uint32_t samplecount);

		void Bind(unsigned short location);

		uint32_t GetSampleCount() const;
	private:
		unsigned int* m_TextureId;
		uint32_t m_SampleCount;
	};

	class OpenGLRenderTarget : public RenderTarget {
	public:
		OpenGLRenderTarget(const RenderTargetSpecification& spec);
		~OpenGLRenderTarget();

		void Bind() const;
		void Unbind() const;

		void Resize(uint32_t width, uint32_t height);

		bool IsValid() const;

		std::shared_ptr<RenderTexture> GetTexture(size_t idx);
		const RenderTargetSpecification& GetSpecification();

		unsigned int GetFrameBufferId();
	private:
		void Generate();
		void Destroy();
		unsigned int ParseRenderBufferFormat(RenderBufferFormat format);
		unsigned int ParseTextureAttachment(RenderBufferFormat format);
		unsigned int ParseRenderBufferAttachment(RenderBufferFormat format);

	private:
		RenderTargetSpecification m_Specification{};
		unsigned int m_FrameBufferId{};
		unsigned int m_BufferIds[16]{}; //Texture or Renderbuffer ids
		bool m_NoColorAttachment{ true };
	};

}
