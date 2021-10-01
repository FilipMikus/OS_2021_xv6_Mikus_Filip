#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
prime(int fd)
{
    //Declaration of variables:
    int prime_num, num;

    if(!read(fd,&prime_num, sizeof(prime_num)))
        return;
    
    fprintf(1, "prime %d\n", prime_num);

    int fds_parent_to_child[2];

    //Create and control of pipe for communication between parental and child process:
    if(pipe(fds_parent_to_child) != 0){
        fprintf(2, "error: primes\n");
        exit(1);
    }

    //Bifurcation of the process:
    int pid = fork();

    //Parental process:
    if(pid != 0){
        close(fds_parent_to_child[0]);
        //Read from left neighbor (parent):
        while(read(fd, &num, sizeof(num)))
            //PRIME FILTER:
            if(num % prime_num)
                //Write to right neighbor (child):
                write(fds_parent_to_child[1], &num, sizeof(num));
        close(fds_parent_to_child[1]);

        wait(0);
        //Child process:
    } else {
        close(fds_parent_to_child[1]);
        //Call a function that handles functionality (recursion):
        prime(fds_parent_to_child[0]);
        close(fds_parent_to_child[0]);

        exit(0);
    }
}

int
main(int argc, char *argv[])
{
    //Check the number of arguments:
    if(argc != 1){
        fprintf(2, "usage: primes\n");
        exit(1);
    }

    //Declaration and initialization fds:
    int fds_parent_to_child[2];

    //Create and control of pipe for communication between parental and child process:
    if(pipe(fds_parent_to_child) != 0){
        fprintf(2, "error: primes\n");
        exit(1);
    }

    //Bifurcation of the process:
    int pid = fork();

    //Parental process:
    if(pid != 0){
        close(fds_parent_to_child[0]);
        for(int i=2; i<=35; i++)
            write(fds_parent_to_child[1], &i, sizeof(i));
        close(fds_parent_to_child[1]);

        wait(0);
        exit(0);
        //Child process:
    } else {
        close(fds_parent_to_child[1]);
        //Call a function that handles functionality (recursion):
        prime(fds_parent_to_child[0]);
        close(fds_parent_to_child[0]);

        exit(0);
    }
}
