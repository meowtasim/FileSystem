#include <iostream>
#include <string>
#include <math.h>
#include <cstring>
#include <ctime>
#define diskSpace 1024 // in terms of bytes
#define blockSize 1    // in terms of bytes
const int numberOfBlocks = diskSpace / blockSize;
using namespace std;

class allocationTableBlock // done
{
public:
    int used = 0;              // Default value indicating unoccupied
    char *blockPointer = NULL; // Pointer to actual memory of block
    ~allocationTableBlock()
    {
        delete[] blockPointer;
    }
};
// initialize the FAT with all 0s representing empty blocks
allocationTableBlock FAT[numberOfBlocks];
int stack[numberOfBlocks], top = numberOfBlocks - 1;

// Finds free blocks in constant time
class file;
class directory;
class file
{
public:
    // metedata
    // File name------
    string name = "default";
    // Metadata------
    string creationDate;
    int fileSize;
    string dataType;
    // permissions
    static bool readFile;
    static bool writeFile;
    static bool executeFile;
    static bool deleteFile;
    directory *locationDirectory; // Directory in which file exists
    // Index table------
    int *indexTable;
    int blocksUsed;

    // public: // Need to set default values
    // Operations allowed on files
    file(bool a = 0)
    { // 0 means public user and 1 means owner
        // set up permissions for public and owner
        cout << "Made file" << endl;
        // readFile = openFile = closeFile = 1;
        // writeFile = executeFile = a;
    }
    ~file()
    {
        delete[] indexTable;
        cout << "Destroyed file" << endl;
    }
};

bool file::readFile = true;
bool file::writeFile = false;
bool file::executeFile = true;
bool file::deleteFile = false;

class directory
{
public:
    // metedata
    string name;
    string creationDate;
    int directorySize = 0;
    directory *parentDirectory;
    // Tabel that points to all file objects, also point towards directory objects
    file *fileRecords[20]; // for a single level directory
    int numberOfFiles = 0;
    directory *directoryRecords[5];
    int numberOfDirectories = 0;
    // Operations allowed on directories
    directory(directory *obj = NULL) : parentDirectory(obj) {} // In case of making sub directories
    // ~directory()
    // {
    //     delete[] fileRecords;
    //     delete[] directoryRecords;
    // }
};

int findEmptyBlock() // constant time
{
    return stack[top--];
} // Returns the index of the first empty block

void takeFileMetadata(file &actualFile)
{
    string filename;
    cout << "Enter the name of the file : ";
    cin >> filename;
    actualFile.name = filename;
    time_t now = time(0);
    actualFile.creationDate = ctime(&now);
    // need to take file name and everything from user and time
}
void createFile(directory &d, const string &fileContent, file &actualFile)
{
    cout << "Starting file creation" << endl;
    cout << fileContent << " size of string is " << fileContent.size() << endl;
    int fileSize = fileContent.size(); // Calculating size of the file
    d.directorySize += fileSize;       // Update size of directory
    int blocksUsed = ceil(static_cast<double>(fileSize) / blockSize);
    actualFile.blocksUsed = blocksUsed;
    actualFile.indexTable = new int[blocksUsed]; // creates array of that many elements
    //----------------------------------------
    int pointer = 0, i;
    for (int k = 0; k < blocksUsed; k++)
    {
        cout << "Made 1 block " << blocksUsed << " huh" << endl;
        int blockNumber = findEmptyBlock(); // Finding empty block
        FAT[blockNumber].used = 1;
        actualFile.indexTable[k] = blockNumber; // Update index table with free block found
        FAT[blockNumber].blockPointer = new char[blockSize];
        // copying the string
        for (i = 0; i < blockSize; i++)
        {
            FAT[blockNumber].blockPointer[i] = fileContent[i + pointer];
        }
        pointer += i;
    }
    actualFile.fileSize = sizeof(fileContent); // updating metadata
    actualFile.locationDirectory = &d;
    d.fileRecords[d.numberOfFiles] = &actualFile; // Updating directory
    d.numberOfFiles++;

    takeFileMetadata(actualFile); // ask user meta data
    cout << "Finishing file creation" << endl;
}

// Function to remove file from directory array
void removeFileFromDirectory(file &a)
{
    int filePosition;
    for (filePosition = 0; filePosition < a.locationDirectory->numberOfFiles; filePosition++) // find file position in file list
    {
        if (a.name == a.locationDirectory->fileRecords[filePosition]->name)
        {
            break;
        }
    }
    for (int i = filePosition; i < a.locationDirectory->numberOfFiles; i++) // shifting remaining entries
    {
        a.locationDirectory->fileRecords[i] = a.locationDirectory->fileRecords[i + 1]; // shift by 1
    }
    a.locationDirectory->numberOfFiles--;
} // Succesfully removed file from directory

