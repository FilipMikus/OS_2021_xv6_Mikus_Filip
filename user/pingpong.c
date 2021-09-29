#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    //Check the number of arguments:
    if(argc != 1){
        fprintf(2, "usage: pingpong\n");
        exit(1);
    }

    char byte;

    //Declaration and initialization fds:
    int fds_parent_to_child[2];
    int fds_child_to_parent[2];
    //Create and control of two pipes for communication between parental and child process:
    if(pipe(fds_parent_to_child) != 0 || pipe(fds_child_to_parent) != 0){
        fprintf(2, "error: pingpong\n");
        exit(1);
    }

    //Bifurcation of the process:
    int pid = fork();

    //Parental process:
    if(pid != 0){
        //Write in the child process pipe:
        write(fds_parent_to_child[1], &byte, 1);
        //Read from the child process pipe:
        read(fds_child_to_parent[0], &byte, 1);
        fprintf(1, "%d: received pong\n", getpid());
        wait(0);
        exit(0);
        //Child process:
    } else {
        //Read from the parental process pipe:
        read(fds_parent_to_child[0], &byte, 1);
        fprintf(1, "%d: received ping\n", getpid());
        //Write in the parental process pipe:
        write(fds_child_to_parent[1], &byte, 1);

        exit(0);
    }
}