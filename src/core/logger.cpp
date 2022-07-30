#include "logger.hpp"

std::vector<std::ostream*> Vin::Logger::m_LogOutputs = std::vector<std::ostream*>{};

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

void Vin::Logger::Write(const char* msg)
{
    for (auto& oLogStream : m_LogOutputs) {
        *oLogStream << msg;
    }
}

void Vin::Logger::LogI(const char* msg)
{
    static const char* format = "[Info][%02d:%02d:%02d] : %s\n";
    time_t t = time(NULL);
    tm* tm = localtime(&t);
    size_t buffsize = 512;
    char* buff = (char*)malloc(buffsize);
    if (buff == nullptr)
        return;
    sprintf(buff, format, tm->tm_hour, tm->tm_min, tm->tm_sec, msg);
    Write(buff);
    free(buff);
}

void Vin::Logger::WarnI(const char* msg)
{
    static const char* format = "[Warning][%02d:%02d:%02d] : %s\n";
    time_t t = time(NULL);
    tm* tm = localtime(&t);
    size_t buffsize = 512;
    char* buff = (char*)malloc(buffsize);
    if (buff == nullptr)
        return;
    sprintf(buff, format, tm->tm_hour, tm->tm_min, tm->tm_sec, msg);
    Write(buff);
    free(buff);
}

void Vin::Logger::ErrI(const char* msg)
{
    static const char* format = "[ERROR][%02d:%02d:%02d] : %s\n";
    time_t t = time(NULL);
    tm* tm = localtime(&t);
    size_t buffsize = 512;
    char* buff = (char*)malloc(buffsize);
    if (buff == nullptr)
        return;
    sprintf(buff, format, tm->tm_hour, tm->tm_min, tm->tm_sec, msg);
    Write(buff);
    free(buff);
}
