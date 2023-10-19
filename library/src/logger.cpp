#include <vin/core/logger/logger.h>

Vin::Vector<std::ostream*> Vin::Logger::outs{};

void Vin::Logger::AddLogOutput(std::ostream *output) {
    outs.push_back(output);
}

void Vin::Logger::Write(StringView str) {
    for (auto& outstream : outs) {
        *outstream << str;
    }
}