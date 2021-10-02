#ifndef BST_H
#define BST_H
#include <vector>

//Tree of directories, SLL of files in directory

using namespace std;
#include "string"

struct files
{
    string filename;
    string path;
    files *next = NULL;
};

struct directory;
struct nextDirectories
{
    directory *d;
};

struct directory
{
    string name;
    directory *previous = NULL;
    files *head = NULL;
    string path;

    //Vector of connected directories
    vector<nextDirectories> subdirectories;
};

//Tree of files
class BST
{
    public:
        BST();
        ~BST();

        //Print items in current directory
        void printDirectory();
        string getCurrentPath();
        string getCurrentFolder();
        bool printFileContent(string filename);
        void search(string name);

        void changeDirectory(string newdirectory);
        bool addDirectory(string foldername);
        void removeDirectory(string foldername);

        bool addFile(string filename);
        bool writeToFile(string filename, bool append);
        bool removeFile(string filename);
        int moveFile(string filename);
    private:
        directory *root;
        //Points to current folder/directory
        directory *currentPath;
};
#endif