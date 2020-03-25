#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<signal.h> 

#include "utilities.h"
#include "socket_helper.h"

int sockfd;
char *buffer;
int can_cancel;

void sigint_handler(int id){
    printf("sigint signal recieved....\n");
    if(can_cancel){
        send_easy(sockfd, "cancel", 6);
        can_cancel = 0;
        int res = system("clear");
        (void)res;
        printf("you cancelled command.\n");
    } else {
        send_easy(sockfd, "exit", 4);
        graceful_shutdown(sockfd, buffer);
        exit(0);
    }
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
    recv_easy(sockfd, buffer);
    if(strcmp(buffer, "okay")){
        printf("%s", buffer);
    } else if(client_auth_logic(args.username, args.password)){
        short client_exit = 0;
        while(!client_exit){
            printf("(%s)>> ", args.username);
            my_get_line(buffer);
            if(strlen(buffer) == 0 ) continue;
            if(!strcmp(buffer, "clear")){
                int res = system("clear");
                (void)res;
            } else if(!strcmp(buffer, "exit")) {
                send_easy(sockfd, "exit", 4);
                client_exit = 1;
            } else {
                send_easy(sockfd, buffer, strlen(buffer));
                short finished = 0;
                can_cancel = 1;
                do{
                    recv_easy(sockfd, buffer);
                    if(strcmp(buffer, "finished")){
                        printf("%s", buffer);
                    } else {   
                        finished = 1;
                    }
                }while(!finished && can_cancel);
                if(can_cancel){
                    can_cancel = 0;
                }
            }
        }
    } else {
        printf("unsuccessful login attempt.\n");
    }
    graceful_shutdown(sockfd, buffer);
    return 0;
}