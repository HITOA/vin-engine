#include "framebuffer_opengl.hpp"

#include "glad/gl.h"
#include "logger/logger.hpp"

Vin::OpenGLRenderTarget::OpenGLRenderTarget(const RenderTargetSpecification& spec) : 
	m_Specification{ spec }, m_FrameBufferId{}, m_BufferIds{}, m_NoColorAttachment{ true }
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
	OPTICK_GPU_EVENT("Bind FrameBuffer");
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
}

void Vin::OpenGLRenderTarget::Unbind() const
{
	OPTICK_GPU_EVENT("Unbind FrameBuffer");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Vin::OpenGLRenderTarget::Resize(uint32_t width, uint32_t height)
{
	OPTICK_EVENT();
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

	return std::make_shared<OpenGLRenderTexture>(&m_BufferIds[idx], m_Specification.sample);
}

const Vin::RenderTargetSpecification& Vin::OpenGLRenderTarget::GetSpecification()
{
	return m_Specification;
}

unsigned int Vin::OpenGLRenderTarget::GetFrameBufferId()
{
	return m_FrameBufferId;
}

void Vin::OpenGLRenderTarget::Generate()
{
	OPTICK_GPU_EVENT("Generate Framebuffer");

	glCreateFramebuffers(1, &m_FrameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);

	for (size_t i = 0; i < m_Specification.attachements.size(); ++i) {
		RenderBufferSpecification& spec = m_Specification.attachements[i];
		if (m_Specification.sample > 1) {
			if (spec.isTexture) {
				GLuint attachment = ParseTextureAttachment(spec.format);

				if (attachment == GL_COLOR_ATTACHMENT0)
					m_NoColorAttachment = false;

				glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_BufferIds[i]);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_BufferIds[i]);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,
					m_Specification.sample, ParseRenderBufferFormat(spec.format),
					m_Specification.width, m_Specification.height, GL_TRUE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D_MULTISAMPLE, m_BufferIds[i], 0);
			}
			else {
				GLuint attachment = ParseRenderBufferAttachment(spec.format);

				if (attachment == GL_COLOR_ATTACHMENT0)
					m_NoColorAttachment = false;

				glCreateRenderbuffers(1, &m_BufferIds[i]);
				glNamedRenderbufferStorageMultisample(m_BufferIds[i],
					m_Specification.sample, ParseRenderBufferFormat(spec.format),
					m_Specification.width, m_Specification.height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_BufferIds[i]);
			}
		}
		else {
			if (spec.isTexture) {
				GLuint attachment = ParseTextureAttachment(spec.format);

				if (attachment == GL_COLOR_ATTACHMENT0)
					m_NoColorAttachment = false;

				glCreateTextures(GL_TEXTURE_2D, 1, &m_BufferIds[i]);
				glBindTexture(GL_TEXTURE_2D, m_BufferIds[i]);
				glTexStorage2D(GL_TEXTURE_2D, 1,
					ParseRenderBufferFormat(spec.format),
					m_Specification.width, m_Specification.height);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				if (attachment == GL_DEPTH_ATTACHMENT)
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

				glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_BufferIds[i], 0);
			}
			else {
				GLuint attachment = ParseRenderBufferAttachment(spec.format);

				if (attachment == GL_COLOR_ATTACHMENT0)
					m_NoColorAttachment = false;

				glCreateRenderbuffers(1, &m_BufferIds[i]);
				glNamedRenderbufferStorage(m_BufferIds[i],
					ParseRenderBufferFormat(spec.format),
					m_Specification.width, m_Specification.height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_BufferIds[i]);
			}
		}
	}

	if (m_NoColorAttachment) { //Doesn't seem to work ?
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
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
	case RenderBufferFormat::RGBA32F: return GL_RGBA32F;
	case RenderBufferFormat::RGBA16F: return GL_RGBA16F;
	case RenderBufferFormat::RGBA32: return GL_RGBA;
	case RenderBufferFormat::RGB24: return GL_RGB;
	case RenderBufferFormat::RG16: return GL_RG16;
	case RenderBufferFormat::R8: return GL_R8;
	case RenderBufferFormat::R16: return GL_R16;
	case RenderBufferFormat::R32: return GL_R32F;
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
	case RenderBufferFormat::RGBA32F: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::RGBA16F: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::RGBA32: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::RGB24: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::RG16: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::R8: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::R16: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::R32: return GL_COLOR_ATTACHMENT0;
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
	case RenderBufferFormat::RGBA32F: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::RGBA16F: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::RGBA32: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::RGB24: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::RG16: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::R8: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::R16: return GL_COLOR_ATTACHMENT0;
	case RenderBufferFormat::R32: return GL_COLOR_ATTACHMENT0;
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

Vin::OpenGLRenderTexture::OpenGLRenderTexture(unsigned int* textureId, uint32_t samplecount) : m_TextureId{ textureId }, m_SampleCount{ samplecount } {}

void Vin::OpenGLRenderTexture::Bind(unsigned short location)
{
	glBindTextureUnit(location, *m_TextureId);
}

uint32_t Vin::OpenGLRenderTexture::GetSampleCount() const
{
	return m_SampleCount;
}
