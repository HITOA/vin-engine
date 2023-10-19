#ifndef VIN_ENGINE_STDCONTAINER_H
#define VIN_ENGINE_STDCONTAINER_H

#include <vin/core/memory/stdallocator.h>
#include <string>
#include <vector>

namespace Vin {

    /**
     * STD Container in Vin namespace, they use the vin allocator.
     */

    template<typename T = char, Core::AllocationStrategy strategy = Core::AllocationStrategy::Persistent>
    using BasicString = std::basic_string<T, std::char_traits<T>, Core::VinStdAllocator<T, strategy>>;
    template<typename T = char>
    using BasicStringView = std::basic_string_view<T, std::char_traits<T>>;
    using String = BasicString<>;
    using StringView = BasicStringView<>;

    template<typename T, Core::AllocationStrategy strategy = Core::AllocationStrategy::Persistent>
    using Vector = std::vector<T, Core::VinStdAllocator<T, strategy>>;

}

#endif //VIN_ENGINE_STDCONTAINER_H
