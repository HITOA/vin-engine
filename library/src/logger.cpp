#include <vin/core/logger/logger.h>

Vin::Vector<std::ostream*> Vin::Logger::outs{};
Vin::String Vin::Logger::currMsg{};

void Vin::Logger::AddLogOutput(std::ostream *output) {
    outs.push_back(output);
}

void Vin::Logger::Write(StringView str) {
    currMsg += str;
}

void Vin::Logger::Commit() {
    for (auto& outstream : outs) {
        *outstream << currMsg;
    }
    currMsg = "";
}