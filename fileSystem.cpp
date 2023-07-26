#include <iostream>
#include <string>
#define diskSpace 1024 // in terms of mb
#define blockSize 1    // in terms of mb
const int numberOfBlocks = diskSpace / blockSize;
using namespace std;

class allocationTableBlock // done
{
public:
    int blockNumber = 0;
    char *blockPointer = NULL;
    ~allocationTableBlock(){
        delete[] blockPointer;
    }
};

allocationTableBlock FAT[numberOfBlocks];

class directory
{
private:
    // metedata
    string name;
    string creationDate;
    int numberOfFiles;
    int directorySize;

public:
    // Operations allowed on directories
};
class file
{
private:
    // metedata
    string name;
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
    int startingBlock;

public:
    // Operations allowed on files
    file(int a = 0)
    { // 0 means public user and 1 means owner
      // set up permissions for public and owner
    }
    friend void createFile(string fileContent, file &actualFile);
};
int findEmptyBlock() // done
{
    for (int i = 1; i < numberOfBlocks; i++)
    {
        if (FAT[i].blockNumber == 0)
        {
            return i;
        }
    }
} // Returns the index of the first empty block
void takeFileMetadata(file &actualFile){
    //need to take file name and everything from user
}
void createFile(string fileContent, file &actualFile)
{
    int fileSize = sizeof(fileContent);
    int pointer = 0;
    int startingBlock;
    while (fileSize > blockSize)
    {
        int blockNumber = findEmptyBlock();
        if (pointer = 0)
        {
            startingBlock = blockNumber;
        }
        FAT[blockNumber].blockPointer = new char[blockSize];
        // moving the string
        for (int i = 0; i < blockSize; i++)
        {
            FAT[blockNumber].blockPointer[i] = fileContent[i + pointer];
        }
        fileSize -= blockSize;
    }
    //Metadata of the file
    actualFile.startingBlock=startingBlock;
    actualFile.fileSize=sizeof(fileContent);
    takeFileMetadata(actualFile);
}

int main()
{
    // If an entry has value 0, that block is unused
    // initialize the FAT with all 0s representing empty blocks
    file f1;
    string sampleText="Hello thereee";
    createFile(sampleText,f1);
}
