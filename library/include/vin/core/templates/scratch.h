#ifndef VIN_ENGINE_SCRATCH_H
#define VIN_ENGINE_SCRATCH_H

#include <vin/core/memory/memorymanager.h>
#include <stddef.h>
#include <stdint.h>

#define MIN_SCRATCH_BUFFER_SIZE 32

namespace Vin {

    template<Vin::Core::AllocationStrategy strategy = Vin::Core::AllocationStrategy::Persistent>
    class ScratchBuffer {
    public:
        ScratchBuffer() {
            buffer = (uint8_t*)Vin::Core::MemoryManager::GetInstance()->Allocate<strategy>(MIN_SCRATCH_BUFFER_SIZE);
            capacity = MIN_SCRATCH_BUFFER_SIZE;
        }

        ~ScratchBuffer() {
            Vin::Core::MemoryManager::GetInstance()->Deallocate<strategy>(buffer);
        }

        void Clear() {
            size = 0;
        }

        template<typename T>
        void Write(const T& v) {
            while (size + sizeof(T) >= capacity) {
                capacity *= 2;
                buffer = (uint8_t*)Vin::Core::MemoryManager::GetInstance()->Reallocate<strategy>(buffer, capacity);
            }
            memcpy(buffer + size, &v, sizeof(T));
            size += sizeof(T);
        }

        void Write(const uint8_t* data, size_t s) {
            while (size + s >= capacity) {
                capacity *= 2;
                buffer = (uint8_t*)Vin::Core::MemoryManager::GetInstance()->Reallocate<strategy>(buffer, capacity);
            }
            memcpy(buffer + size, data, s);
            size += s;
        }

        uint8_t* GetData() {
            return buffer;
        }

        size_t GetSize() {
            return size;
        }

    private:
        size_t size{ 0 };
        size_t capacity{ 0 };
        uint8_t* buffer{ nullptr };
    };

    class ScratchReader {
    public:
        ScratchReader() = delete;
        ScratchReader(uint8_t* buffer, size_t size) : buffer{ buffer }, size{ size } {};

        template<typename T>
        T Read() {
            T v = *(T*)(buffer + cursor);
            cursor += sizeof(T);
            return v;
        }

    private:
        size_t cursor{};
        size_t size{ 0 };
        uint8_t* buffer{ nullptr };
    };
}

#endif //VIN_ENGINE_SCRATCH_H
