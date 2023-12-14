#ifndef VIN_ENGINE_FIXEDVECTOR_H
#define VIN_ENGINE_FIXEDVECTOR_H

#include <cstddef>
#include <vin/core/templates/containerbase.h>
#include <initializer_list>

namespace Vin {

    template<typename T, size_t size>
    class FixedVector {
    public:
        FixedVector() = default;
        FixedVector(std::initializer_list<T> l) {
            count = l.size() > size ? size : l.size();
            for (size_t i = 0; i < count; ++i) {
                auto it = l.begin() + i;
                data[i] = *it;
            }
        }

        ContainerStatus TryPush(const T& v) {
            if (count >= size)
                return ContainerStatus::Full;

            data[count++] = v;
            return ContainerStatus::Success;
        }
        [[nodiscard]] inline size_t GetCount() const {
            return count;
        }
        [[nodiscard]] inline bool IsEmpty() const {
            return count <= 0;
        }
        [[nodiscard]] inline bool IsFull() const {
            return count >= size;
        }
        [[nodiscard]] inline T* GetData() {
            return data;
        }

        inline T& operator[](size_t idx) {
            return data[idx];
        }

        inline const T& operator[](size_t idx) const {
            return data[idx];
        }

    private:
        size_t count{ 0 };
        T data[size];
    };

}

#endif //VIN_ENGINE_FIXEDVECTOR_H
