#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utilities.h"


int main(int argc, char **argv) {

    struct info data;
    if(!get_args(argc, argv, &data, 's'))
        return -1;

    printf("\nServer initialized with \n\tusername: %s \n\tpassword: %s \n\tport: %d \n", data.username, data.password, data.port);
    return 0;
}