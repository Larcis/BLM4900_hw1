#ifndef _SOCKET_HELPER_H_
#define _SOCKET_HELPER_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#define BUFFER_SIZE 1024
/**
 * when error happens, use this func. Prints out with printf then exits with code -1
 * param out: info message.
*/
void onerror(char *out);


/**
 * wrapper for creating TCP/IP compatible sockeet.
 * returns socket descriptor.
*/
int create_tcp_socket();


/**
 * Fills address struct with given hostname and port number.
 * returns filled structs pointer. needs to be freed after usage.
*/
struct sockaddr_in* create_sock_address(in_addr_t hostname, int port);


/**
 * Error check while trying to bind. For simple use.
*/
void bind_wrapper(int sockfd, struct sockaddr_in* addr);


/**
 * Error check while trying to listen. For simple use. 
*/
void listen_wrapper(int sockfd, int backlog);


/**
 * Error check while trying to gethostbyname. For simple use. 
*/
struct hostent* gethostbyname_wrapper(char * hostname);

void recv_easy(int sockfd, char * buffer);
void send_easy(int sockfd, char *message, int size);
#endif