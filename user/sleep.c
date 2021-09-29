#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    //Check the number of arguments:
    if(argc != 2){
        fprintf(2, "usage: sleep <TICKS>\n");
        exit(1);
    }

    //Convert an argument to a number of ticks:
    int ticks = atoi(argv[1]);

    //Check the number of ticks (parsing error):
    if(ticks == 0){
        fprintf(2, "usage: sleep <TICKS>\n");
        exit(1);
    }

    //System call sleep():
    sleep(ticks);
    exit(0);
}