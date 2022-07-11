#include <logger.hpp>
#include <vector>
#include <format>
#include <string>
#include <chrono>

namespace Vin::Logger {
    namespace {
        std::vector<std::ostream*> _loggerOuts{};
        LoggerFormat _format{};
    }

    std::string GetCurrentFormatedTime() {
        std::chrono::time_point tp = std::chrono::system_clock::now();
        time_t ctt = std::chrono::system_clock::to_time_t(tp);
        tm ctm{};
        localtime_s(&ctm, &ctt);
        return std::format("{}:{}:{}", ctm.tm_hour, ctm.tm_min, ctm.tm_sec);
    }

    void SetLoggerFormat(LoggerFormat format) {
        _format = format;
    }

    void AddLogOutputStream(std::ostream* out) {
        _loggerOuts.push_back(out);
    }

    void RemoveLogOutputStream(std::ostream* out) {
        if (_loggerOuts.size() < 1)
            return;
        auto it = std::find(_loggerOuts.begin(), _loggerOuts.end(), out);
        if (it == _loggerOuts.end())
            return;
        _loggerOuts.erase(it);
    }

    void Write(std::string_view msg) {
        for (auto& oLogStream : _loggerOuts) {
            oLogStream->write(msg.data(), msg.size());
        }
    }

    void Log(std::string_view msg) {
        Write(std::format(_format.logFormat, msg, GetCurrentFormatedTime()));
    }

    void Warn(std::string_view msg) {
        Write(std::format(_format.warnFormat, msg, GetCurrentFormatedTime()));
    }

    void Err(std::string_view msg) {
        Write(std::format(_format.errFormat, msg, GetCurrentFormatedTime()));
    }
}