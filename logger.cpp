#include "logger.h"
#include <time.h>


Logger* Logger::instance()
{
    if(_instance == nullptr)
    {
        _instance = new Logger();
    }

    return _instance;
}


Logger::Logger()
{
    fileStream.open("SurfaceLog.txt", std::ofstream::out | std::ofstream::app);
    if(!fileStream.is_open())
    {
        fileStream.open("SurfaceLog.txt", std::ofstream::out | std::ofstream::app);
    }
}

void Logger::WriteToLogFile(const char *string)
{
    time_t time = _CurrentTime();

    if(fileStream.is_open())
    {
        fileStream << "System Time: " << std::ctime(&time);
        fileStream << string << std::endl;
    }
}



Logger::~Logger()
{
    fileStream.close();
}


time_t Logger::_CurrentTime()
{
    auto timeNow = std::chrono::system_clock::now();

    return std::chrono::system_clock::to_time_t(timeNow);
}
