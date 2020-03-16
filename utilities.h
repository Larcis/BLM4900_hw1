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
void get_args(int argc, char **argv, struct info *data);

char * get_buffer(int size);
void clear_buffer(char* buffer);
void graceful_shutdown(int sockfd, char* buffer);

void my_get_line(char * buff);
void replace_chars(char* buff, char rep, char with);
#endif