// createDirectory funtion-->In switch cases

// delete file
void deleteFile(file &a) // done
{
    removeFileFromDirectory(a);
    for (int i = 0; i < a.blocksUsed; i++)
    {
        FAT[a.indexTable[i]].used = 0; // Mark block as unused
        top++;
        stack[top] = a.indexTable[i];
        delete FAT[a.indexTable[i]].blockPointer; // free memory space allocated for block
        FAT[a.indexTable[i]].blockPointer = NULL;
    }
    a.~file(); // destroy the file object hopefully
}
// delete directory
directory *deleteDirectory(directory &d)
{
    directory *parentDirectory = d.parentDirectory;
    int i;
    while (d.numberOfDirectories != 0) // Delete all content of the directory
    {
        deleteDirectory(*d.directoryRecords[d.numberOfDirectories]);
        d.numberOfDirectories--;
    }
    while (d.numberOfFiles != 0)
    {
        deleteFile(*d.fileRecords[d.numberOfFiles]);
        d.numberOfFiles--;
    }
    // Delete entry of this directory in parent directory
    for (i = 0; i < parentDirectory->numberOfDirectories; i++)
    {
        if (parentDirectory->directoryRecords[i]->name == d.name)
        {
            break;
        }
    }
    for (i; i < parentDirectory->numberOfDirectories; i++)
    {
        parentDirectory->directoryRecords[i] = parentDirectory->directoryRecords[i + 1];
    }
    parentDirectory->numberOfDirectories--;
    delete &d;
    return parentDirectory;
}

// function to return pointer to file upon giving name
file *findFile(directory *currentDirectory, string fileName)
{
    for (int i = 0; i < currentDirectory->numberOfFiles; i++)
    {
        if (currentDirectory->fileRecords[i]->name == fileName)
        {
            return currentDirectory->fileRecords[i];
        }
    }
    return NULL;
}

directory *findDirectory(directory *currentDirectory, string directoryName)
{
    for (int i = 0; i < currentDirectory->numberOfDirectories; i++)
    {
        if (currentDirectory->directoryRecords[i]->name == directoryName)
        {
            return currentDirectory->directoryRecords[i];
        }
    }
    return NULL;
}

// rename
// moving
void moveFile(file &a, directory &d)
{ // move file to destination directory
    if (d.numberOfFiles == 20)
    { // if space is not available
        cout << "Error : Maximum capacity of directory reached" << endl;
        return;
    }
    d.fileRecords[d.numberOfFiles] = &a; // first add file to new directory
    d.numberOfFiles++;
    removeFileFromDirectory(a); // Remove file from previous directory
    a.locationDirectory = &d;   // Update file metadata
}
// copy (oit)
// displaying the whole file system--Main
// Periodic defragmentation?
// Need to handle space constraints
// Taking different types of files from os or something

void read_file(file *filename)
{
    for (int i = 0; i < filename->blocksUsed; i++)
    {
        for (int j = 0; j < blockSize; j++)
        {
            cout << FAT[filename->indexTable[i]].blockPointer[j];
        }
    }
    cout << endl;
}

