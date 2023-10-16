#ifndef VIN_ENGINE_RESOURCEHANDLE_H
#define VIN_ENGINE_RESOURCEHANDLE_H

#include <vin/core/templates/ref.h>
#include <typeindex>

namespace Vin::Resource {

    struct ResourceHandle {
    public:
        ResourceHandle() = delete;
        ResourceHandle(Core::RefData* refData, char* ptr, std::type_index typeIndex, unsigned int path)
            : ref{ refData, ptr }, typeIndex{ typeIndex }, path{ path } {};

    public:
        Core::Ref<char> ref;
        std::type_index typeIndex;
        unsigned int path;
    };

}

#endif //VIN_ENGINE_RESOURCEHANDLE_H
