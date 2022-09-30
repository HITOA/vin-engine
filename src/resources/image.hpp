#pragma once

#include "vinpch.hpp"
#include "resource.hpp"
#define STB_IMAGE_IMPLEMENTATION

#pragma warning(push, 0)  
#include <stb_image.h>
#pragma warning(pop)

namespace Vin {
	class Image : public Resource {
	public:
		void Load(eastl::unique_ptr<GameFile> file) final {
			size_t filesize = file->GetSize();

			char* buff = new char[filesize];
			file->ReadBytes(buff, filesize);

			m_Data = stbi_load_from_memory((unsigned char*)buff, filesize, &m_Width, &m_Height, &m_Channels, 0);

			delete[] buff;
		};

		void Unload() final {
			stbi_image_free(m_Data);
		};

		int GetWidth() {
			return m_Width;
		}

		int GetHeight() {
			return m_Height;
		}

		int GetChannels() {
			return m_Channels;
		}

		unsigned char* GetData() {
			return m_Data;
		}

	private:
		int m_Width, m_Height, m_Channels;
		unsigned char* m_Data;
	};
}
