#include <vin/core/logger/logger.h>

Vin::Core::Vector<std::ostream*> Vin::Core::Logger::Logger::outs{};

void Vin::Core::Logger::Logger::AddLogOutput(std::ostream *output) {
    outs.push_back(output);
}

void Vin::Core::Logger::Logger::Write(Core::StringView str) {
    for (auto& outstream : outs) {
        *outstream << str;
    }
}