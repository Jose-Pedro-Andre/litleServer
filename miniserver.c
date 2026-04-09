#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
// #include <>

typedef struct client_t
{
    int id;
    char msg[100000];
} client_array;

int ft_create_socket(void)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        perror("Error in creation socket");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "Socket created sucessed\n");
    return sfd;
}

int ft_bind_and_listenPort(char *port)
{
    struct sockaddr_in servaddr;
    int sfd;

    sfd = ft_create_socket();
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(2130706433);
    servaddr.sin_port = htons(atoi(port));
    if (bind(sfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
    {
        perror("Error in bind fd");
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, 10) != 0)
    {
        perror("Error in listen");
        exit(EXIT_FAILURE);
    }
    return sfd;
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        fprintf(stderr, "arguments invalid\n");
        return 1;
    }
    int fd_server;
    client_array c_array[1024];
    fd_set set_read, set_write, master_set;

    memset(&master_set, 0, sizeof(master_set));
    char write_b[100000], read_b[100000];

    fd_server = ft_bind_and_listenPort(av[1]);
    int max_fd = fd_server;
    FD_ZERO(&master_set);
    FD_SET(fd_server, &master_set);
    while (1)
    {
        set_read = master_set;
        set_write = master_set;
        if (select(max_fd + 1, &set_read, &set_write, NULL, NULL) <= 0)
            continue;
    }
    
}