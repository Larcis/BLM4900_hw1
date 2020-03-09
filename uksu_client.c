#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utilities.h"

int main(int argc, char **argv){
    struct info args;
    args.type = 'c';
    if(!get_args(argc, argv, &args))
        return -1;

    printf("\nClient initialized with \n\tusername: %s \n\tpassword: %s \n\tport: %d \n\thostname: %s \n", 
            args.username, args.password, args.port, args.host_name);
    return 0;
}