#include "framebuffer_opengl.hpp"

#include "glad/gl.h"
#include "logger/logger.hpp"

Vin::OpenGLRenderTarget::OpenGLRenderTarget(const RenderTargetSpecification& spec) : m_Specification{ spec }, m_FrameBufferId{}, m_BufferIds{}
{
	Generate();
	if (!IsValid())
		Logger::Err("Can't create RenderTexture, bad layout.");
	Unbind();
}

Vin::OpenGLRenderTarget::~OpenGLRenderTarget()
{
	Destroy();
}

void Vin::OpenGLRenderTarget::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
}

void Vin::OpenGLRenderTarget::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Vin::OpenGLRenderTarget::Resize(int width, int height)
{
	Destroy();
	m_Specification.width = width;
	m_Specification.height = height;
	Generate();
	Unbind();
}

bool Vin::OpenGLRenderTarget::IsValid() const
{
	return glCheckNamedFramebufferStatus(m_FrameBufferId, GL_FRAMEBUFFER);
}

std::shared_ptr<Vin::RenderTexture> Vin::OpenGLRenderTarget::GetTexture(size_t idx)
{
	if (!m_Specification.attachements[idx].isTexture) {
		Vin::Logger::Err("Attachment {} is not a texture.", idx);
		return nullptr;
	}

	return std::make_shared<OpenGLRenderTexture>(&m_BufferIds[idx]);
}

const Vin::RenderTargetSpecification& Vin::OpenGLRenderTarget::GetSpecification()
{
	return m_Specification;
}

void Vin::OpenGLRenderTarget::Generate()
{
	glCreateFramebuffers(1, &m_FrameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);

	for (size_t i = 0; i < m_Specification.attachements.size(); ++i) {
		RenderBufferSpecification& spec = m_Specification.attachements[i];
		if (spec.isTexture) {
			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_BufferIds[i]);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_BufferIds[i]);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
				m_Specification.sample, ParseRenderBufferFormat(spec.format),
				m_Specification.width, m_Specification.height, GL_TRUE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, ParseTextureAttachment(spec.format), GL_TEXTURE_2D_MULTISAMPLE, m_BufferIds[i], 0);
		}
		else {
			glCreateRenderbuffers(1, &m_BufferIds[i]);
			glNamedRenderbufferStorageMultisample(m_BufferIds[i],
				m_Specification.sample, ParseRenderBufferFormat(spec.format),
				m_Specification.width, m_Specification.height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, ParseRenderBufferAttachment(spec.format), GL_RENDERBUFFER, m_BufferIds[i]);
		}
	}
}

void Vin::OpenGLRenderTarget::Destroy()
{
	Unbind();

	glDeleteFramebuffers(1, &m_FrameBufferId);

	for (size_t i = 0; i < m_Specification.attachements.size(); ++i) {
		RenderBufferSpecification& spec = m_Specification.attachements[i];
		if (spec.isTexture) {
			glDeleteTextures(1, &m_BufferIds[i]);
		}
		else {
			glDeleteRenderbuffers(1, &m_BufferIds[i]);
		}
	}
}

unsigned int Vin::OpenGLRenderTarget::ParseRenderBufferFormat(RenderBufferFormat format)
{
	switch (format) {
	case RenderBufferFormat::RGBA32: return GL_RGBA;
	case RenderBufferFormat::RGB24: return GL_RGB;
	case RenderBufferFormat::RG16: return GL_RG16;
	case RenderBufferFormat::R8: return GL_R8;
	case RenderBufferFormat::R16: return GL_R16;
	case RenderBufferFormat::BGRA32: return GL_BGRA;
	case RenderBufferFormat::DEPTH_COMPONENT16: return GL_DEPTH_COMPONENT16;
	case RenderBufferFormat::DEPTH_COMPONENT24: return GL_DEPTH_COMPONENT24;
	case RenderBufferFormat::DEPTH_COMPONENT32F: return GL_DEPTH_COMPONENT32F;
	case RenderBufferFormat::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
	case RenderBufferFormat::DEPTH32F_STENCIL8: return GL_DEPTH32F_STENCIL8;
	case RenderBufferFormat::STENCIL_INDEX8: return GL_STENCIL_INDEX8;
	}
	return 0;
}

unsigned int Vin::OpenGLRenderTarget::ParseTextureAttachment(RenderBufferFormat format)
{
	switch (format) {
	case RenderBufferFormat::RGBA32: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::RGB24: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::RG16: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::R8: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::R16: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::BGRA32: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::DEPTH_COMPONENT16: return GL_DEPTH_ATTACHMENT;
	case RenderBufferFormat::DEPTH_COMPONENT24: return GL_DEPTH_ATTACHMENT;
	case RenderBufferFormat::DEPTH_COMPONENT32F: return GL_DEPTH_ATTACHMENT;
	case RenderBufferFormat::DEPTH24_STENCIL8: return GL_DEPTH_STENCIL_ATTACHMENT;
	case RenderBufferFormat::DEPTH32F_STENCIL8: return GL_DEPTH_STENCIL_ATTACHMENT;
	case RenderBufferFormat::STENCIL_INDEX8: return GL_STENCIL_ATTACHMENT;
	}
	return 0;
}

unsigned int Vin::OpenGLRenderTarget::ParseRenderBufferAttachment(RenderBufferFormat format)
{
	switch (format) {
	case RenderBufferFormat::RGBA32: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::RGB24: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::RG16: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::R8: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::R16: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::BGRA32: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::DEPTH_COMPONENT16: return GL_DEPTH_ATTACHMENT;
	case RenderBufferFormat::DEPTH_COMPONENT24: return GL_DEPTH_ATTACHMENT;
	case RenderBufferFormat::DEPTH_COMPONENT32F: return GL_DEPTH_ATTACHMENT;
	case RenderBufferFormat::DEPTH24_STENCIL8: return GL_DEPTH_STENCIL_ATTACHMENT;
	case RenderBufferFormat::DEPTH32F_STENCIL8: return GL_DEPTH_STENCIL_ATTACHMENT;
	case RenderBufferFormat::STENCIL_INDEX8: return GL_STENCIL_ATTACHMENT;
	}
	return 0;
}

Vin::OpenGLRenderTexture::OpenGLRenderTexture(unsigned int* textureId) : m_TextureId{ textureId } {}

void Vin::OpenGLRenderTexture::Bind(unsigned short location)
{
	glBindTextureUnit(location, *m_TextureId);
}
