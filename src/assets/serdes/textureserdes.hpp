#pragma once

#include "assets/assetserdes.hpp"
#include "renderer/texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Vin {

	template<>
	struct AssetDeserializer<Texture> {
		static std::shared_ptr<Texture> Deserialize(std::unique_ptr<File> file) {
			size_t filesize = file->GetSize();
			char* buff = new char[filesize];

			file->ReadBytes(buff, filesize);

			int width, height, channels;
			unsigned char* data = stbi_load_from_memory((unsigned char*)buff, (int)filesize, &width, &height, &channels, 0);

			delete[] buff;

			std::shared_ptr<Texture> tex = Texture::Create(width, height, channels > 3 ? Vin::TextureFormat::RGBA32 : Vin::TextureFormat::RGB24);
			tex->SetData(data);

			stbi_image_free(data);

			return tex;
		}
	};

}
