#include "BST.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdio.h>
using namespace std;
namespace fs = std::filesystem;

BST::BST()
{
    root  = new directory;
    root->head = NULL;
    root->path = "";
    root->name = "";
    currentPath = root;
}

//Deallocates all memory, starting from the furthest folder
void destructor(directory *n)
{
    if (!n)
        return;

    else
    {
        for (int i = 0; i < n->subdirectories.size(); i++)
        {
            if (n->subdirectories[i].d)
                destructor(n->subdirectories[i].d);
        }
    }

    //Remove all files from linked list if ll is not empty
    if (n->head)
    {
        files *temp = n->head;
        while (n->head)
        {
            cout << "deallocating file: " << n->head->filename << endl;
            n->head = n->head->next;
            delete temp;
            temp = n->head;
        }
    }

    //Deallocate folder
    if (n->name != "")
    {
        cout << "deallocating folder: " << n->name << endl;
        delete n;
    }
}

BST::~BST()
{
    //Set current path to main folder before deallocating all memory
    currentPath = root;
    destructor(root);
    root = NULL;
    currentPath = NULL;
}

void BST::printDirectory()
{
    cout << "Folders: ";
    for (int i = 0; i < currentPath->subdirectories.size(); i++)
    {
        cout << currentPath->subdirectories[i].d->name << "\t";
    }
    cout << "\nFiles: ";
    files *temp = currentPath->head;
    while (temp)
    {
        cout << temp->filename << "\t";
        temp = temp->next;
    }
    cout << endl;
}

string BST::getCurrentPath()
{
    return currentPath->path;
}

string BST::getCurrentFolder()
{
    return currentPath->name;
}

//Searches system for folder, if found sets currentPath
void changeDirectoryHelper(string newdirectory, directory *curr, directory *&path)
{
    //If directory is invalid
    if (!curr)
        return;

    //Set currentPath to new folder if found
    if (curr->name == newdirectory)
    {
        path = curr;
    }

    //Search all subdirectories depth first
    for (int i = 0; i < curr->subdirectories.size(); i++)
    {
        if (curr->subdirectories[i].d)
            changeDirectoryHelper(newdirectory, curr->subdirectories[i].d, path);
    }
}

//Change directory
void BST::changeDirectory(string newdirectory)
{
    //Check if they are in the directory they are searching for
    if (newdirectory == currentPath->name)
    {
        cout << "You are already in this folder." << endl;
        return;
    }

    //Set directory to main folder
    if (newdirectory == "root")
    {
        currentPath = root;
        return;
    }

    changeDirectoryHelper(newdirectory, root, currentPath);

    //Check if current directory has been changed
    if (currentPath->name == newdirectory)
        cout << "Current directory updated." << endl;
    else
        cout << "Directory not found" << endl;
}


//Returns true if folder is created
bool BST::addDirectory(string foldername)
{
    //Check if folder already exists
    for (int i = 0; i < currentPath->subdirectories.size(); i++)
    {
        if (foldername == currentPath->subdirectories[i].d->name)
        {
            return false;
        }
    }

    //Allocate memory for new directory and add to subdirectory vector
    nextDirectories newFolder;
    newFolder.d = new directory;
    newFolder.d->name = foldername;
    newFolder.d->previous = currentPath;
    newFolder.d->path = currentPath->path + foldername + "/";
    newFolder.d->head = NULL;
    currentPath->subdirectories.push_back(newFolder);

    //Create folder in system
    fs::create_directory(newFolder.d->path);

    return true;
}

