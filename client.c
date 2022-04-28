#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <ctype.h>

#define MAX 80
#define PORT 12040
#define PORT_2 22040

#define SA struct sockaddr
void func(int sockfd,int sockfd_2)
{
    char buff[MAX];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter name surename and data of birth : ");
        while ((buff[n++] = getchar()) != '\n');
        n = 0;
        buff[strcspn(buff, "\n")] = 0;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("%s",buff);

        bzero(buff, sizeof(buff));
        read(sockfd_2,buff,sizeof(buff));
        printf("%s\n",buff);
        bzero(buff, sizeof(buff));

        strncpy(buff, "bye", MAX);
        write(sockfd_2, buff, sizeof(buff));
        //printf("From Server : %s\n", buff);

        if ((strncmp(buff, "bye", 4)) == 0) {
            printf("bye\n");
            break;
        }
    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    int sockfd_2, connfd_2;
    struct sockaddr_in servaddr_2, cli_2;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockfd_2 = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1 || sockfd_2 == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
    bzero(&servaddr_2, sizeof(servaddr_2));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // assign IP, PORT
    servaddr_2.sin_family = AF_INET;
    servaddr_2.sin_addr.s_addr = inet_addr("127.0.0.2");
    servaddr_2.sin_port = htons(PORT_2);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0 || connect(sockfd_2, (SA*)&servaddr_2, sizeof(servaddr_2)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else printf("connected to the server..\n");


    func(sockfd,sockfd_2);
    close(sockfd);
    close(sockfd_2);


}
