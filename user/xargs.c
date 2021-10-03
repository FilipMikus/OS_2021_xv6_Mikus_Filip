#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXLEN 512

int
main(int argc, char *argv[])
{
    //Check the number of arguments:
    if(argc < 2){
        fprintf(2, "usage: xargs <command> [params, ...]\n");
        exit(1);
    }

    //Array of program arguments:
    char* cmd[MAXARG];
    for(int i = 1; i<argc; i++){
        cmd[i-1] = argv[i];
    }

    //Variables declaration:
    char buf[MAXLEN];
    char c;
    int pid;
    int j = 0;

    //Reading characters from STDIN(0):
    while(read(0, &c, sizeof(c))){
        //Detection of end-line!
        if(c == '\n'){

            //Bifurcation of the process:
            pid = fork();
            //Parental process:
            if(pid != 0){
                wait(0);
                //Child process:
            } else {
                //Add end character to buffer
                buf[j] = '\0';
                cmd[argc-1] = buf;
                //xargs program (argv[1]) execution:
                exec(argv[1], cmd);
            }
            j = 0;
        } else {
            //Buffer overflow detection:
            if(j == MAXLEN){
                fprintf(2, "error: xargs\n");
                exit(1);
            }
            //Fill buffer:
            buf[j] = c;
            j++;
        }
    }
    exit(0);
}
