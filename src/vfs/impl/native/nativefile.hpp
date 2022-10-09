#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"

#include "vfs/file.hpp"

namespace Vin {
	
	class NativeFile : public File {
	public:
		NativeFile(const std::filesystem::path& path, FileMode mode);

	public:
		void Close() final;

		bool IsValid() final;

		size_t ReadBytes(char* buff, size_t buffsize) final;
		size_t WriteBytes(char* buff, size_t buffsize) final;

		bool IsEof() final;
		size_t GetSize() final;
		size_t GetPos() final;
		void SetPos(size_t pos) final;

	private:
		std::fstream m_FileStream{};
		std::filesystem::path m_Path{};
	};

}
