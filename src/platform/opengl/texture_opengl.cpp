#include "texture_opengl.hpp"

#include "glad/gl.h"
#include "core/math/mathhelper.hpp"

Vin::OpenGLTexture::OpenGLTexture(size_t width, size_t height, TextureFormat format, bool mipmap) : 
	m_Width{ width }, m_Height{ height }, m_Format{ format }, m_Mipmap{ mipmap },
	m_Filtering{ TextureFiltering::Bilinear }, m_Wrapping{ TextureWrapping::Repeat }
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);
	if (!mipmap) {
		glTextureStorage2D(m_TextureId, 1, ParseTextureInternalFormat(m_Format), width, height);
	}
	else {
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
		size_t levels = 0;
		while (width > 1 && height > 1 && levels < GL_MAX_TEXTURE_SIZE) {
			glTexImage2D(GL_TEXTURE_2D, levels, ParseTextureInternalFormat(m_Format), width, height,
				0, ParseTextureFormat(m_Format), ParseTextureFormatType(m_Format), NULL);
			width = Max<int>(width / 2, 1);
			height = Max<int>(height / 2, 1);
			levels++;
		}
	}

	SetFiltering(m_Filtering);
	SetWrapping(m_Wrapping);
}

Vin::OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &m_TextureId);
}

Vin::TextureFormat Vin::OpenGLTexture::GetFormat()
{
	return m_Format;
}

Vin::TextureFiltering Vin::OpenGLTexture::GetFiltering()
{
	return m_Filtering;
}

Vin::TextureWrapping Vin::OpenGLTexture::GetWrapping()
{
	return m_Wrapping;
}

size_t Vin::OpenGLTexture::GetWidth()
{
	return m_Width;
}

size_t Vin::OpenGLTexture::GetHeight()
{
	return m_Height;
}

void Vin::OpenGLTexture::SetFiltering(TextureFiltering filtering)
{
	switch (filtering)
	{
	case Vin::TextureFiltering::Point:
		glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case Vin::TextureFiltering::Bilinear:
		glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case Vin::TextureFiltering::Trilinear:
		glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	default:
		glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	}
}

void Vin::OpenGLTexture::SetWrapping(TextureWrapping wrapping)
{
	glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_S, ParseTextureWrapping(wrapping));
	glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_T, ParseTextureWrapping(wrapping));
}

void Vin::OpenGLTexture::SetData(void* data)
{
	glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height, ParseTextureFormat(m_Format), ParseTextureFormatType(m_Format), data);
	if (m_Mipmap) {
		glGenerateTextureMipmap(m_TextureId);
	}
}

void Vin::OpenGLTexture::Bind(unsigned short location)
{
	glBindTextureUnit(location, m_TextureId);
}

int Vin::OpenGLTexture::ParseTextureWrapping(TextureWrapping wrapping)
{
	switch (wrapping)
	{
	case TextureWrapping::Repeat:
		return GL_REPEAT;
	case TextureWrapping::Clamp:
		return GL_CLAMP_TO_BORDER;
	case TextureWrapping::ClampEdge:
		return GL_CLAMP_TO_EDGE;
	case TextureWrapping::MirroredRepeat:
		return GL_MIRRORED_REPEAT;
	default:
		return GL_REPEAT;
	}
}

int Vin::OpenGLTexture::ParseTextureFormat(TextureFormat format)
{
	switch (format) {
	case TextureFormat::RGBA32:
		return GL_RGBA;
	case TextureFormat::RGB24:
		return GL_RGB;
	case TextureFormat::RG16:
		return GL_RG;
	case TextureFormat::R8:
		return GL_RED;
	case TextureFormat::R16:
		return GL_RED;
	case TextureFormat::BGRA32:
		return GL_BGRA;
	default:
		return GL_RGBA;
	}
}

int Vin::OpenGLTexture::ParseTextureFormatType(TextureFormat format)
{
	switch (format) {
	case TextureFormat::RGBA32:
		return GL_UNSIGNED_BYTE;
	case TextureFormat::RGB24:
		return GL_UNSIGNED_BYTE;
	case TextureFormat::RG16:
		return GL_UNSIGNED_BYTE;
	case TextureFormat::R8:
		return GL_UNSIGNED_BYTE;
	case TextureFormat::R16:
		return GL_UNSIGNED_SHORT;
	case TextureFormat::BGRA32:
		return GL_UNSIGNED_BYTE;
	default:
		return GL_UNSIGNED_BYTE;
	}
}

int Vin::OpenGLTexture::ParseTextureInternalFormat(TextureFormat format)
{
	switch (format) {
	case TextureFormat::RGBA32:
		return GL_RGBA8;
	case TextureFormat::RGB24:
		return GL_RGB8;
	case TextureFormat::RG16:
		return GL_RG8;
	case TextureFormat::R8:
		return GL_R8;
	case TextureFormat::R16:
		return GL_R16;
	case TextureFormat::BGRA32:
		return GL_RGBA8;
	default:
		return GL_RGBA8;
	}
}
