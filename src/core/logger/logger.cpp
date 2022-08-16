#include "logger.hpp"

eastl::fixed_vector<std::ostream*, 8, true> Vin::Logger::m_LogOutputs{};

void Vin::Logger::AddLogOutputStream(std::ostream* out)
{
	if (out != nullptr)
		m_LogOutputs.push_back(out);
}

void Vin::Logger::RemoveLogOutputStream(std::ostream* out)
{
    if (m_LogOutputs.size() < 1)
        return;
    auto it = std::find(m_LogOutputs.begin(), m_LogOutputs.end(), out);
    if (it == m_LogOutputs.end())
        return;
    m_LogOutputs.erase(it);
}

void Vin::Logger::AddDefaultLogOutputStream()
{
    m_LogOutputs.push_back(&std::cout);
}

void Vin::Logger::Write(const char* msg)
{
    for (auto& oLogStream : m_LogOutputs) {
        *oLogStream << msg;
    }
}

void Vin::Logger::WriteHeader(const char* header, const char* msg)
{
    time_t t = time(NULL);
    tm* tm = localtime(&t);
    Write(fmt::format("[{}][{}:{}:{}] : {}\n", header, tm->tm_hour, tm->tm_min, tm->tm_sec, msg).data());
}
