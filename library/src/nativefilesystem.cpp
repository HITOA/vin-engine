#include <vin/vfs/native/nativefilesystem.h>
#include <vin/vfs/native/nativefile.h>
#include <filesystem>


Vin::IO::NativeFileSystem::NativeFileSystem() {
    root = std::filesystem::current_path().string();
}

Vin::IO::NativeFileSystem::NativeFileSystem(StringView root) : root{ root } {
}

Vin::Ref<Vin::IO::File> Vin::IO::NativeFileSystem::Open(StringView path, Vin::IO::FileMode mode) {
    std::filesystem::path p{ root };
    p /= path;

    return MakeRef<NativeFile>(p.string(), mode);
}

bool Vin::IO::NativeFileSystem::Exists(StringView path) {
    std::filesystem::path realPath{ root };
    realPath /= path;

    return std::filesystem::exists(realPath);
}

