#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> 
#include <pthread.h>

#include "utilities.h"
#include "socket_helper.h"

int  sockfd; // servers socket 
char *buffer; // buffer for send and recv, memory allocated in main
int client_exit, //flag for checking if connected client exited
    client_cancelled; // flag for checking if client cancelled a command


void sigint_handler(int id){
    printf("sigint signal recieved....\n");
    graceful_shutdown(sockfd, buffer);
    exit(0);  
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

void* is_client_cancelled(void* args){
    int *csock = (int*)args;
    char buff[BUFFER_SIZE];
    //printf("cancel listener started.\n");
    while(!client_cancelled && !client_exit){
        recv_easy(*csock, buff);
        if(!strcmp(buff, "cancel")){
            client_cancelled = 1;
        }
    }
    printf("client cancelled a command.\n");
    return NULL;
}

void * busy_greeter(void * args){
    printf("server busy greeter thread started\n");
    int new_fd;    
    struct  sockaddr_in their_addr;
    socklen_t sin_size = sizeof(struct sockaddr_in);
    char * message = "\nHello! Server got a client before you. Please try again later....\n";
    //busy greeter loop, killed in main when its no longer needed
    while(1){
        if((new_fd = accept(sockfd,(struct sockaddr*)&their_addr,&sin_size)) == -1){
            printf("error while accepting connection.\n");
        } else {
            send_easy(new_fd, message, strlen(message));
            close(new_fd);
        }
    }
    return NULL;
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
    //signal(SIGPIPE, sigpipe_handler);
    struct sockaddr_in  *addr =create_sock_address(INADDR_ANY, args.port);
    bind_wrapper(sockfd, addr);
    listen_wrapper(sockfd, 1);

    //variables for incoming connection
    int new_fd;    
    struct  sockaddr_in their_addr;
    socklen_t sin_size = sizeof(struct sockaddr_in);

    //main server loop
    while(1){
        if((new_fd = accept(sockfd,(struct sockaddr*)&their_addr,&sin_size)) == -1){
            printf("error while accepting connection.\n");
        } else {
            send_easy(new_fd, "okay", 4);
            pthread_t busy_greeter_thread;
            if(pthread_create(&busy_greeter_thread, NULL, busy_greeter, NULL)) {
                onerror("Error creating thread\n");
            }
            printf("server got connection from %s\n", inet_ntoa(their_addr.sin_addr));
            if(server_auth_logic(new_fd, args.username, args.password)){
                printf("successful login: %s.\n", inet_ntoa(their_addr.sin_addr));
                client_exit = 0;
                while(!client_exit){
                    recv_easy(new_fd, buffer);
                    if(!strcmp(buffer, "exit")){   
                        client_exit = 1;
                        printf("%s exited.\n", inet_ntoa(their_addr.sin_addr));
                    } else {
                        FILE *fp;
                        int status;
                        fp = popen(strcat(buffer, " 2>&1"), "r");
                        if (fp == NULL){
                            printf("error in popen.");
                        } else {
                            client_cancelled = 0;
                            pthread_t is_client_cancelled_thread;
                            if(pthread_create(&is_client_cancelled_thread, NULL, is_client_cancelled, &new_fd)) {
                                onerror("Error creating thread\n");
                            }
                            while (!client_cancelled && fgets(buffer, BUFFER_SIZE, fp) != NULL){
                                send_easy(new_fd, buffer, strlen(buffer));
                            }
                            if(!client_cancelled){
                                pthread_cancel(is_client_cancelled_thread);
                            }
                            status = pclose(fp);
                            if (status == -1) {
                                printf("error while closing popen!\n");
                            }
                        }
                        if(!client_cancelled){
                            send_easy(new_fd, "finished", 8);
                        }
                    }
                }
            } else {
                printf("unsuccessful login attempt from %s.\n", inet_ntoa(their_addr.sin_addr));
                close(new_fd);
            }
            pthread_cancel(busy_greeter_thread);
        }
    }
    return 0;
}