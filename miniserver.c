#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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

int ft_bind_and_listenPort(int sfd, char *port)
{
    struct sockaddr_in servaddr;

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(2130706433);
    servaddr.sin_port = htons(atoi(port));
    if (bind(sfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
    {
        perror("Error in bind fd");
        exit(EXIT_FAILURE);
    }

    

}