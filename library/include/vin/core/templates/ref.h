#ifndef VIN_ENGINE_REF_H
#define VIN_ENGINE_REF_H

#include <stddef.h>
#include <vin/core/memory/memorymanager.h>

namespace Vin {

    struct RefData {
        int owners{ 0 };
    };

    template<typename T, Core::AllocationStrategy strategy = Core::AllocationStrategy::Persistent>
    class Ref {
    public:
        Ref() : counter{ nullptr }, obj{ nullptr } {};
        Ref(RefData* counter, T* obj) : counter{ counter }, obj{ obj } {
            if (counter != nullptr)
                counter->owners += 1;
        }
        Ref(const Ref<T, strategy>& ref) {
            counter = ref.counter;
            obj = ref.obj;
            if (counter != nullptr)
                counter->owners += 1;
        }
        ~Ref() {
            if (counter) {
                counter->owners -= 1;
                if (counter->owners <= 0) {
                    Core::Destroy(obj);
                    Core::MemoryManager::Deallocate<strategy>(counter);
                }
            }
        }

        Ref<T, strategy>& operator=(Ref<T, strategy> rhs) {
            std::swap(this->counter, rhs.counter);
            std::swap(this->obj, rhs.obj);
            return *this;
        }

    public:
        inline T* Get() const {
            return obj;
        }

        inline RefData* GetRefData() const {
            return counter;
        }

        inline int GetOwnerCount() const {
            if (counter != nullptr)
                return counter->owners;
            else
                return 0;
        }

        inline T* operator->() const {
            return obj;
        }

        inline T& operator*() const {
            return *obj;
        }

        inline operator bool() {
            return counter && obj;
        }

        template<typename U>
        inline operator Ref<U, strategy>() {
            static_assert(std::is_base_of<U, T>::value);
            return Ref<U, strategy>{ this->counter, (U*)this->obj };
        }

    private:
        RefData* counter{ nullptr };
        T* obj{ nullptr };

        template<typename U, typename... Args,  Core::AllocationStrategy _strategy>
        friend Ref<U, _strategy> MakeRef(Args...);
    };

    template<typename T, typename... Args, Core::AllocationStrategy strategy = Core::AllocationStrategy::Persistent>
    inline Ref<T, strategy> MakeRef(Args... args) {
        Ref<T, strategy> ref{};
        ref.counter = (RefData*)Core::MemoryManager::Allocate<strategy>(sizeof(RefData) + sizeof(T)) ;
        ref.counter->owners = 1;
        ref.obj = (T*)(((char*)ref.counter) + sizeof(RefData));
        Core::Construct(ref.obj, args...);
        return std::move(ref);
    }

}

#endif //VIN_ENGINE_REF_H
