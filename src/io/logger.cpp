#include <io/logger.hpp>

void logger::log(const char *message, const char *functionName, LogLevel level)
{
    //Prints a message with respect to log level and function name if logging is enabled
    char *formattedMessage = (char *) "";
    if(level == WARNING)
    {
        formattedMessage =(char *) "warning: ";
    }
    else
    {
        formattedMessage =(char *) "error: ";
    }

    if(vars::configuration != NULL)
    {
        if(vars::configuration->get(LOGGING_ENABLED_CONFIG))
        {
            std::cout << formattedMessage << functionName << "(): " << message << std::endl;
        }
    }
}