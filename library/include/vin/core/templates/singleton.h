#ifndef VIN_ENGINE_SINGLETON_H
#define VIN_ENGINE_SINGLETON_H

namespace Vin::Core {

    template<typename T>
    class Singleton {
    protected:
        static T* singleton;

    public:
        static inline T* GetInstance() {
            if (singleton)
                return singleton;
            singleton = new T{};
            return singleton;
        }
    };

    template<typename T>
    T* Singleton<T>::singleton{ nullptr };

}

#endif //VIN_ENGINE_SINGLETON_H