int main()
{
    int owner = 0;
    for (int i = 0; i < numberOfBlocks; i++)
    {
        stack[i] = numberOfBlocks - i - 1;
    }
    directory mainDirectory;
    directory *currentDirectory = &mainDirectory;
    int choice;
    file *f1;
    string fcontent, fileName, fileName2;
    cout << "\n\n                   _______--------------------FILE SYSTEM--------------------_______                   " << endl;
    cout << "\n1.Create File\n"
         << "2.Create Directory\n"
         << "3.View File System\n"
         << "4.Delete File\n"
         << "5.Delete Directory\n"
         << "6.Rename File\n"
         << "7.Copy File\n"
         << "8.Move File\n"
         << "9.Modify File\n"
         << "10.Exit\n"
         << "11.Open directory\n" // changing the current directory
         << "12.Change owner\n"
         << "13.Read file\n"
         << endl;
    while (1)
    {

        // cin.ignore();
        cout << "Enter Input: ";
        cin >> choice;
        switch (choice)
        {
        case 1: // Create file - Works
        {
            if (owner == 1)
            {
                cout << "Enter file content: ";
                cin.ignore();
                getline(cin, fcontent);
                f1 = new file;
                // cout<<fcontent<<endl;
                createFile(*currentDirectory, fcontent, *f1);
            }
            else
            {
                cout << "Unauthorized Access" << endl;
            }

            break;
        }
        case 2: // Create Directory - Done
        {
            if (owner == 1)
            {
                if (currentDirectory->numberOfDirectories == 5)
                {
                    cout << "Error : Maximum number of subdirectories reached" << endl;
                }
                else
                {
                    directory *d1 = new directory(currentDirectory); // need to take parameters from user
                    cout << "Enter name of new directory  : ";
                    cin >> fileName;
                    d1->name = fileName;
                    currentDirectory->directoryRecords[currentDirectory->numberOfDirectories] = d1;
                    currentDirectory->numberOfDirectories++;
                }
            }
            else
            {
                cout << "Unauthorized Access" << endl;
            }
            break;
        }
        case 3: // Display - works
        {
            // cout << " : " << currentDirectory->fileRecords[0]->name << endl;
            if (currentDirectory->numberOfDirectories == 0 && currentDirectory->numberOfFiles == 0)
            {
                cout << "Current Directory is empty " << endl;
                break;
            }
            for (int i = 0; i < currentDirectory->numberOfDirectories; i++)
            {
                cout << i+1 << " : " << currentDirectory->directoryRecords[i]->name << endl;
            }
            for (int i = 0; i < currentDirectory->numberOfFiles; i++)
            {
                cout << i + currentDirectory->numberOfDirectories+1 << " : " << currentDirectory->fileRecords[i]->name << ".txt" << endl;
            }
            break;
        }

        case 4: // Delete file - works
            if (owner == 1)
            {
                cout << "Enter the name of the file you want to delete : ";
                cin >> fileName;
                f1 = findFile(currentDirectory, fileName);
                if (f1 == NULL)
                {
                    cout << "File does not exist in the current directory" << endl;
                    break;
                }
                deleteFile(*f1);
            }
            else
            {
                cout << "Unauthorized Access" << endl;
            }
            break;
        case 5: // Deletes a directory and all files within it - works
            if (owner == 1)
            {
                if (currentDirectory == &mainDirectory)
                {
                    cout << "Cannot delete the root directory, please open the directory you want to open" << endl;
                    break;
                }
                currentDirectory = deleteDirectory(*currentDirectory);
            }
            else
            {
                cout << "Unauthorized Access" << endl;
            }
            break;
        case 6: // Rename file - works
            if (owner == 1)
            {
                cout << "Enter the name of the file you want to rename : ";
                cin >> fileName;
                f1 = findFile(currentDirectory, fileName);
                if (f1 == NULL)
                {
                    cout << "File does not exist in the current directory" << endl;
                    break;
                }
                cout << "Enter the new name of the file : ";
                cin >> fileName2;
                f1->name = fileName2;
                cout << "Rename successful" << endl;
            }
            else
            {
                cout << "Unauthorized Access" << endl;
            }
            break;
        case 7: // copy file - hard - Mutasim
            break;
        case 8: // move file - doable
        {
            if (owner == 1)
            {
                string fname, destDirectory;
                cout << "Enter filename: ";
                cin >> fname;
                cout << "Enter directory name: ";
                cin >> destDirectory;
                file *f = findFile(currentDirectory, fname);
                directory *d = findDirectory(currentDirectory, destDirectory);
                moveFile(*f, *d);
                cout << "Moved file successfully" << endl;
            }

            else
            {
                cout << "Unauthorized Access" << endl;
            }
            break;
        }
        case 9: // modify file - idk bro we need to think
            break;
        case 10: // how dare someone exit, also how to even exit switch
            exit(1);
        case 11:
        { // Open directory - doable - Mutasim
            cout << "Enter the name of directory you want to open : ";
            cin >> fileName2;
            directory *d = findDirectory(currentDirectory, fileName2);
            currentDirectory = d;
            break;
        }
        case 12: // Need to be able to change access rights and give access according to it - Saana
        {
            bool ans;
            if (owner == 0)
            {
                cout << "Authorization Level -> Public " << endl;
            }
            else
            {
                cout << "Authorization Level -> Owner " << endl;
            }
            cout << "Do you want to change ownership? ";
            cin >> ans;
            if (ans == 1 && owner == 0)
            {
                owner = 1;
                f1->readFile = true;
                f1->writeFile = true;
                f1->executeFile = true;
                f1->deleteFile = true;
            }
            else if (ans == 1 && owner == 1)
            {
                owner = 0;
                f1->readFile = true;
                f1->writeFile = false;
                f1->executeFile = true;
                f1->deleteFile = false;
            }
            else
            {
                break;
            }
        }
        break;

        case 13: // Read file - doable to hard
            cout << "Enter the name of the file you want to read : ";
            cin >> fileName;
            f1 = findFile(currentDirectory, fileName);
            if (f1 == NULL)
            {
                cout << "File does not exist in the current directory" << endl;
                break;
            }
            else
            {
                read_file(f1);
            }
            break;
        }
    }
}
