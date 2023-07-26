# FileSystem
## C++ Project 
2 File System Simulation
Objective: Design and implement a simple file system simulator to understand how file systems
manage data storage.
Specifications:
- Files & Directories: Your system should be able to create files and directories. Each file and
directory should have a name, creation timestamp, and owner. Files should also have a size and
contain data (simple text for this project).
- Hierarchical Structure: Files should be organized in a hierarchical directory structure. This could be
represented by a tree data structure, with each node being a file or a directory.
- Operations: Implement basic file operations, such as open, close, read, write, delete, rename, and
move. Directories should support operations like create, delete, rename, and list contents.
- Permissions: Implement a basic permission system. Each file and directory should have read,
write, and execute permissions, and these permissions could be different for the file/directory owner
and others.
- Persistence (optional): As an extra challenge, you could save your file system's state to a file and
reload it when the program starts. This would simulate the persistence of a real file system.
