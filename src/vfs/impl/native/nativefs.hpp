#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"

#include "vfs/filesystem.hpp"

namespace Vin {

	class NativeFS : public FileSystem {
	public:
		NativeFS() = delete;
		NativeFS(std::string_view root);

	public:
		bool IsValid() final;

		std::unique_ptr<File> Open(std::string_view path, FileMode mode) final;
		bool Exists(std::string_view path) final;

	private:
		std::filesystem::path m_Root;
	};

}
