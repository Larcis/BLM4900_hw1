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

/**
 * SIGINT handler for client.
 * if client started a command
 * before SIGINT then this fucntion
 * sends server a cancellation request
 * if that not the case then it closes
 * client process with releasing used
 * resources.
*/
void sigint_handler(int id){
    printf("sigint signal recieved....\n");
    if(can_cancel){
        send_easy(sockfd, "cancel", 6);
        can_cancel = 0;
        printf("you cancelled command.\n");
    } else {
        send_easy(sockfd, "exit", 4);
        graceful_shutdown(sockfd, buffer);
        exit(0);
    }
}

/**
 * simple authentication flow, has a matching 
 * function in the server code.
*/
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
    //subscribing to SIGINT signal with the custom handler function
    signal(SIGINT, sigint_handler);
    //hostname to ip code
    struct hostent *he;
    if((he = gethostbyname(args.host_name)) == NULL){
        onerror("gethostbyname");
    }
    struct  sockaddr_in their_addr;
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(args.port);
    their_addr.sin_addr = *((struct in_addr*)he->h_addr_list[0]);
    memset(&(their_addr.sin_zero), '\0', 8);
    //connect request to the specified server.
    if(connect(sockfd, (struct sockaddr*)&their_addr, sizeof(struct sockaddr)) == -1){
        onerror("connect");
    }
    /*
        first value received from server that
        corresponds if server is busy or not.
        if server is not busy then the main process
        sends an "okay" string else the greeter thread
        sends an informing message.
    */
    recv_easy(sockfd, buffer);
    if(strcmp(buffer, "okay")){
        //if server is busy then write the message and exit
        printf("%s", buffer);
    } else if(client_auth_logic(args.username, args.password)){
        short client_exit = 0;
        //while user did not write exit as a command or sent a SIGINT
        while(!client_exit){
            printf("(%s@%s):>> ", args.username, args.username);
            //custom input function, read untill \n(enter key) and deletes \n at the end
            my_get_line(buffer);
            //if command is empty , request new command
            if(strlen(buffer) == 0 ) continue;
            //if the command is clear then clear the terminal 
            //that client process is working
            if(!strcmp(buffer, "clear")){
                int res = system("clear");
                (void)res;
            //if command is exit then send an exit request to the server
            //and close the client process with releasing the resources
            } else if(!strcmp(buffer, "exit")) {
                send_easy(sockfd, "exit", 4);
                client_exit = 1;
            //any command that can work in the terminal
            } else {
                send_easy(sockfd, buffer, strlen(buffer));
                short finished = 0;
                //just started a command,
                //give user 1 cancellation right
                can_cancel = 1;
                do{
                    recv_easy(sockfd, buffer);
                    if(strcmp(buffer, "finished")){
                        printf("%s", buffer);
                    } else {   
                        finished = 1;
                    }
                //receive and pring until the server
                //send "finish" flalg or user cancel the command
                }while(!finished);
                //if command does not cancelled by user and finished
                //already then take cancellation right back.
                if(can_cancel){
                    can_cancel = 0;
                } else {
                    int res = system("clear");
                    (void)res;
                }
            }
        }
    } else {
        printf("unsuccessful login attempt.\n");
    }
    //release resources
    graceful_shutdown(sockfd, buffer);
    return 0;
}