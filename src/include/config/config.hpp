#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#define LOGGING_ENABLED_CONFIG "logging_enabled"

#define DEFAULT_CONFIG "./onebar_config.json"
#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 1
#define PLATFORM_UNSUPPORTED -1
//Determine the platform
#ifdef _WIN32
#define PLATFORM PLATFORM_WINDOWS //Windows
#elif linux
#define PLATFORM PLATFORM_LINUX //Linux
#else
#define PLATFORM PLATFORM_UNSUPPORTED //Unsupported
#endif

#include <any>
#include <cstdlib>
#include <io/file.hpp>
#include <io/logger.hpp>
#include <iostream>
#include <list>
#include <cstring>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;
class config
{
    private:
        file_node *configFile = NULL;
        std::list<char *> keys;
        json configuration = NULL;
        json parseConfiguration();
    public:
        config(const char *cfgName);
        bool readConfiguration(json configurationData);
        json getConfiguration();
        std::list<char *> getKeys();
        void append(const char *key, auto value);
        json get(const char *key);
        bool apply();
        void close();
};
#endif