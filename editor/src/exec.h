#ifndef VIN_EDITOR_EXEC_H
#define VIN_EDITOR_EXEC_H

#include <vin/vin.h>
#include <stdio.h>
#include <memory>

inline Vin::String Exec(const char* cmd) {
    static char buff[128]{};
    Vin::String result{};
#ifdef VIN_WIN32
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
#else
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
#endif
    if (!pipe) {
        Vin::Logger::Err("popen() failed.");
        return "";
    }
    while (fgets(buff, 128, pipe.get()) != nullptr) {
        result += buff;
    }
    return result;
}

#endif //VIN_EDITOR_EXEC_H
