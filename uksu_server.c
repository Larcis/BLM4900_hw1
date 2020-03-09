#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utilities.h"
#include "socket_helper.h"

int main(int argc, char **argv) {

    struct info args;
    args.type = 's';
    if(!get_args(argc, argv, &args))
        return -1;

    printf("\nServer initialized with \n\tusername: %s \n\tpassword: %s \n\tport: %d \n", args.username, args.password, args.port);
    int sockfd = create_tcp_socket();
    printf("%d ", sockfd);
    close(sockfd);
    
    return 0;
}