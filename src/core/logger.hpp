#pragma once

#include <vinpch.hpp>

namespace Vin {

    class Logger {
    public:
        static void AddLogOutputStream(std::ostream* out);

        static void RemoveLogOutputStream(std::ostream* out);

        static void Write(const char* msg);
        static void LogI(const char* msg);
        static void WarnI(const char* msg);
        static void ErrI(const char* msg);
        
        template<typename... Args>
        static inline void Log(const char* format, Args... args) {
            char* buff = (char*)malloc(512);
            if (buff == nullptr)
                return;
            sprintf(buff, format, args...);
            LogI(buff);
            free(buff);
        }

        template<typename... Args>
        static inline void Warn(const char* format, Args... args) {
            char* buff = (char*)malloc(512);
            if (buff == nullptr)
                return;
            sprintf(buff, format, args...);
            WarnI(buff);
            free(buff);
        }

        template<typename... Args>
        static inline void Err(const char* format, Args... args) {
            char* buff = (char*)malloc(512);
            if (buff == nullptr)
                return;
            sprintf(buff, format, args...);
            ErrI(buff);
            free(buff);
        }
    private:
        static std::vector<std::ostream*> m_LogOutputs;
    };
}
