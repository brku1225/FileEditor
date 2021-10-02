#include <iostream>
#include <cctype>
#include <iomanip>
#include "BST.hpp"
using namespace std;

int main()
{
    cout << "Welcome to file editor!\nPlease make sure no files or directories exist in this folder." << endl;

    //Initialize with current directory
    BST fileEditor;

    string filename;
    string input;
    int option = 0;
    while (true)
    {
        cout << "What would you like to do? (Enter the number)" << endl;
        cout << "Folder                                 File                    Other" << endl;
        cout << "---------------------------            --------------          ----------------------------" << endl;
        cout << "1. Add directory                       5. Add file             10. Search for file or folder\n" << endl;
        cout << "2. Change current directory            6. Edit file            11. Print current path\n" << endl;
        cout << "3. Delete folder                       7. Remove File          12. Quit\n" << endl;
        cout << "4. Print items in current directory    8. Move file\n" << endl;
        cout << "                                       9. Print file content" << endl;
        cin >> input;

        for (int i = 0; i < input.length(); i++)
        {
            if (!isdigit(input[i]))
            {
                option = 0;
                break;
            }
            else
                option = option * 10 + input[i] - 48;
        }
        
        switch (option)
        {
            case 1:
                cout << "Enter name of new folder (one word): ";
                cin >> filename;
                if (fileEditor.addDirectory(filename))
                    cout << "Folder created." << endl;
                else
                    cout << "This folder is already in this directory." << endl;
                break;
            case 2:
                cout << "Which folder are you looking for? (enter 'root' to go to main folder)" << endl;
                cin >> filename;
                fileEditor.changeDirectory(filename);
                break;
            case 3:
                cout << "Enter the name of the folder: ";
                cin >> filename;
                fileEditor.removeDirectory(filename);
                break;
            case 4:
                fileEditor.printDirectory();
                break;
            case 5:
                cout << "Enter name of the new file (one word): ";
                cin >> filename;
                if (fileEditor.addFile(filename))
                    cout << "File successfully created" << endl;
                else
                    cout << "File already exists" << endl;
                break;
            case 6:
                cout << "What would you like to do?" << endl;
                while (input != "1" || input != "2")
                {
                    option = 0;
                    cout << "1. Append to file\n2. Overwrite file" << endl;
                    cin >> input;
                    if (input == "1")
                    {
                        cout << "Enter the name of the file:  ";
                        cin >> filename;
                        if (!fileEditor.writeToFile(filename, true))
                            cout << "File not in current directory." << endl;
                        break;
                    }
                    else if (input == "2")
                    {
                        cout << "Enter the name of the file:  ";
                        cin >> filename;
                        if (!fileEditor.writeToFile(filename, false))
                            cout << "File not in current directory." << endl;
                        break;
                    }
                    cout << "Please enter a 1 or 2." << endl;
                }
                break;
            case 7:
                cout << "Enter the name of the file: ";
                cin >> filename;
                if (fileEditor.removeFile(filename))
                    cout << "File successfully deleted." << endl;
                else
                    cout << "File not found in current directory." << endl;
                break;
            case 8:
                cout << "Enter the name of the file: ";
                cin >> filename;
                switch(fileEditor.moveFile(filename))
                {
                    case 1:
                        cout << "No files in current directory." << endl;
                    case 2:
                        cout << "File not found in current directory." << endl;
                    case 3:
                        cout << "File successfully moved." << endl;
                }
                break;
            case 9:
                cout << "Enter the name of the file: ";
                cin >> filename;
                if (!fileEditor.printFileContent(filename))
                    cout << "File not in current directory." << endl;
                break;
            case 10:
                cout << "Enter the name of the file or folder to be found: ";
                cin >> filename;
                fileEditor.search(filename);
                cout << endl;
                break;
            case 11:
                cout << fileEditor.getCurrentPath();
                break;
            case 12:
                cout << "Goodbye :(" << endl;
                cout << "Your files and folders will be saved,\nbut you cannot use this program with" << endl;
                cout << "those files anymore." << endl;
                return 0;
                break;
            default:
                cout << "Please enter a number corresponding to an action above." << endl;
                break;
        }
        option = 0;
        cout << endl;
    }
    return 0;
}