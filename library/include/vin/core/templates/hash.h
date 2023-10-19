#ifndef VIN_ENGINE_HASH_H
#define VIN_ENGINE_HASH_H

#include <vin/core/templates/stdcontainers.h>

namespace Vin {

    template<typename T>
    inline unsigned int Hash(T& v) {
        static_assert(false, "No hash function for specified type.");
        return 0;
    }

    template<>
    inline unsigned int Hash<StringView>(StringView& v) {
        unsigned int hash = 2166136261U;
        for (auto& c : v) {
            hash ^= c;
            hash *= 16777619U;
        }
        return hash;
    }

}

#endif //VIN_ENGINE_HASH_H
