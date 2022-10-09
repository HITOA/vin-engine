#pragma once

#include "vinpch.hpp"
#include "core/base.hpp"

#include "vfs/file.hpp"

namespace Vin {

	class PakFile : public File {
	public:
		PakFile(char* buff, size_t buffsize); //Data must be uncompressed
		
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
		char* m_Buff;
		size_t m_BuffSize;
		size_t m_BuffIdx;
	};

}
