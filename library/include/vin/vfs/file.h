#ifndef VIN_ENGINE_FILE_H
#define VIN_ENGINE_FILE_H

#include <vin/core/templates/ref.h>
#include <vin/core/templates/stdcontainers.h>

namespace Vin::VFS {

    enum class FileMode {
        Read = 0,
        Write,
        WriteAppend
    };

    class File {
    public:
        virtual ~File() { Close(); };

    public:
        virtual void Close() {};

        virtual size_t ReadBytes(char* buff, size_t buffsize) = 0;
        virtual size_t WriteBytes(char* buff, size_t buffsize) = 0;

        virtual bool IsEof() = 0;
        virtual size_t GetSize() = 0;
        virtual size_t GetPos() = 0;
        virtual void SetPos(size_t pos) = 0;
    };

}

#endif //VIN_ENGINE_FILE_H
