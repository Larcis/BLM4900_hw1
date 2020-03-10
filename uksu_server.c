#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<signal.h> 

#include "utilities.h"
#include "socket_helper.h"

int  sockfd;
char *buffer;
int sigint_recieved = 0;

void sigint_handler(int id){
    printf("sigint signal recieved....");
    sigint_recieved = 1;
    close(sockfd);
}

int server_auth_logic(int csock, char *username, char *password){
    recv_easy(csock, buffer);
    if(!strcmp(username, buffer)){
        send_easy(csock, "OK", 2);
        recv_easy(csock, buffer);
        if(!strcmp(password, buffer)){
            send_easy(csock, "OK", 2);
            return 1;
        } else {
            send_easy(csock, "FAIL", 4);
            printf("invalid password!\n");
            return 0;
        }
    } else {
        send_easy(csock, "FAIL", 4);
        printf("invalid username!\n");
        return 0;
    }

}
int main(int argc, char **argv) {

    //argument logic with get_opt
    struct info args;
    args.type = 's';
    get_args(argc, argv, &args);
    printf("\nServer initialized with \n\tusername: %s \n\tpassword: %s \n\tport: %d \n", args.username, args.password, args.port);
    
    //global buffer initialization
    buffer = get_buffer(BUFFER_SIZE);
    
    //server socket setup
    sockfd = create_tcp_socket();
    signal(SIGINT, sigint_handler);
    struct sockaddr_in  *addr =create_sock_address(INADDR_ANY, args.port);
    bind_wrapper(sockfd, addr);
    listen_wrapper(sockfd, 1);

    //variables for incoming connection
    int new_fd;    
    struct  sockaddr_in their_addr;
    socklen_t sin_size = sizeof(struct sockaddr_in);

    //main server loop
    while(!sigint_recieved){
        if((new_fd = accept(sockfd,(struct sockaddr*)&their_addr,&sin_size)) == -1){
            printf("error while accepting connection.\n");
        } else {
            printf("server got connection from %s\n", inet_ntoa(their_addr.sin_addr));
            if(server_auth_logic(new_fd, args.username, args.password)){
                while(!sigint_recieved){
                    recv_easy(new_fd, buffer);
                    send_easy(new_fd, buffer, strlen(buffer));
                }
            } else {
                printf("unsuccessful login attempt.\n");
                close(new_fd);
            }

        }
    }
    graceful_shutdown(sockfd, buffer);
    return 0;
}