//Returns true if new file is created
bool BST::addFile(string filename)
{
    //Pointer to linked list of files
    files *add = currentPath->head;

    //Check if there are files are in current directory
    if (!add)
    {
        //Allocate memory for a new file and add to directory linked list
        currentPath->head = new files;
        currentPath->head->filename = filename;
        currentPath->head->next = NULL;
        currentPath->head->path = currentPath->path + filename;

        //Create file in system
        ofstream output(currentPath->path + filename);

        return true;
    }

    //Check if file exists
    if (add->filename == filename)
    {
        return false;
    }

    //Add to directory linked list and check if file exists
    while (add->next)
    {
        if (add->filename == filename)
        {
            return false;
        }
        add = add->next;
    }

    //Allocate memory for new file
    add->next = new files;
    add->next->filename = filename;
    add->next->path = currentPath->path + filename;

    //Create file in new system
    ofstream output(currentPath->path + filename);

    return true;
}

//Depth first search through entire system
void searchHelper(string name, directory *curr, int &counter)
{
    //Directory is invalid
    if (!curr)
        return;
    
    //Folder found with the name
    if (curr->name == name)
    {
        counter++;
        cout << "Type: folder\t Folder name: " << name << endl;
    }

    //Check directory linked list for files
    else
    {
        files *temp = curr->head;
        while (temp)
        {
            if (temp->filename == name)
            {
                counter++;
                cout << "Type: file\t Location: " << curr->path << endl;
                break;
            }
            temp = temp->next;
        }
    }

    //Recursive call to search all folders
    for (int i = 0; i < curr->subdirectories.size(); i++)
    {
        if (curr->subdirectories[i].d)
            searchHelper(name, curr->subdirectories[i].d, counter);
    }
}

//Prints location of all folders or files with the same name
void BST::search(string name)
{
    int counter = 0;
    searchHelper(name, root, counter);
    cout << counter << " instances of \'" << name << "\'" << endl;
}

//Returns true if file is found
bool BST::printFileContent(string filename)
{
    //Try to read from file
    ifstream input(currentPath->path + filename);

    //Check is file is not opened
    if (!input)
        return false;

    //Print out contents of file
    string line;
    while (getline(input, line))
        cout << line << endl;

    return true;
}

//Returns true if file is found
bool BST::writeToFile(string filename, bool append)
{
    string word = "";
    
    //User wants to append to file
    if (append)
    {
        //Open file for writing
        ofstream output(currentPath->path + filename, ios::app);

        //Check if file is found
        if (!output)
            return false;

        //User writes to file here
        cout << "Enter 'DONE' to quit typing\n\n";
        while (word != "DONE")
        {
            cout << ">> ";
            if (getline(cin, word) && word.empty())
                getline(cin, word);
            if (word != "DONE")
                output << word << endl;
        }
    }

    //User wants to overwrite file
    else
    {
        //Open file for writing
        ofstream output(currentPath->path + filename);

        //Check if file is found
        if (!output)
            return false;

        //User writes to file here
        cout << "Enter 'DONE' to quit typing\n\n";
        while (word != "DONE")
        {
            cout << ">> ";
            if (getline(cin, word) && word.empty())
                getline(cin, word);
            if (word != "DONE")
                output << word << endl;
        }
    }
    return true;
}

//Depth first search of enire system
void rdhHelper(directory *n)
{
    if (!n)
        return;

    for (int i = 0; i < n->subdirectories.size(); i++)
    {
        if (n->subdirectories[i].d)
            rdhHelper(n->subdirectories[i].d);
    }

    //Deallocate memory for files in directory linked list
    if (n->head)
    {
        files *del = n->head;
        while (n->head)
        {
            cout << "deleting: " << del->filename << endl;
            n->head = n->head->next;
            delete del;
            del = n->head;
        }
    }

    //Delete folder at end of recursive call
    cout << "deleting folder: " << n->name << endl;
    delete n;
    n = NULL;
}

