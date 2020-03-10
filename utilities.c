#include "utilities.h"

extern char *optarg;
extern int optind, opterr, optopt;

char *get_buffer(int size){
    char * buffer = (char*)calloc(sizeof(char), size);
    if(buffer == NULL){
        printf("memory allocation error!");
        exit(-1);
    }
    return buffer;
}

void clear_buffer(char* buffer){
    if(buffer != NULL)
        free(buffer);
}

void graceful_shutdown(int sockfd, char* buffer){
    close(sockfd);
    clear_buffer(buffer);
}

void get_args(int argc, char **argv, struct info *data) {
    int c;
    opterr = 0;
    short is_port_set, is_password_set, is_username_set, is_hostname_set;
    is_port_set = is_password_set = is_username_set = is_hostname_set = 0;
    if(data->type == 's')
        is_hostname_set = 1;
    data->port = 6666;
    data->username = "admin";
    data->password = "password";
    data->host_name = "localhost";
    while ((c = getopt(argc, argv, "u:p:h:")) != -1) {
        if (c == 'p' && !is_port_set) {
            is_port_set = 1;
            data->port = atoi(optarg);
        } else if (c == 'p' && !is_password_set) {
            is_password_set = 1;
            data->password = optarg;
        } else if (c == 'u' && !is_username_set) {
            is_username_set = 1;
            data->username = optarg;
        } else if (c == '?') {
            fprintf(stderr, "Unknown option `-%c'. Ignoring...\n", optopt);
        } else if (data->type == 'c' && c == 'h' && !is_hostname_set) {
            is_hostname_set = 1;
            data->host_name = optarg;
        }
    }
    char usage[256],
        *note = "\n\n\tNote: Port must be before password and all are required.\n\n";
    sprintf(usage, "\n\n\tUsage:\n\t\t%s -p port(int) -p password(string) -u username(string)", argv[0]);
    if (data->type == 'c')
        strcat(usage, " -h hostname(string)");
    strcat(usage, note);

    if (!is_username_set || !is_password_set ||  !is_port_set || !is_hostname_set) {
        printf("%s", usage);
        exit(-1);
    }
}