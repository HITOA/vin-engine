#ifndef VIN_ENGINE_REF_H
#define VIN_ENGINE_REF_H

#include <stddef.h>
#include <vin/core/memory/memorymanager.h>

namespace Vin::Core {

    struct RefData {
        int owners{ 0 };
    };

    template<typename T, Memory::Strategy strategy = Memory::Strategy::Persistent>
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
                    Memory::Destroy(obj);
                    Memory::MemoryManager::Deallocate<strategy>(counter);
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

        template<typename U, typename... Args,  Memory::Strategy _strategy>
        friend Ref<U, _strategy> MakeRef(Args...);
    };

    template<typename T, typename... Args, Memory::Strategy strategy = Memory::Strategy::Persistent>
    inline Ref<T, strategy> MakeRef(Args... args) {
        Ref<T, strategy> ref{};
        ref.counter = (RefData*)Memory::MemoryManager::Allocate<strategy>(sizeof(RefData) + sizeof(T)) ;
        ref.counter->owners = 1;
        ref.obj = (T*)(((char*)ref.counter) + sizeof(RefData));
        Memory::Construct(ref.obj, args...);
        return std::move(ref);
    }

}

#endif //VIN_ENGINE_REF_H
