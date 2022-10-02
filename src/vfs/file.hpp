#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"

namespace Vin {

	enum class FileMode {
		None = 0,
		Read,
		Write,
		WriteAppend
	};

	class File { //Base class for implementing fs (NativeFile, GameFile)
	public:
		virtual bool IsValid() = 0;
		//Virtual function to work on file
		virtual size_t ReadBytes(char* buff, size_t buffsize) = 0;
		virtual size_t WriteBytes(char* buff, size_t buffsize) = 0;

		virtual bool IsEof() = 0;
		virtual size_t GetSize() = 0;
		virtual size_t GetPos() = 0;
		virtual void SetPos(size_t pos) = 0;
	};

}
