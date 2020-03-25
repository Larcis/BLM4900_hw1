#include "socket_helper.h"


void onerror(char *out){
    printf("\n%s\n", out);
    exit(-1);
}

int create_tcp_socket(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1)
        onerror("Error while creating tcp socket.");
    
    return sockfd;
}

struct sockaddr_in* create_sock_address(in_addr_t hostname, int port){
    struct sockaddr_in * address = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    if(!address){
        onerror("malloc while create_sock_address\n");
    }
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    address->sin_addr.s_addr = hostname;
    memset(&(address->sin_zero), '\0', 8);
    return address;
}

void bind_wrapper(int sockfd, struct sockaddr_in* addr){
    int res = bind(sockfd, (struct sockaddr*) addr, sizeof (struct sockaddr));
    if(res == -1)
        onerror("Error while binding.");
}

void listen_wrapper(int sockfd, int backlog){
    if(listen(sockfd, backlog) == -1)
        onerror("Error while listening.");
}

struct hostent* gethostbyname_wrapper(char * hostname){
    struct hostent *he = gethostbyname(hostname);
    if(he == NULL)
        onerror("Error while getting host by name.");
    
    return he;
}

void recv_easy(int sockfd, char* buffer){
    int numbytes;
    if((numbytes = recv(sockfd, buffer, BUFFER_SIZE-1, 0)) == -1){
        onerror("recieve");
    }
    buffer[numbytes] = '\0';
    //printf("\nrecieved: %d \n", numbytes);
}
void send_easy(int sockfd, char *message, int size){
    int numbytes;
    if( (numbytes = send(sockfd, message, size, 0)) == -1){
        onerror("send");
    }
    //printf("\nsent: %d original_size: %d\n", numbytes, size);
}