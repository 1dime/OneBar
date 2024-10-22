#include <config/config.hpp>

json config::parseConfiguration()
{
    //Read configuration data from configuration file
    void *configuration = (void *) this->configFile->read();
    if(configuration != NULL)
    {
        char *configurationData = (char *) static_cast<char *>(configuration);
        return json::parse(configurationData);
    }
    
    warn("could not read configuration.");
    return NULL;
}

bool config::readConfiguration(json configurationData)
{
    //Read the entire configuration data, ignoring objects
    for(const auto &[k, v] : configurationData.items())
    {
        json element = configurationData.at(k);
        if(element.is_object())
        {
            return this->readConfiguration(element);
        }
        else
        {
            keys.push_back((char *) k.c_str());
            this->configuration[k] = v;
        }
    }
    return true;
}

config::config(const char *cfgName)
{
    //Reads JSON configuration from config file
    char *configFilePath = NULL;
    //Get the configuration path stored in user's personal directory
    switch(PLATFORM)
    {
        case PLATFORM_WINDOWS:
            configFilePath = std::getenv("USERPROFILE");
            std::strcat(configFilePath, "\\");
            break;
        case PLATFORM_LINUX:
            configFilePath = std::getenv("HOME");
            std::strcat(configFilePath, "/");
            break;
        case PLATFORM_UNSUPPORTED:
            error("unsupported platform.");
            exit(EXIT_FAILURE);
            break;
    }

    //And open configuration
    this->configFile = new file_node(cfgName);
    std::strcat(configFilePath, cfgName);
    //Now, check if the configuration file doesnt exist
    if(!this->configFile->exists())
    {
        //Try to find the configuration file in user's personal directory
        this->configFile = new file_node(configFilePath);
        //Now, check if the configuration file doesnt exist
        if(!this->configFile->exists())
        {
            //Try to use default configuration
            this->configFile = new file_node(DEFAULT_CONFIG);
            if(!this->configFile->exists())
            {
                //Could not find a configuration file
                error("failed to open configuration file");
                exit(EXIT_FAILURE);
            }
        }
    }

    //Now, read the configuration data
    this->configuration = json::object();
    json tempconfiguration = this->parseConfiguration();
    this->readConfiguration(tempconfiguration);
}


json config::getConfiguration()
{
    //Returns the configuration
    return this->configuration;
}

std::list<char *> config::getKeys()
{
    //Returns all keys
    return this->keys;
}

void config::append(const char *key, auto value)
{
    //Add value and key to configuration JSON
    this->configuration[key] = value;
}

json config::get(const char *key)
{
    //Returns configuration data based on provided key
    return this->configuration[key];
}


bool config::apply()
{
    //Saves configuration file
    if(this->configFile->isOpen())
    {
        //Configuration file was successfully opened, write configuration to it
        this->configFile->write(this->getConfiguration());
        return true;
    }

    return false;
}

void config::close()
{
    //Closes configuration file
    if(this->configFile->isOpen())
    {
        //Close the configuration file as it is open
        this->configFile->close();
    }
}