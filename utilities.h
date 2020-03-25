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

/**
 * returns a char array pointer with specified size
 */
char * get_buffer(int size);

/**
 * frees the given pointer
*/
void clear_buffer(char* buffer);
/**
 * closes socket and frees the buffer
*/
void graceful_shutdown(int sockfd, char* buffer);

/**
 * customized input method that uses getchar and
 * gets input until new line character(enter key)
*/
void my_get_line(char * buff);

/**
 * replaces the specified chars with a new one in a string.
*/
void replace_chars(char* buff, char rep, char with);
#endif