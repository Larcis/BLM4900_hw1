#ifndef _UTILITIES_H
#define _UTILITIES_H

#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>
#include <string.h>

struct info {
    char *username;
    char *password;
    char *host_name;
    int port;
};

/**
 * param argc : argument count of the main function
 * param argv : argument vector of the main function
 * param data : struct info pointer to fill in
 * param type = 's' : for server, 'c': for client
*/
int get_args(int argc, char **argv, struct info *data, char type);

#endif