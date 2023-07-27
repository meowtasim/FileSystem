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
    bool readFile;
    bool writeFile;
    bool executeFile;
    bool openFile;
    bool closeFile;
    int renameFile;
    int deleteFile;
    directory *locationDirectory;
    // Index table------
    int *indexTable;
    int blocksUsed;

    // public: // Need to set default values
    // Operations allowed on files
    file(bool a = 0)
    { // 0 means public user and 1 means owner
        // set up permissions for public and owner
        cout << "Made file" << endl;
        readFile = openFile = closeFile = 1;
        writeFile = executeFile = a;
    }
    ~file()
    {
        delete[] indexTable;
        cout << "Destroyed file" << endl;
    }
};
class directory
{
public:
    // metedata
    string name;
    string creationDate;
    int directorySize;
    directory* parentDirectory;
    // Tabel that points to all file objects, also point towards directory objects
    file *fileRecords[20]; // for a single level directory
    int numberOfFiles = 0;
    directory *directoryRecords[5];
    int numberOfDirectories = 0;
    // Operations allowed on directories
    directory(directory *obj=NULL):parentDirectory(obj){}//In case of making sub directories
    // ~directory()
    // {
    //     delete[] fileRecords;
    //     delete[] directoryRecords;
    // }
};
int findEmptyBlock() // need to make it constant time
{
    for (int i = 0; i < numberOfBlocks; i++)
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
void createFile(directory &d, const string &fileContent, file &actualFile)
{
    cout << "Starting file creation" << endl;
    cout << fileContent << " size of string is " << fileContent.size() << endl;
    string a = "b";
    int fileSize = fileContent.size();
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
        a.locationDirectory->numberOfFiles--;
    }
} // Succesfully removed file from directory

// createDirectory funtion-->In switch cases

// delete file
void deleteFile(file &a) // done
{
    removeFileFromDirectory(a);
    for (int i = 0; i < a.blocksUsed; i++)
    {
        FAT[a.indexTable[i]].used = 0;            // Mark block as unused
        delete FAT[a.indexTable[i]].blockPointer; // free memory space allocated for block
        FAT[a.indexTable[i]].blockPointer = NULL;
    }
    a.~file(); // destroy the file object hopefully
}
// delete directory
directory* deleteDirectory(directory &d)
{
    directory* parentDirectory=d.parentDirectory;
    int i;
    while (d.numberOfDirectories != 0)//Delete all content of the directory
    {
        deleteDirectory(*d.directoryRecords[d.numberOfDirectories]);
        d.numberOfDirectories--;
    }
    while (d.numberOfFiles != 0)
    {
        deleteFile(*d.fileRecords[d.numberOfFiles]);
        d.numberOfFiles--;
    }
    //Delete entry of this directory in parent directory
    for(i=0;i<parentDirectory->numberOfDirectories;i++){
        if(parentDirectory->directoryRecords[i]->name==d.name){
            break;
        }
    }
    for(i;i<parentDirectory->numberOfDirectories;i++){
        parentDirectory->directoryRecords[i]=parentDirectory->directoryRecords[i+1];
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
// rename
// moving
void moveFile(file &a, directory &d)
{ // move file to destination directory
    if (d.numberOfFiles == 20)
    { // if space is not available
        cout << "Error : Maximum capacity of directory reached" << endl;
        return;
    }
    d.numberOfFiles++;
    d.fileRecords[d.numberOfFiles] = &a; // first add file to new directory
    removeFileFromDirectory(a);          // Remove file from previous directory
    a.locationDirectory = &d;            // Update file metadata
}
// copy (oit)
// displaying the whole file system--Main
// Periodic defragmentation?
// Need to handle space constraints
// Taking different types of files from os or something

int main()
{
    directory mainDirectory;
    directory *currentDirectory = &mainDirectory;
    int choice;
    file *f1;
    string fcontent, fileName,fileName2;
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
         << endl;
    while (1)
    {

        // cin.ignore();
        cout << "Enter Input: ";
        cin >> choice;
        switch (choice)
        {
        case 1://Create file - Works
        {
            cout << "Enter file content: ";
            cin.ignore();
            getline(cin, fcontent);
            f1 = new file;
            // cout<<fcontent<<endl;
            createFile(*currentDirectory, fcontent, *f1);

            break;
        }
        case 2:// Create Directory - Work in progress
        { 
            if (currentDirectory->numberOfDirectories == 5)
            {
                cout << "Error : Maximum number of subdirectories reached" << endl;
            }
            else
            {
                directory *d1 = new directory(currentDirectory); // need to take parameters from user
                currentDirectory->numberOfDirectories++;
                currentDirectory->directoryRecords[currentDirectory->numberOfDirectories] = d1;
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
                cout << i << " : " << currentDirectory->directoryRecords[i]->name << endl;
            }
            for (int i = 0; i < currentDirectory->numberOfFiles; i++)
            {
                cout << i + currentDirectory->numberOfDirectories << " : " << currentDirectory->fileRecords[i]->name << endl;
            }
            break;
        }
        case 4: //Delete file - works
            cout << "Enter the name of the file you want to delete : ";
            cin >> fileName;
            f1 = findFile(currentDirectory, fileName);
            if (f1 == NULL)
            {
                cout << "File does not exist in the current directory" << endl;
                break;
            }
            deleteFile(*f1);
            break;
        case 5://Deletes a directory and all files within it - works
            if(currentDirectory==&mainDirectory){
                cout<<"Cannot delete the root directory, please open the directory you want to open"<<endl;
            }
            currentDirectory=deleteDirectory(*currentDirectory);
            break;
        case 6://Rename file - works
            cout<<"Enter the name of the file you want to rename : ";
            cin>>fileName;
            f1=findFile(currentDirectory,fileName);
            if (f1 == NULL)
            {
                cout << "File does not exist in the current directory" << endl;
                break;
            }
            cout<<"Enter the new name of the file : ";
            cin>>fileName2;
            f1->name=fileName2;
            cout<<"Rename successful"<<endl;
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        }
    }
}
