#ifndef VIN_ENGINE_STDCONTAINER_H
#define VIN_ENGINE_STDCONTAINER_H

#include <vin/core/memory/stdallocator.h>
#include <string>
#include <vector>
#include <unordered_map>

#define PATH_TO_STRING(name) name.string<char, std::char_traits<char>, Vin::Core::VinStdAllocator<char, Vin::Core::AllocationStrategy::Persistent>>()

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

    template<typename KeyT, typename ValueT, typename Hash = std::hash<KeyT>, typename KeyEqual = std::equal_to<KeyT>, Core::AllocationStrategy strategy = Core::AllocationStrategy::Persistent>
    using UnorderedMap = std::unordered_map<KeyT, ValueT, Hash, KeyEqual, Core::VinStdAllocator<std::pair<const KeyT, ValueT>, strategy>>;

}

#endif //VIN_ENGINE_STDCONTAINER_H
