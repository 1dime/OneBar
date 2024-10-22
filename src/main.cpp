#include "io/logger.cpp"
#include <vars/vars.hpp>
#include "io/file.cpp"
#include "config/config.cpp"

//All paths in configuration file can be arrays too
int main(int argc, char **argv)
{
    //Get configuration file, either passed or default
    vars::configuration = NULL;
    if(argc > 1 && (std::strcmp(argv[1], "") != 0))
    {
        //Load passed configuration file
        vars::configuration = new config(argv[1]);
    }
    else
    {
        //Load default configuration file
        vars::configuration = new config(DEFAULT_CONFIG);
    }

    GLFWindow *window = glfwCreateWindow(100, 100, "Learn", NULL, NULL);
}
