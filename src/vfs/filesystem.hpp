#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"
#include "file.hpp"

namespace Vin {

	class FileSystem { //Base class for implementing fs (NativeFS, GameFS..)
	public:
		//Virtual fs function (Open, Exists, Delete..)
		virtual std::unique_ptr<File> Open(std::string_view path, FileMode mode) = 0;
		virtual bool Exists(std::string_view path) = 0;
	};

}
