#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//Recursive function find():
void
find(char* dir, char* file)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    //Control of opening fd:
    if((fd = open(dir, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", dir);
        return;
    }

    //Initialization and control of file status:
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return;
    }

    //Check if it is a folder:
    if(st.type != T_DIR){
        fprintf(2, "find: %s is not a directory\n", dir);
        close(fd);
        return;
    }

    //Initialization of an auxiliary variable with a path:
    strcpy(buf, dir);
    p = buf+strlen(buf);
    *p++ = '/';

    //Gradual reading of files in a folder
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;

        if(!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if(stat(buf, &st) < 0){
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }

        switch(st.type){
            case T_FILE:
                if(!strcmp(de.name, file))
                    fprintf(1, "%s\n", buf);
                break;
            case T_DIR:
                //Rekurzivne vnaranie do podpriecinka:
                find(buf, file);
                break;
        }
    }
    close(fd);
}

int
main(int argc, char *argv[])
{
    //Check the number of arguments:
    if(argc != 3){
        fprintf(2, "usage: find <DIRECTORY NAME> <FILE NAME>\n");
        exit(1);
    }

    //Call a function that handles functionality (recursion):
    find(argv[1], argv[2]);
    exit(0);
}