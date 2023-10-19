#ifndef VIN_ENGINE_FORMATTER_H
#define VIN_ENGINE_FORMATTER_H

#include <vin/core/templates/stdcontainers.h>

namespace Vin::Core {

    template<typename T>
    struct Formatter {
        static_assert("No formatter for specified type.");
    };

    template<>
    struct Formatter<const char*> {
        StringView operator()(const char* str) {
            return str;
        }
    };

    template<>
    struct Formatter<StringView> {
        StringView operator()(StringView str) {
            return str;
        }
    };

    template<>
    struct Formatter<String> {
        StringView operator()(const String& str) {
            return str;
        }
    };

    template<>
    struct Formatter<int> {
        StringView operator()(int v) {
            static char buff[33]{};
            memset(buff, 0, 33);
            sprintf(buff, "%i", v);
            return buff;
        }
    };

    template<>
    struct Formatter<float> {
        StringView operator()(float v) {
            static char buff[33]{};
            memset(buff, 0, 33);
            sprintf(buff, "%f", v);
            return buff;
        }
    };

}

#endif //VIN_ENGINE_FORMATTER_H
