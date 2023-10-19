#ifndef VIN_ENGINE_EVENT_H
#define VIN_ENGINE_EVENT_H

#include <vin/core/templates/stdcontainers.h>
#include <functional>

namespace Vin {

    template<typename F>
    class Event {
        static_assert("Event return type can't be anything else than void.");
    };

    template<typename... Args>
    class Event<void(Args...)> {
    public:
        using Signature = void(Args...);

    public:
        inline void Invoke(Args... args) {
            for (auto& callback : callbacks)
                callback(args...);
        }

        inline void operator()(Args... args) {
            for (auto& callback : callbacks)
                callback(args...);
        }

        inline void operator+=(const std::function<Signature>& callback) {
            callbacks.emplace_back(callback);
        }

        inline void operator-=(const std::function<Signature>& callback) {
            auto it = std::find(callbacks.begin(), callbacks.end(), callback);
            if (it != callbacks.end())
                callbacks.erase(callbacks.begin() + it);
        }

    private:
        Vector<std::function<Signature>> callbacks{};
    };

}

#endif //VIN_ENGINE_EVENT_H
