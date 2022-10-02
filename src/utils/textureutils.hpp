#pragma once

#include <vinpch.hpp>
#include <renderer/texture.hpp>
#include <resources/resources.hpp>
#include <resources/image.hpp>

namespace Vin {

	std::shared_ptr<Vin::Texture> LoadTexture(const char* path, bool mipmap = true) {
		/*std::shared_ptr<Vin::Image> img = Vin::Resources::Load<Vin::Image>(path);
		if (img.get() == nullptr)
			return nullptr;

		std::shared_ptr<Vin::Texture> texture = Vin::Texture::Create(
			img->GetWidth(), img->GetHeight(), 
			img->GetChannels() > 3 ? Vin::TextureFormat::RGBA32 : Vin::TextureFormat::RGB24, mipmap);
		texture->SetData(img->GetData());

		Vin::Resources::Unload(img);

		return texture;*/
		return nullptr;
	}

}
