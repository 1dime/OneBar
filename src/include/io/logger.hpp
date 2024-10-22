#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <vars/vars.hpp>

enum LogLevel
{
    WARNING = 0,
    ERROR = 1
};

#define warn(message) logger::log(message, __FUNCTION__, WARNING);
#define error(message) logger::log(message, __FUNCTION__, ERROR);
#define concat(x, y)({ \
    size_t xSize = strlen(x);\
    size_t ySize = strlen(y);\
    char *result = (char *) std::malloc(xSize + ySize + 1);\
    strcpy(result, x); \
    strcat(result, y); \
    result; \
})

class logger
{
    public:
        static void log(const char *message, const char *callingFunction, LogLevel level);
};
#endif