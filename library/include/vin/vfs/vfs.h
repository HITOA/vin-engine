#ifndef VIN_ENGINE_VFS_H
#define VIN_ENGINE_VFS_H

#include <vin/core/templates/ref.h>
#include <vin/core/templates/stdcontainers.h>
#include <vin/vfs/filesystem.h>

namespace Vin::VFS {

    class VirtualFileSystem {
    public:
        static void AddFileSystem(Core::StringView root, Core::Ref<FileSystem> fs);

        static Core::Ref<File> Open(Core::StringView path, FileMode mode);
        static bool Exists(Core::StringView path);

    private:
        static Core::Vector<std::pair<Core::String, Core::Ref<FileSystem>>> filesystems;
    };

}

#endif //VIN_ENGINE_VFS_H
