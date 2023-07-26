#include <iostream>
#include <string>
#define diskSpace 1024//in terms of mb
#define blockSize 1//in terms of mb
using namespace std;
typedef struct fileList
{
    file *currentFile;
    fileNodes *next;
} fileNodes; // A linked list used to keep track of all files in a directory
// typedef struct linkedlist{
//     int pageNumber;
//     linkedList* next;
// }linkedList;//For file allocation table
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
class file : directory
{
private:
    // metedata
    string name;
    string creationDate;
    int fileSize;
    string dataType;
    //permissions
    int readFile;
    int writeFile;
    int executeFile;
    int openFile;
    int closeFile;
    int renameFile;
    int deleteFile;
public:
    // Operations allowed on files
    file(int a=0){//0 means public user and 1 means owner
        //set up permissions for public and owner
    }
};
int main()
{
    int numberOfBlocks=diskSpace/blockSize;
    int FAT[numberOfBlocks];//If an entry has value 0, that block is unused
    //initialize the FAT with all 0s representing empty blocks
    for(int i=0;i<numberOfBlocks;i++){
        FAT[i]=0;
    }
}
