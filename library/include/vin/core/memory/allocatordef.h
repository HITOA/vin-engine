#ifndef VIN_ENGINE_ALLOCATORDEF_H
#define VIN_ENGINE_ALLOCATORDEF_H

#include <stddef.h>

namespace Vin::Core::Memory {

    struct Blk {
        void* ptr{ nullptr };
        size_t size{ 0 };
    };

}

#endif //VIN_ENGINE_ALLOCATORDEF_H
