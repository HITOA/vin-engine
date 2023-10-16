#include <vin/vfs/native/nativefilesystem.h>
#include <vin/vfs/native/nativefile.h>
#include <filesystem>


Vin::VFS::NativeFileSystem::NativeFileSystem() {
    root = std::filesystem::current_path().string();
}

Vin::VFS::NativeFileSystem::NativeFileSystem(Core::StringView root) : root{ root } {
}

Vin::Core::Ref<Vin::VFS::File> Vin::VFS::NativeFileSystem::Open(Core::StringView path, Vin::VFS::FileMode mode) {
    std::filesystem::path p{ root };
    p /= path;

    return Vin::Core::MakeRef<Vin::VFS::NativeFile>(p.string(), mode);
}

bool Vin::VFS::NativeFileSystem::Exists(Core::StringView path) {
    std::filesystem::path realPath{ root };
    realPath /= path;

    return std::filesystem::exists(realPath);
}

