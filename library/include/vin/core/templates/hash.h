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
    inline unsigned int Hash<const unsigned int>(const unsigned int& v) {
        return v;
    }

    template<>
    inline unsigned int Hash<unsigned int>(unsigned int& v) {
        return v;
    }

    template<>
    inline unsigned int Hash<const StringView>(const StringView& v) {
        static unsigned int hash = 2166136261U;
        for (auto& c : v) {
            hash ^= c;
            hash *= 16777619U;
        }
        return hash;
    }

    template<>
    inline unsigned int Hash<StringView>(StringView& v) {
        return Hash<const StringView>(v);
    }

    template<typename T>
    inline void HashCombine(unsigned int& hash, T& v) {
        hash ^= Hash<T>(v) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }
}

#endif //VIN_ENGINE_HASH_H
