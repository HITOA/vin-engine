#include <vin/vfs/native/nativefile.h>
#include <filesystem>
#include <limits>

Vin::IO::NativeFile::NativeFile(StringView path, Vin::IO::FileMode mode) {
    static std::ios_base::openmode openmode{};

    switch (mode)
    {
        case FileMode::Read:
            openmode |= std::ios_base::binary | std::ios_base::in;
            break;
        case FileMode::Write:
            openmode |= std::ios_base::binary | std::ios_base::in | std::ios_base::out;
            break;
        case FileMode::WriteAppend:
            openmode |= std::ios_base::binary | std::ios_base::in | std::ios_base::out | std::ios_base::app;
            break;
        default:
            break;
    }

    fs = std::fstream{ std::filesystem::path{ path }, openmode };
}

void Vin::IO::NativeFile::Close() {
    fs.close();
}

size_t Vin::IO::NativeFile::ReadBytes(char *buff, size_t buffsize) {
    fs.read(buff, (std::streamsize)buffsize);
    return fs.gcount();
}
size_t Vin::IO::NativeFile::WriteBytes(char *buff, size_t buffsize) {
    fs.write(buff, (std::streamsize)buffsize);
    return fs.gcount();
}

bool Vin::IO::NativeFile::IsEof() {
    return fs.rdstate() & std::ios_base::eofbit;
}
size_t Vin::IO::NativeFile::GetSize() {
    long p = fs.tellg();
    fs.seekg(0, std::ios_base::beg);
    fs.ignore(std::numeric_limits<std::streamsize>::max());
    size_t size = fs.gcount();
    fs.clear();
    fs.seekg(p);
    return size;
}
size_t Vin::IO::NativeFile::GetPos() {
    return fs.tellg();
}
void Vin::IO::NativeFile::SetPos(size_t pos) {
    fs.seekg((long)pos);
    fs.seekp((long)pos);
}