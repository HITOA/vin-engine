#ifndef VIN_ENGINE_FORMATTER_H
#define VIN_ENGINE_FORMATTER_H

#include <vin/core/templates/stdcontainers.h>

namespace Vin::Core::Logger {

    template<typename T>
    struct Formatter {
        static_assert("No formatter for specified type.");
    };

    template<>
    struct Formatter<const char*> {
        Core::StringView operator()(const char* str) {
            return str;
        }
    };

    template<>
    struct Formatter<Core::StringView> {
        Core::StringView operator()(Core::StringView str) {
            return str;
        }
    };

    template<>
    struct Formatter<Core::String> {
        Core::StringView operator()(const Core::String& str) {
            return str;
        }
    };

    template<>
    struct Formatter<int> {
        Core::StringView operator()(int v) {
            static char buff[33]{};
            memset(buff, 0, 33);
            sprintf(buff, "%i", v);
            return buff;
        }
    };

    template<>
    struct Formatter<float> {
        Core::StringView operator()(float v) {
            static char buff[33]{};
            memset(buff, 0, 33);
            sprintf(buff, "%f", v);
            return buff;
        }
    };

}

#endif //VIN_ENGINE_FORMATTER_H
