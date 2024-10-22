#include <io/file.hpp>

/*
file_node: creates a new file node if path is not empty
@param path: path of file / folder
*/
file_node::file_node(std::string *nodePath)
{
    //Creates a new file node
    //Create a filesystem path and error code to determine if is file
    filesystem::path fsPath(nodePath->c_str());
    error_code code;
    //Check if the path is set
    if(!nodePath->empty())
    {
        //It is, so initialize the node variable
        this->node = (node_t *) std::malloc(sizeof(node_t));
        //As well as node variable's variables
        this->node->path = nodePath;
        this->node->isAFile = !filesystem::is_directory(fsPath, code);
    }
}

/*
file_node: creates a new file node if path is not empty
@param path: path of file / folder
*/
file_node::file_node(const char *nodePath)
{
    //Create a filesystem path and error code to determine if is file
    filesystem::path fsPath(nodePath);
    error_code code;
    //Check if the path is set
    if(nodePath != NULL)
    {
        //It is, so initialize the node variable
        this->node = (node_t *) std::malloc(sizeof(node_t));
        //As well as node variable's variables
        this->node->path = new std::string(nodePath);
        this->node->isAFile = !filesystem::is_directory(fsPath, code);
    }
}

/*
getPath: gets path of file
@returns file path
*/
std::string *file_node::getPath()
{
    return this->node->path;
}

/*
isFile: gets if node is a file or not
@returns if node is file
*/
bool file_node::isFile()
{
    return this->node->isAFile;
}

/*
isOpen: gets if node is still open
@returns if node is still open
*/
bool file_node::isOpen()
{
    return ((this->readFile) || (this->writeFile))
        && ((this->readFile.is_open()) || (this->writeFile.is_open()))
        || this->node->isOpen;
}

/*
exists: checks if node exists on storage medium
@returns if node exists
*/
bool file_node::exists()
{
    //Return if node exists on storage
    std::ifstream fileStream(this->getPath()->c_str());
    bool exists = fileStream.good();
    fileStream.close();
    return exists;
}

/*
getSize: gets size of file node
@returns file size
*/
int file_node::getSize()
{
    //Return the file size
    return filesystem::file_size(this->node->path->c_str());
}

/*
addNode: adds new node to node list
@param newNode: node to be added
*/
void file_node::addNode(file_node *newNode)
{
    //Adds node to list
    newNode->node->previous = this->node;
    this->node->next = newNode->node;
    this->node = newNode->node;
}

/*
addNode: adds new node to node list
@param newNode: node to be added
*/
void file_node::addNode(node_t *newNode)
{
    //Adds node to list
    newNode->previous = this->node;
    this->node->next = newNode;
    this->node = newNode;
}

/*
previous: gets previous node in list
@returns previous node
*/
node_t *file_node::previous()
{
    //Gets previous file if this has not been ran, sets previous file to its previous if it has
    if(this->previousRan)
    {
        this->node = this->node->previous;
    }
    this->previousRan = true;
    return this->node->previous;
}

/*
next: gets next node in list
@returns next node
*/
node_t *file_node::next()
{
    //Gets next node if this has not been ran, sets next node to its next if it has
    if(this->nextRan)
    {
        this->node = this->node->next;
    }
    this->nextRan = true;
    return this->node;
}

/*
first: gets first node in list
@returns first node
*/
node_t *file_node::first()
{
    //Gets first node in list
    while(this->node->previous != NULL)
    {
        this->node = this->node->previous;
    }

    return this->node;
}

/*
last: gets final node in list
@returns final node
*/
node_t *file_node::last()
{
    //Gets last node in list
    while(this->node->next != NULL)
    {
        this->node = this->node->next;
    }
    return this->node;
}

/*
listItems: gets a list of all files in a directory
@returns list of files
*/
file_node *file_node::listItems()
{
    //Returns list of all items in a path, if it is a directory
    if(!this->isFile())
    {
        for(const auto &entry : filesystem::recursive_directory_iterator(this->getPath()->c_str()))
        {
            this->addNode(new file_node((char *) entry.path().c_str()));
        }
    }

    return this;
}

/*
find: searches for a given node in entire tree
@param path: path of directory / file node is associated with
*/
file_node *file_node::find(const char *path)
{
    while(this->previous())
    {
        if(this->getPath()->compare(path) == 0)
        {
            return this;
        }
    }

    return NULL;
}

/*
read: returns content if node is a file, list of files if not
@returns content or list of files
*/
void *file_node::read()
{
    //Reads content from file, if it is a file. Otherwise, returns list of files
    void *content = NULL;
    if(this->isFile())
    {
        content = (void *) std::malloc(this->getSize());
        if(this->isOpen())
        {
            warn(concat(this->getPath()->c_str(), " is open."));
            return NULL;
        }

        this->readFile.open(this->getPath()->c_str());
        std::vector<char> buffer;
        buffer.resize(this->getSize());
        this->readFile.read(buffer.data(), this->getSize());
        std::memcpy((char *) content, buffer.data(), this->getSize());
    }
    else
    {
        content = (void *) this->listItems();
    }

    return content;
}

/*
write: writes data to a file
@param data: data to be written
*/
void file_node::write(auto data)
{
    //Writes content to file, if it is a file
    if(this->isFile())
    {
        //Write to the file if possible
        if(this->isOpen())
        {
            warn(concat(this->getPath()->c_str(), " is already open."));
        }

        this->writeFile.open(this->getPath()->c_str(), ios::binary);
        this->writeFile << data;
        this->writeFile.flush();
    }
    else
    {
        warn(concat(this->getPath()->c_str(), " is a directory."));
    }
}

/*
close: closes file if it is open
*/
void file_node::close()
{
    //Closes file if it is open
    if(this->isOpen())
    {
        if(this->readFile.is_open())
        {
            this->readFile.close();
        }

        if(this->writeFile.is_open())
        {
            this->writeFile.close();
        }

        this->node->isOpen = true;
    }
}