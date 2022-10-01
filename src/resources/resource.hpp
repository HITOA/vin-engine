#pragma once

#include "vinpch.hpp"
#include "filesystem/gamefilesystem.hpp"

namespace Vin {
	typedef unsigned int ResourceHandle;
	typedef unsigned short ResourceTypeId;

	class ResourceTrait {
	public:
		static ResourceHandle GetNextHandle() {
			return ++s_LastHandle;
		}
		template<typename T>
		static const ResourceTypeId GetTypeId() {
			static const ResourceTypeId typeId = ++s_LastTypeId;
			return typeId;
		}
	private:
		static ResourceHandle s_LastHandle;
		static ResourceTypeId s_LastTypeId;
	};

	class Resource {
	public:
		virtual ~Resource() {};

		virtual void Load(std::unique_ptr<GameFile> file) = 0;
		virtual void Unload() = 0;

		ResourceHandle GetHandle() {
			return m_Handle;
		}

		const char* GetPath() {
			return m_Path;
		}
	private:
		ResourceHandle m_Handle;
		ResourceTypeId m_TypeId;
		const char* m_Path;

		friend class Resources;
	};
}
