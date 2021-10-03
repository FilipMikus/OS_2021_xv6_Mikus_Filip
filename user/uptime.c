#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    //Check the number of arguments:
    if(argc != 1){
        fprintf(2, "usage: uptime\n");
        exit(1);
    }

    //Get number of ticks (system call uptime):
    int ticks = uptime();
    
    //Control of return value from system call uptime:
    if(ticks <= 0){
        fprintf(2, "error: uptime\n");
        exit(1);
    } 
    
    fprintf(1, "uptime: %d ticks from power-on\n", ticks);
    exit(0);
}
