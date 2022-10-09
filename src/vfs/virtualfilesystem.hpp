#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"
#include "filesystem.hpp"
#include "file.hpp"

namespace Vin {

	class VirtualFileSystem { // Map fs location to virtual file system
	public:
		bool AddFileSystem(std::shared_ptr<FileSystem> fs, bool priority = false);
		bool RemoveFileSystem(std::shared_ptr<FileSystem> fs);
		bool SetWriteFileSystem(std::weak_ptr<FileSystem> fs);

		//FS function (Open, Exists, Delete..)
		/**
		 Open a file. Write & Write append should create new file if it doesn't exists.
		 */
		std::unique_ptr<File> Open(std::string_view path, FileMode mode);
		bool Exists(std::string_view path);

	private:
		std::vector<std::shared_ptr<FileSystem>> m_FileSystems{};
		std::weak_ptr<FileSystem> m_WriteFileSystem{};
	};

	class VFS {
	public:
		static bool AddFileSystem(std::shared_ptr<FileSystem> fs, bool priority = false);
		static bool RemoveFileSystem(std::shared_ptr<FileSystem> fs);
		static bool SetWriteFileSystem(std::weak_ptr<FileSystem> fs);

		//FS function (Open, Exists, Delete..)
		/**
		 Open a file. Write & Write append should create new file if it doesn't exists.
		 */
		static std::unique_ptr<File> Open(std::string_view path, FileMode mode);
		static bool Exists(std::string_view path);

	private:
		static VirtualFileSystem m_Vfs;
	};
}
