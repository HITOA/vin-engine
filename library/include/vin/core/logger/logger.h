#ifndef VIN_ENGINE_LOGGER_H
#define VIN_ENGINE_LOGGER_H

#include <vin/core/templates/stdcontainers.h>
#include <ostream>
#include <vin/core/logger/formatter.h>

namespace Vin {

    class Logger {
    public:
        static void AddLogOutput(std::ostream* output);
        static void Write(StringView str);
        static void Commit();

        template<typename Arg>
        static inline void Print(Arg arg) {
            Write(Core::Formatter<Arg>{}(arg));
        }

        template<typename Arg, typename... Args>
        static inline void Print(Arg arg, Args... args) {
            Print(arg);
            Print(args...);
        }

        template<typename... Args>
        static inline void Log(Args... args) {
            time_t t = time(NULL);
            tm* tm = localtime(&t);
            Print("[Log][", tm->tm_hour, ":", tm->tm_min, ":", tm->tm_sec, "] : ", args..., "\n");
            Commit();
        }

        template<typename... Args>
        static inline void Warn(Args... args) {
            time_t t = time(NULL);
            tm* tm = localtime(&t);
            Print("[Warning][", tm->tm_hour, ":", tm->tm_min, ":", tm->tm_sec, "] : ", args..., "\n");
            Commit();
        }

        template<typename... Args>
        static inline void Err(Args... args) {
            time_t t = time(NULL);
            tm* tm = localtime(&t);
            Print("[Error][", tm->tm_hour, ":", tm->tm_min, ":", tm->tm_sec, "] : ", args..., "\n");
            Commit();
        }
    private:
        static Vector<std::ostream*> outs;
        static String currMsg;
    };

}

#endif //VIN_ENGINE_LOGGER_H
