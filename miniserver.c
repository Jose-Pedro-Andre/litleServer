#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

typedef struct client_t
{
    int id;
    char msg[100000];
} client_array;

client_array c_array[1024];
fd_set set_read, set_write, master_set;
int max_clients = 0, iter = 0;

char buffer_send[1000000], buffer_recv[1000000];

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

int ft_bind_and_listenPort(char *port, char *host)
{
    struct sockaddr_in servaddr;
    int sfd;
    struct  in_addr addr;
    if (inet_pton(AF_INET, (const char *)host, &addr) != 1)
    {
        perror("Error in converting string host into network adress structure\n");
        exit(EXIT_FAILURE);
    }
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

void send_all_client(int current_client)
{
    for(int fd = 0; fd < max_clients; fd++)
    {
        if (fd != current_client && FD_ISSET(fd, &set_write))
        {
            send(fd, buffer_send, strlen(buffer_send), 0);
        }
    }
}

int accept_client(int sockfd)
{
    int new_client = accept(sockfd, NULL, NULL);
    if (new_client <= 0)
        return 0;
    if (new_client > max_clients)
        max_clients = new_client;
    FD_SET(new_client, &master_set);
    c_array[new_client].id = iter++;
    bzero(c_array[new_client].msg, sizeof(c_array[new_client].msg));
    sprintf(buffer_send, "server: client %d just arrive\n",  c_array[new_client].id);
    send_all_client(new_client);
    return 1;
}

void message_from_any_client(int client)
{
    int bytes = recv(client, buffer_recv, sizeof(buffer_recv), 0);
    if (bytes <= 0)
    {

        FD_CLR(client, &master_set);
        close(client);
        sprintf(buffer_send, "Server: client %d just left\n",  c_array[client].id);
        send_all_client(client);
        return ;
    }
    size_t j =  0;

    for (int i = 0; i < bytes; i++, j++)
    {
        c_array[client].msg[j] = buffer_recv[i];
        if (buffer_recv[i] == '\n')
        {
            c_array[client].msg[j] = 0;
            sprintf(buffer_send, "client %d: %s\n",c_array[client].id, c_array[client].msg);
            send_all_client(client);
            bzero(c_array[client].msg, sizeof(c_array[client].msg));
            j = 0;
        }
    }
}

int main(int ac, char **av)
{
    if (ac != 3)
    {
        fprintf(stderr, "arguments invalid\n");
        return 1;
    }
    int fd_server;

    fd_server = ft_bind_and_listenPort(av[2], av[1]);
    max_clients = fd_server;
    
    FD_ZERO(&master_set);
    FD_SET(fd_server, &master_set);
    while (1)
    {
        set_read = master_set;
        set_write = master_set;
        if (select(max_clients + 1, &set_read, &set_write, NULL, NULL) <= 0)
            continue;
        for(int fd = 0; fd <= max_clients; fd++)
        {
            if (fd == fd_server && FD_ISSET(fd, &set_read))
            {
                if (!accept_client(fd))
                    continue;
            }
            else if (FD_ISSET(fd, &set_read))
            {
                message_from_any_client(fd);
            }
        }
    }
    
}