#include <vin/vfs/vfs.h>
#include <filesystem>

Vin::Vector<std::pair<Vin::String, Vin::Ref<Vin::IO::FileSystem>>> Vin::VirtualFileSystem::filesystems{};

void Vin::VirtualFileSystem::AddFileSystem(StringView root, Ref<IO::FileSystem> fs) {
    filesystems.emplace_back(std::make_pair(root, fs));
}

Vin::Ref<Vin::IO::File> Vin::VirtualFileSystem::Open(StringView path, Vin::IO::FileMode mode) {
    for (auto& fs : filesystems) {
        size_t idx = path.find(fs.first);
        if (idx != StringView::npos) {
            StringView realPath{ path.substr(idx + fs.first.size()) };
            if (fs.second->Exists(realPath))
                return fs.second->Open(realPath, mode);
        }
    }

    return Ref<IO::File>{ nullptr, nullptr };
}

bool Vin::VirtualFileSystem::Exists(StringView path) {
    for (auto& fs : filesystems) {
        size_t idx = path.find(fs.first);
        if (idx != StringView::npos) {
            StringView realPath{ path.substr(idx + fs.first.size()) };
            if (fs.second->Exists(realPath))
                return fs.second->Exists(realPath);
        }
    }

    return false;
}