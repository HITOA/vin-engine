#pragma once

#include "resource.hpp"

/*namespace Vin {

	class RawFile : public Resource {
	public:
		virtual void Load(std::unique_ptr<GameFile> file) {
			size = file->GetSize() + 1;
			data = new char[size];

			file->ReadBytes(data, size);
			data[size - 1] = '\0';
		};
		virtual void Unload() {
			delete data;
		};

		char* GetData() {
			return data;
		}

		size_t GetSize() {
			return size;
		}
	private:
		char* data;
		size_t size;
	};

}*/
