#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<signal.h> 

#include "utilities.h"
#include "socket_helper.h"

int sockfd;
char *buffer;
void sigint_handler(int id){
    printf("sigint signal recieved....");
    graceful_shutdown(sockfd, buffer);
    exit(0);
}

int client_auth_logic(char * username, char * password){

    send_easy(sockfd, username, strlen(username));
    recv_easy(sockfd, buffer);
    if(!strcmp(buffer, "OK")){
        send_easy(sockfd, password, strlen(password));
        recv_easy(sockfd, buffer);
        if(!strcmp(buffer, "OK")){
            printf("logined successfully.\n");
            return 1;
        } else {
            printf("invalid password!\n");
            return 0;
        }
    } else {
        printf("invalid username!\n");
        return 0;
    }
}

int main(int argc, char **argv){
    
    //argument logic with get_opt
    struct info args;
    args.type = 'c';
    get_args(argc, argv, &args);
    printf("\nClient initialized with \n\tusername: %s \n\tpassword: %s \n\tport: %d \n\thostname: %s \n", 
            args.username, args.password, args.port, args.host_name);

    //global buffer initialization
    buffer = get_buffer(BUFFER_SIZE);

    //client socket setup
    sockfd = create_tcp_socket();
    signal(SIGINT, sigint_handler);
    struct hostent *he;
    if((he = gethostbyname(args.host_name)) == NULL){
        onerror("gethostbyname");
    }
    struct  sockaddr_in their_addr;
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(args.port);
    their_addr.sin_addr = *((struct in_addr*)he->h_addr_list[0]);
    memset(&(their_addr.sin_zero), '\0', 8);
    if(connect(sockfd, (struct sockaddr*)&their_addr, sizeof(struct sockaddr)) == -1){
        onerror("connect");
    }

    if(client_auth_logic(args.username, args.password)){
        while(1){
            printf("(%s)>> ", args.username);
            scanf("%s", buffer);
            if(strcmp(buffer, "clear")){
                send_easy(sockfd, buffer, strlen(buffer));
                recv_easy(sockfd, buffer);
                printf("(%s)>> %s\n", args.username, buffer);
            } else {
                system("clear");
            }
        }
    } else {
        printf("unsuccessful login attempt.\n");
    }

    graceful_shutdown(sockfd, buffer);
    return 0;
}