//Searches for folder, deletes all files/folders from system and deallocates memory
void removeDirectoryHelper(string foldername, directory *n)
{
    //Folder invalid
    if (!n)
    {
        cout << "Folder not found." << endl;
        return;
    }

    //Folder found
    if (n->name == foldername)
    {
        //Remove all files and folders from system
        fs::remove_all(n->path);

        //Deallocate memory for all subsequent files and folders
        rdhHelper(n);

        //Clear subdirectory vector
        if (!n->subdirectories.empty())
            n->subdirectories.clear();
    }

    //Recursive call to search entire system
    for (int i = 0; i < n->subdirectories.size(); i++)
    {
        if (n->subdirectories[i].d)
            removeDirectoryHelper(foldername, n->subdirectories[i].d);
    }
}

void BST::removeDirectory(string foldername)
{
    //Set directory to root before removing anything
    cout << "Setting current directory to root..." << endl;
    currentPath = root;

    removeDirectoryHelper(foldername, root);
}

//Returns true if file exists
bool BST::removeFile(string filename)
{
    //Check if any files exists in current directory
    if (!currentPath->head)
        return false;

    //Two pointers to continue linked list chain
    files *curr = currentPath->head;
    files *prev = NULL;

    //Check if file is head of linked list
    if (curr->filename == filename)
    {
        //Set new head of linked list
        currentPath->head = currentPath->head->next;

        //Remove file from system
        fs::remove(curr->path);

        //Deallocate memory
        delete curr;
        curr = NULL;
        return true;
    }

    //Continue search for file
    prev = curr;
    curr = curr->next;
    while (curr)
    {
        if (curr->filename == filename)
            break;
        prev = curr;
        curr = curr->next;
    }

    if (!curr)
        return false;
    
    //Skip file in linked list before deallocating
    prev->next = curr->next;
    //Remove file from system
    fs::remove(curr->path);
    delete curr;
    curr = NULL;
    return true;
}

//Searches entire system for folder
void searchForFolder(directory *n, string foldername, directory *&ptr)
{
    if (!n)
        return;

    //If folder is found, set folderptr
    if (n->name == foldername)
    {
        ptr = n;
        return;
    }

    //Depth first search
    for (int i = 0; i < n->subdirectories.size(); i++)
    {
        if (n->subdirectories[i].d)
            searchForFolder(n->subdirectories[i].d, foldername, ptr);
    }
}

int BST::moveFile(string filename)
{
    //Current directory empty
    if (!currentPath->head)
    {
        cout << "No files exist in current directory." << endl;
        return 0;
    }

    //Search for file
    files *file = currentPath->head;
    while (file)
    {
        if (file->filename == filename)
            break;
        file = file->next;
    }

    //File not found
    if (!file)
        return 1;

    //Find new folder
    string foldername;
    directory *folderptr;
    cout << "Enter the name of the new folder: ";
    cin >> foldername;
    searchForFolder(root, foldername, folderptr);
    if (!folderptr)
    {
        cout << "Folder not found." << endl;
        return -1;
    }

    //Copy file to new folder
    string line;
    ifstream input(file->path);
    ofstream output(folderptr->path + filename);
    while (getline(input, line))
    {
        output << line << endl;
    }

    //Remove file from system
    fs::remove(file->path);

    //Deallocate memory for file
    files *curr = currentPath->head;
    files *prev = NULL;
    if (curr->filename == filename)
    {
        currentPath->head = currentPath->head->next;
        delete curr;
        curr = NULL;
    }
    else
    {
        while (curr->filename != filename)
        {
            prev = curr;
            curr = curr->next;
        }
        prev->next = curr->next;
        delete curr;
        curr = NULL;
    }

    //Add new file to the new folders linked list of files
    curr = folderptr->head;
    prev = NULL;
    if (!folderptr->head)
    {
        folderptr->head = new files;
        folderptr->head->filename = filename;
        folderptr->head->path = folderptr->path + filename;
    }
    else
    {
        files *newFile = new files;
        newFile->filename = filename;
        newFile->path = folderptr->path + filename;
        while (curr->next)
        {
            curr = curr->next;
        }   
        curr->next = newFile;
        newFile->next = NULL;
    }
    return 3;
}