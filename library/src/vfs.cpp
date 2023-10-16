#include <vin/vfs/vfs.h>
#include <filesystem>

Vin::Core::Vector<std::pair<Vin::Core::String, Vin::Core::Ref<Vin::VFS::FileSystem>>> Vin::VFS::VirtualFileSystem::filesystems{};

void Vin::VFS::VirtualFileSystem::AddFileSystem(Core::StringView root, Core::Ref<FileSystem> fs) {
    filesystems.emplace_back(std::make_pair(root, fs));
}

Vin::Core::Ref<Vin::VFS::File> Vin::VFS::VirtualFileSystem::Open(Core::StringView path, Vin::VFS::FileMode mode) {
    for (auto& fs : filesystems) {
        size_t idx = path.find(fs.first);
        if (idx != Core::StringView::npos) {
            Core::StringView realPath{ path.substr(idx + fs.first.size()) };
            if (fs.second->Exists(realPath))
                return fs.second->Open(realPath, mode);
        }
    }

    return Core::Ref<File>{ nullptr, nullptr };
}

bool Vin::VFS::VirtualFileSystem::Exists(Core::StringView path) {
    for (auto& fs : filesystems) {
        size_t idx = path.find(fs.first);
        if (idx != Core::StringView::npos) {
            Core::StringView realPath{ path.substr(idx + fs.first.size()) };
            if (fs.second->Exists(realPath))
                return fs.second->Exists(realPath);
        }
    }

    return false;
}