#include <vin/vin.h>
#include <vin/allocator/stdallocator.h>
#include <iostream>
#include <vector>

int main() {
    Test();

    std::vector<int, Vin::Allocator::VinPersistentAllocator<int>> vec{};
    vec.push_back(4);
    vec.push_back(75);

}
