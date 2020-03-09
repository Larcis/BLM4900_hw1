#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>
#include <string.h>

struct info {
    char *username;
    char *password;
    char *host_name;
    int port;
    char type;
};

/**
 * param argc : argument count of the main function
 * param argv : argument vector of the main function
 * param data : struct info pointer to fill in
 * param type = 's' : for server, 'c': for client
*/
int get_args(int argc, char **argv, struct info *data);

#endif