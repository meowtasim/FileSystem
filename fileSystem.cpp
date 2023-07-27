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
class directory
{
private:
    // metedata
    string name;
    string creationDate;
    int directorySize;
    // Tabel that points to all file objects, also point towards directory objects
    file *fileRecords; // for a single level directory
    int numberOfFiles = 0;
    directory *directoryRecords;
    int numberOfDirectories = 0;

public:
    // Operations allowed on directories
    directory()
    {
        fileRecords = new file[20];
        directoryRecords = new directory[5];
    }
    friend void removeFileFromDirectory(file &a);
    friend void moveFile(file &a, directory &d);
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
    friend void removeFileFromDirectory(file &a);
    friend void moveFile(file &a, directory &d);
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

// Function to remove file from directory array
void removeFileFromDirectory(file &a)
{
    int filePosition;
    for (int filePosition = 0; filePosition < a.blocksUsed; filePosition++)// find file position in file list
    {
        if (a.name == a.locationDirectory->fileRecords[filePosition].name)
        { 
            break;
        }
    } 
    for (int i = filePosition; i < a.locationDirectory->numberOfFiles; i++)// shifting remaining entries
    {
        a.locationDirectory->fileRecords[i] = a.locationDirectory->fileRecords[i + 1]; // shift by 1
        a.locationDirectory->numberOfFiles--;
    }
} // Succesfully removed file from directory

// createDirectory funtion
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
    d.fileRecords[d.numberOfFiles] = a; // first add file to new directory
    removeFileFromDirectory(a);         // Remove file from previous directory
    a.locationDirectory = &d;           // Update file metadata
}
// copy (oit)
// displaying the whole file system
// Periodic defragmentation?
// Need to handle space constraints
// Taking different types of files from os or something


int main(){
    int choice;
    char fcontent[100];
	cout<<"\n\n                   _______--------------------FILE SYSTEM--------------------_______                   "<<endl;
    cout<<"\n1.Create File\n"<<"2.Create Directory\n"<<"3.View File System\n"<<"4.Delete File\n"<<"5.Delete Directory\n"<<"6.Rename File\n"<<"7.Copy File\n"<<"8.Move File\n"<<"9.Modify File\n"<<"10.Exit\n"<<endl;
	cout<<"Enter Input: ";
    cin>>choice;

    while(1){
        switch(choice) {
            case 1: 
                cout<<"Enter file content: ";
                // cin.get(fcontent,100);
                getline(cin,fcontent, "\n");
                cout<<fcontent<<endl;

                break;
            case 2: 
                break;
            case 3: 
                break;
            case 4: 
                break;
            case 5: 
                break;
            case 6: 
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
