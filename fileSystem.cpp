#include <iostream>
#include <string>
using namespace std;
typedef struct fileList
{
    file *currentFile;
    fileNodes *next;
} fileNodes; // A linked list used to keep track of all files in a directory
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
}