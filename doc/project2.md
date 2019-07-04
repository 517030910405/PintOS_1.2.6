# Project 2 Design Document

> SJTU MS110  
> Spring 2019

## GROUP


- JiaSen Li <lijiasen0921@126.com>
- Erzhi Liu <654105282@qq.com>
- Boning Li <lbn187@sjtu.edu.cn>
- Enze Sun <sun_en_ze@126.com>

## PROJECT PARTS

### A. ARGUMENT PASSING  

#### ALGORITHMS

In the function *load*, we split the string file_name into some segments.

We add two extra parameters to the function *setup_stack*, that is *argc* and *argv*, with which we can put the arguments in the proper address.  Additionally, we have to put them in the stack inversely (the first arguments is in the lowest address).

There are many other details in the calling convention, such as the alignment of the address.

We limit the number of arguments to be less than 256.

### B. SYSTEM CALLS


####syscall in Pintos

    SYS_HALT,                   /* Halt the operating system. */
    SYS_EXIT,                   /* Terminate this process. */
    SYS_EXEC,                   /* Start another process. */
    SYS_WAIT,                   /* Wait for a child process to die. */
    SYS_CREATE,                 /* Create a file. */
    SYS_REMOVE,                 /* Delete a file. */
    SYS_OPEN,//                 /* Open a file. */
    SYS_FILESIZE,//             /* Obtain a file's size. */
    SYS_READ,                   /* Read from a file. */
    SYS_WRITE,                  /* Write to a file. */
    SYS_SEEK,//                 /* Change position in a file. */
    SYS_TELL,//                 /* Report current position in a file. */
    SYS_CLOSE,//                /* Close a file. */

    /* Project 3 and optionally project 4. */
    SYS_MMAP,                   /* Map a file into memory. */
    SYS_MUNMAP,                 /* Remove a memory mapping. */

    /* Project 4 only. */
    SYS_CHDIR,                  /* Change the current directory. */
    SYS_MKDIR,                  /* Create a directory. */
    SYS_READDIR,                /* Reads a directory entry. */
    SYS_ISDIR,                  /* Tests if a fd represents a directory. */
    SYS_INUMBER                 /* Returns the inode number for a fd. */
    
#### ALGORITHMS

**get the use data**

To get the user data,  we just read the data from the address *ESP* + 4 and above it, where *ESP* is stored in the *intr_frame*. And we check whether the address is valid.

