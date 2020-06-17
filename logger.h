#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <chrono>
#include <ctime>

class Logger
{
    public:
    static Logger* instance();

    void WriteToLogFile(const char* string);

    private:
    static Logger* _instance;

    std::ofstream fileStream;

    Logger();
    Logger(Logger* loger);
    ~Logger();

    time_t _CurrentTime();
};



#endif // LOGGER_H
