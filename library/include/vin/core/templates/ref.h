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
        template<typename... Args>
        explicit Ref(Args... args) {
            Memory::MemoryManager* memoryManager = Memory::MemoryManager::GetInstance();
            counter = (RefData*)memoryManager->Allocate<strategy>(sizeof(RefData) + sizeof(T));
            counter->owners = 1;
            obj = (T*)((char*)counter + sizeof(RefData));
            Memory::Construct(obj, args...);
        }
        template<typename U>
        Ref(const Ref<U, strategy>& ref) {
            static_assert(std::is_base_of<T, U>::value);
            obj = (T*)ref.Get();
            counter = (RefData*)((char*)obj - sizeof(RefData));
            ++counter->owners;
        }
        Ref(Ref<T, strategy>& ref) {
            counter = ref.counter;
            obj = ref.obj;
            ++counter->owners;
        }
        ~Ref() {
            --counter->owners;
            if (counter->owners <= 0) {
                Memory::Destroy(obj);
                Memory::MemoryManager* memoryManager = Memory::MemoryManager::GetInstance();
                memoryManager->Deallocate<strategy>(counter);
            }
        }

    public:
        inline T* Get() const {
            return obj;
        }

        inline int GetOwnerCount() const {
            return counter->owners;
        }

        inline T* operator->() const {
            return obj;
        }

        inline T& operator*() const {
            return *obj;
        }

    protected:
        RefData* counter{ nullptr };
        T* obj{ nullptr };
    };

}

#endif //VIN_ENGINE_REF_H
