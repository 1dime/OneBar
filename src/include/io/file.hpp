#ifndef _FILE_HPP_
#define _FILE_HPP_

#include <io/logger.hpp>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

typedef struct node node_t;
struct node
{
    bool isAFile = true;
    bool isOpen = false;
    std::string *path;
    node_t *next;
    node_t *previous;
};

class file_node
{
    private:
        std::ifstream readFile;
        std::ofstream writeFile;
        bool previousRan = false;
        bool nextRan = false;
        node_t *node = NULL;
    
    public:
        file_node(std::string *path);
        file_node(const char *path);
        std::string *getPath();
        bool isFile();
        bool isOpen();
        bool exists();
        int getSize();
        void addNode(file_node *newNode);
        void addNode(node_t *newNode);
        node_t *previous();
        node_t *next();
        node_t *first();
        node_t *last();
        file_node *listItems();
        file_node *find(const char *path);
        void *read();
        void write(auto data);
        void close();
};
#endif