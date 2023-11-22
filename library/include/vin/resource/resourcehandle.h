#ifndef VIN_ENGINE_RESOURCEHANDLE_H
#define VIN_ENGINE_RESOURCEHANDLE_H

#include <vin/core/templates/ref.h>
#include <typeindex>

namespace Vin {

    struct ResourceHandle {
    public:
        ResourceHandle() = delete;
        ResourceHandle(RefData* refData, char* ptr, std::type_index typeIndex, unsigned int path)
            : ref{ refData, ptr }, typeIndex{ typeIndex }, path{ path } {};

    public:
        Ref<char> ref;
        std::type_index typeIndex;
        unsigned int path;
        //bool persistent;
    };

}

#endif //VIN_ENGINE_RESOURCEHANDLE_H
