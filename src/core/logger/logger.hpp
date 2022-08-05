#pragma once

#include <vinpch.hpp>

#include <fmt/core.h>

#include "vectorfmt.hpp"

namespace Vin {

    class Logger {
    public:
        static void AddLogOutputStream(std::ostream* out);

        static void RemoveLogOutputStream(std::ostream* out);

        static void AddDefaultLogOutputStream();

        static void Write(const char* msg);
        static void WriteHeader(const char* header, const char* msg);
        
        template<typename... Args>
        static inline void Log(const char* format, Args... args) {
            WriteHeader("Info", fmt::format(format, args...).data());
        }

        template<typename... Args>
        static inline void Warn(const char* format, Args... args) {
            WriteHeader("Warning", fmt::format(format, args...).data());
        }

        template<typename... Args>
        static inline void Err(const char* format, Args... args) {
            WriteHeader("ERROR", fmt::format(format, args...).data());
        }
    private:
        static std::vector<std::ostream*> m_LogOutputs;
    };
}
