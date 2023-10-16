#ifndef VIN_ENGINE_RESOURCEMANAGER_H
#define VIN_ENGINE_RESOURCEMANAGER_H

#include <vin/resource/resourcehandle.h>
#include <vin/core/templates/stdcontainers.h>
#include <vin/resource/resourceloader.h>
#include <vin/vfs/vfs.h>
#include <vin/core/error/error.h>
#include <vin/core/templates/hash.h>

namespace Vin::Resource {

    class ResourceManager {
    public:
        template<typename T>
        static Core::Ref<T> Load(Core::StringView path) {
            unsigned int hashedPath = Core::Hash<Core::StringView>(path);

            for (auto& handle : resources) {
                if (handle.path == hashedPath) {
                    if (handle.typeIndex == typeid(T)) {
                        return Core::Ref<T>{handle.ref.GetRefData(), (T *) handle.ref.Get()};
                    } else {
                        Core::Logger::Logger::Err("Can't load resource: resource already loaded as another type.");
                        return Core::Ref<T>{ nullptr, nullptr };
                    }
                }
            }

            Core::Ref<VFS::File> file = VFS::VirtualFileSystem::Open(path, VFS::FileMode::Read);

            if (!file) {
                Core::Logger::Logger::Err("Can't load resource: \"", path, "\" not found.");
                return Core::Ref<T>{ nullptr, nullptr };
            }

            Core::Ref<T> resource = ResourceLoader<T>{}(file);
            file->Close();

            if (!resource) {
                Core::Logger::Logger::Err("Can't load resource: \"", path, "\" can't be loaded.");
                return Core::Ref<T>{ nullptr, nullptr };
            }

            resources.emplace_back(resource.GetRefData(), (char*)resource.Get(), typeid(T), hashedPath);

            return resource;
        }

        static inline void UnloadUnused() {
            size_t i = 0;
            while (i < resources.size()) {
                if (resources[i].ref.GetOwnerCount() <= 1) {
                    resources.erase(resources.cbegin() + i);
                } else {
                    ++i;
                }
            }
        }

        static inline int GetResourceCount() {
            return (int)resources.size();
        }


    private:
        static Core::Vector<ResourceHandle> resources;
    };

}

#endif //VIN_ENGINE_RESOURCEMANAGER_H
