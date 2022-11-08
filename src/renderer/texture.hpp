#pragma once 

#include "vinpch.hpp"

#include "bindable.hpp"

namespace Vin {

	enum class TextureFormat {
		RGBA32,
		RGB24,
		RG16,
		R8,
		R16,
		BGRA32
	};

	enum class TextureFiltering {
		Point,
		Bilinear,
		Trilinear
	};

	enum class TextureWrapping {
		Repeat,
		ClampEdge,
		Clamp,
		MirroredRepeat
	};

	enum class CubemapTexture {
		Right = 0,
		Left,
		Top,
		Bottom,
		Back,
		Front
	};

	class Texture : public Bindable<unsigned short> {
	public:
		virtual ~Texture() {};

		virtual TextureFormat GetFormat() = 0;
		virtual TextureFiltering GetFiltering() = 0;
		virtual TextureWrapping GetWrapping() = 0;
		virtual size_t GetWidth() = 0;
		virtual size_t GetHeight() = 0;

		virtual void SetFiltering(TextureFiltering filtering) = 0;
		virtual void SetWrapping(TextureWrapping wrapping) = 0;

		virtual void SetData(void* data) = 0;

		static std::shared_ptr<Texture> Create(size_t width, size_t height, TextureFormat format = TextureFormat::RGBA32, bool mipmap = true);
	};

	class Cubemap : public Bindable<unsigned short> {
	public:
		virtual ~Cubemap() {};

		virtual TextureFormat GetFormat() = 0;
		virtual TextureFiltering GetFiltering() = 0;
		virtual TextureWrapping GetWrapping() = 0;
		virtual size_t GetWidth() = 0;
		virtual size_t GetHeight() = 0;

		virtual void SetFiltering(TextureFiltering filtering) = 0;
		virtual void SetWrapping(TextureWrapping wrapping) = 0;

		virtual void SetData(CubemapTexture side, void* data) = 0;

		static std::shared_ptr<Cubemap> Create(size_t width, size_t height, TextureFormat format = TextureFormat::RGBA32, bool mipmap = false);
	};
}
