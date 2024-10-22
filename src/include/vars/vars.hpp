#ifndef _VARS_HPP_
#define _VARS_HPP_
#include <config/config.hpp>

class vars
{
    public:
        static config *configuration;
};

//Define static variables here
config *vars::configuration = NULL;

#endif