#ifndef VIN_ENGINE_NATIVEFILE_H
#define VIN_ENGINE_NATIVEFILE_H

#include <vin/vfs/file.h>
#include <fstream>

namespace Vin::VFS {

    class NativeFile : public File {
    public:
        NativeFile() = delete;
        NativeFile(Core::StringView path, FileMode mode);

        void Close() final;

        size_t ReadBytes(char* buff, size_t buffsize) final;
        size_t WriteBytes(char* buff, size_t buffsize) final;

        bool IsEof() final;
        size_t GetSize() final;
        size_t GetPos() final;
        void SetPos(size_t pos) final;

    private:
        std::fstream fs{};
    };

}

#endif //VIN_ENGINE_NATIVEFILE_H
