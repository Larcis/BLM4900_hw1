#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "utilities.h"

int main(int argc, char **argv){
    struct info data;
    if(!get_args(argc, argv, &data, 'c'))
        return -1;

    printf("\nClient initialized with \n\tusername: %s \n\tpassword: %s \n\tport: %d \n\thostname: %s \n", 
            data.username, data.password, data.port, data.host_name);
    return 0;
}