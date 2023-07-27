#include <iostream>
#include <string>
#include <math.h>
#include <cstring>
#define diskSpace 1024 // in terms of bytes
#define blockSize 1    // in terms of bytes
const int numberOfBlocks = diskSpace / blockSize;
using namespace std;

class allocationTableBlock // done
{
public:
    int used = 0; // Default value indicating unoccupied
    char *blockPointer = NULL;
    ~allocationTableBlock()
    {
        delete[] blockPointer;
    }
};
// If an entry has value 0, that block is unused
// initialize the FAT with all 0s representing empty blocks

allocationTableBlock FAT[numberOfBlocks];
// Find free blocks in constant time
class file;
class directory
{
private:
    // metedata
    string name;
    string creationDate;
    int directorySize;
    // Tabel that points to all file objects, also point towards directory objects
    file *fileRecords = NULL; // for a single level directory
    int numberOfFiles = 0;
    directory *directoryRecords = NULL;
    int numberOfDirectories = 0;

public:
    // Operations allowed on directories
};
class file
{
private:
    // metedata
    // File name------
    string name;
    // Metadata------
    string creationDate;
    int fileSize;
    string dataType;
    // permissions
    int readFile;
    int writeFile;
    int executeFile;
    int openFile;
    int closeFile;
    int renameFile;
    int deleteFile;
    directory *locationDirectory;
    // Index table------
    int *indexTable;
    int blocksUsed;

public: // Need to set default values
    // Operations allowed on files
    file(int a = 0)
    { // 0 means public user and 1 means owner
        // set up permissions for public and owner
        cout << "Made file" << endl;
    }
    ~file()
    {
        delete[] indexTable;
        cout << "Destroyed file" << endl;
    }
    friend void deleteFile(file &a);
    friend void createFile(char fileContent[], file &actualFile);
};
int findEmptyBlock() // need to make it constant time
{
    for (int i = 1; i < numberOfBlocks; i++)
    {
        if (FAT[i].used == 0)
        {
            return i;
        }
    }
} // Returns the index of the first empty block
void takeFileMetadata(file &actualFile)
{
    // need to take file name and everything from user and time
} // |1| |2| |3| |4| |5|
void createFile(char fileContent[], file &actualFile) // Need to update directory
{
    cout << "Starting file creation" << endl;
    cout << fileContent << " size of string is " << sizeof(fileContent) << endl;
    string a = "b";
    int fileSize = strlen(fileContent);
    int blocksUsed = ceil(fileSize / blockSize);
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
    takeFileMetadata(actualFile);              // ask user meta data
    cout << "Finishing file creation" << endl;
}

//Function to remove file from directory array
void removeFileFromDirectory(file &a){

}

// createDirectory funtion
// delete file
void deleteFile(file &a)//done
{
    removeFileFromDirectory(a);
    for(int i=0;i<a.blocksUsed;i++){
        FAT[a.indexTable[i]].used=0;//Mark block as unused
        delete FAT[a.indexTable[i]].blockPointer;//free memory space allocated for block
        FAT[a.indexTable[i]].blockPointer=NULL;
    }
    a.~file();//destroy the file object hopefully
}
// delete directory
// rename
// moving
void moveFile(file &a,directory &d){//move file to destination directory
    
}
// copy (oit)
// displaying the whole file system
// Periodic defragmentation?
// Need to handle space constraints
// Taking different types of files from os or something
// Need to implement tree structure for directory

int main()
{ // Need to make user interface(Menu):
    // --Create file, directory, delete, rename,-- modify, moving, copy, taking file from os or something
    directory rootDirectory;
    file f1;
    char sampleText[] = "Hello thereee";
    string currentPath = "rootDirectory", displayFilesystem = "ls", makeNewFile = "new file", makeNewDirectory = "new directory", deleteObject = "delete";
    char commandLineInput[20];
    createFile(sampleText, f1);
    // cout << strlen(sampleText) << endl;
    while (1)
    {
        cout << "D:\\" << currentPath << ">"; // Display current directory like command prompt
        cin >> commandLineInput;              // Asking some command line function from user
        if (commandLineInput == displayFilesystem)
        {
            // Display all the directories and files of the currently open directory
        }
        else if (commandLineInput == makeNewFile)
        {
            // Need to call createFile
        }
        else if (commandLineInput == makeNewDirectory)
        {
            // Need to call createDirectory
        }
        else if (commandLineInput == deleteObject)
        {
            // Need to call delete function
        }
    }
}
