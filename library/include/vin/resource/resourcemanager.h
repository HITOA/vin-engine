#ifndef VIN_ENGINE_RESOURCEMANAGER_H
#define VIN_ENGINE_RESOURCEMANAGER_H

#include <vin/resource/resourcehandle.h>
#include <vin/core/templates/stdcontainers.h>
#include <vin/resource/resourceloader.h>
#include <vin/vfs/vfs.h>
#include <vin/core/error/error.h>
#include <vin/core/templates/hash.h>

namespace Vin {

    class ResourceManager {
    public:
        template<typename T>
        static Ref<T> Load(StringView path) {
            //make path weakly canonical ?
            unsigned int hashedPath = Hash<StringView>(path);

            for (auto& handle : resources) {
                if (handle.path == hashedPath) {
                    if (handle.typeIndex == typeid(T)) {
                        return Ref<T>{handle.ref.GetRefData(), (T *) handle.ref.Get()};
                    } else {
                        Logger::Logger::Err("Can't load resource: resource already loaded as another type.");
                        return Ref<T>{ nullptr, nullptr };
                    }
                }
            }

            Ref<IO::File> file = VirtualFileSystem::Open(path, IO::FileMode::Read);

            if (!file) {
                Logger::Err("Can't load resource: \"", path, "\" not found.");
                return Ref<T>{ nullptr, nullptr };
            }

            Ref<T> resource = ResourceLoader<T>{}(file);
            file->Close();

            if (!resource) {
                Logger::Err("Can't load resource: \"", path, "\" can't be loaded.");
                return Ref<T>{ nullptr, nullptr };
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
        static Vector<ResourceHandle> resources;
    };

}

#endif //VIN_ENGINE_RESOURCEMANAGER_H
