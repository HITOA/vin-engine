#ifndef VIN_ENGINE_VFS_H
#define VIN_ENGINE_VFS_H

#include <vin/core/templates/ref.h>
#include <vin/core/templates/stdcontainers.h>
#include <vin/vfs/filesystem.h>

namespace Vin {

    class VirtualFileSystem {
    public:
        static void AddFileSystem(StringView root, Ref<IO::FileSystem> fs);

        static Ref<IO::File> Open(StringView path, IO::FileMode mode);
        static bool Exists(StringView path);

    private:
        static Vector<std::pair<String, Ref<IO::FileSystem>>> filesystems;
    };

}

#endif //VIN_ENGINE_VFS_H
