#ifndef VIN_ENGINE_UTILS_H
#define VIN_ENGINE_UTILS_H

#include <stddef.h>
#include <utility>
#include <vin/core/templates/stdcontainers.h>

namespace Vin {

    template<Core::AllocationStrategy strategy>
    inline void NormalizePath(BasicString<char, strategy>& path) {
        for (auto& c : path)
            if (c == '\\')
                c = '/';
        if (path[path.size() - 1] != '/')
            path += '/';
    }

    template<Core::AllocationStrategy strategy = Vin::Core::AllocationStrategy::Persistent>
    inline Vin::BasicString<char, strategy> GenerateRandomNameId(size_t length) {
        static const char* okChars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

        Vin::BasicString<char, strategy> name{};
        name.resize(length);

        for (size_t i = 0; i < length; ++i) {
            name[i] = okChars[rand() % 62];
        }

        return name;
    }
}

#endif //VIN_ENGINE_UTILS_H
