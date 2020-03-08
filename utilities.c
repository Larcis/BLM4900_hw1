#include "utilities.h"

extern char *optarg;
extern int optind, opterr, optopt;

int get_args(int argc, char **argv, struct info *data, char type) {
    int c;
    opterr = 0;
    short is_port_set = 0;
    data->host_name = data->password = data->username = "";
    data->port = -1;
    while ((c = getopt(argc, argv, "u:p:h:")) != -1) {
        if (c == 'p' && !is_port_set) {
            is_port_set = 1;
            data->port = atoi(optarg);
        } else if (c == 'p') {
            data->password = optarg;
        } else if (c == 'u') {
            data->username = optarg;
        } else if (c == '?') {
            fprintf(stderr, "Unknown option `-%c'. Ignoring...\n", optopt);
        } else if (type == 'c' && c == 'h') {
            data->host_name = optarg;
        }
    }
    char usage[256],
        *note = "\n\n\tNote: Port must be before password and all are required.\n\n";
    sprintf(usage, "\n\n\tUsage:\n\t\t%s -p port(int) -p password(string) -u username(string)", argv[0]);
    if (type == 'c')
        strcat(usage, " -h hostname(string)");
    strcat(usage, note);

    if (!strcmp(data->username, "") || !strcmp(data->password, "") ||  data->port == -1 || (type == 'c' && !strcmp(data->host_name, ""))) {
        printf("%s", usage);
        return 0;
    }
    return 1;
    
}