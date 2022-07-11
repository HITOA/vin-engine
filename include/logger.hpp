#pragma once
#include <ostream>
#include <string_view>
#include <format>

namespace Vin::Logger {
    
    struct LoggerFormat {
        std::string_view logFormat  = "[Info][{1}] : {0}\n";
        std::string_view warnFormat = "[Warning][{1}] : {0}\n";
        std::string_view errFormat  = "[ERROR][{1}] : {0}\n";
    };

    void SetLoggerFormat(LoggerFormat info);
    /**
        * \brief Add output stream to the logger output
        * \param out ptr to the output stream
    */
    void AddLogOutputStream(std::ostream* out);
    /**
        * \brief Remove output stream from the logger output
        * \param out ptr to the output stream
    */
    void RemoveLogOutputStream(std::ostream* out);
    /**
        * \brief Write raw string to the logger output
        * \param msg msg to be writed
    */
    void Write(std::string_view msg);
    /**
        * \brief Write Log formated msg to the logger output
        * \param msg msg to be writed
    */
    void Log(std::string_view msg);
    /**
        * \brief Write Warning formated msg to the logger output
        * \param msg msg to be writed
    */
    void Warn(std::string_view msg);
    /**
        * \brief Write Error formated msg to the logger output
        * \param msg msg to be writed
    */
    void Err(std::string_view msg);

#if __cpp_lib_format
    template<class... Args>
    void Log(std::string_view ufmt, Args&&... args) {
        Log(std::vformat(ufmt, std::make_format_args(args...)));
    }

    template<class... Args>
    void Warn(std::string_view ufmt, Args&&... args) {
        Warn(std::vformat(ufmt, std::make_format_args(args...)));
    }

    template<class... Args>
    void Err(std::string_view ufmt, Args&&... args) {
        Err(std::vformat(ufmt, std::make_format_args(args...)));
    }
#endif
}