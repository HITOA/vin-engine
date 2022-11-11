#pragma once

#include "renderer/texture.hpp"

namespace Vin {
	class OpenGLTexture : public Texture {
	public:
		OpenGLTexture(size_t width, size_t height, TextureFormat format, bool mipmap);
		~OpenGLTexture();

		TextureFormat GetFormat() final;
		TextureFiltering GetFiltering() final;
		TextureWrapping GetWrapping() final;
		size_t GetWidth() final;
		size_t GetHeight() final;

		void SetFiltering(TextureFiltering filtering) final;
		void SetWrapping(TextureWrapping wrapping) final;

		void SetData(void* data) final;
		void Bind(unsigned short location) final;
		void Unbind() final;
	private:
		int ParseTextureWrapping(TextureWrapping wrapping);
		int ParseTextureFormat(TextureFormat format);
		int ParseTextureFormatType(TextureFormat format);
		int ParseTextureInternalFormat(TextureFormat format);
	private:
		unsigned int m_TextureId;
		bool m_Mipmap;
		size_t m_Width, m_Height;
		TextureFormat m_Format;
		TextureFiltering m_Filtering;
		TextureWrapping m_Wrapping;
		unsigned short m_LastBindedLocation;
	};

	class OpenGLCubemap : public Cubemap {
		OpenGLCubemap(size_t width, size_t height, TextureFormat format, bool mipmap);
		~OpenGLCubemap();

		TextureFormat GetFormat() final;
		TextureFiltering GetFiltering() final;
		TextureWrapping GetWrapping() final;
		size_t GetWidth() final;
		size_t GetHeight() final;

		void SetFiltering(TextureFiltering filtering) final;
		void SetWrapping(TextureWrapping wrapping) final;

		void SetData(CubemapTexture side, void* data) final;
		void Bind(unsigned short location) final;

	private:
		int ParseTextureWrapping(TextureWrapping wrapping);
		int ParseTextureFormat(TextureFormat format);
		int ParseTextureFormatType(TextureFormat format);
		int ParseTextureInternalFormat(TextureFormat format);

	private:
		unsigned int m_CubemapId;
		bool m_Mipmap;
		size_t m_Width, m_Height;
		TextureFormat m_Format;
		TextureFiltering m_Filtering;
		TextureWrapping m_Wrapping;
	};
}
