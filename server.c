#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>

#define MAX 80
#define PORT 12040
#define PORT_2 22040

#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int connfd,int connfd_2)
{
    char buff[MAX];
    int n;
    int c=0;
    int j = 0;
    int i;
    int year_digit;
    char year[6];
    char date[MAX];
    char receive[30] = "Server received\n";
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
        write(connfd, receive, sizeof(receive));
        for(i =0; i< sizeof(buff)/sizeof(buff[0]);i++){
        	if(isspace(buff[i])){
        		c++;
        	}
        	if(c == 2){
        		date[j++] = buff[i];
        	}
        }
        printf("From client: %s\n", buff);
        bzero(buff, MAX);
        printf("Date : %s\n", date);
        for(i = 0;i<5;i++){
        	year[i] = date[i+4];
        }
        year_digit = atoi(year);
        //printf("%d\n",year_digit);
        year_digit -= 543;
        //printf("%d\n",year_digit);
		sprintf(buff, "%d", year_digit);
        write(connfd_2, buff, sizeof(buff));
        bzero(buff, MAX);
        read(connfd_2, buff, sizeof(buff));
        n = 0;

        if (strncmp("bye", buff, 4) == 0) {
            printf("bye\n");
            break;
        }
    }
}

// Driver function
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    int sockfd_2, connfd_2, len_2;
    struct sockaddr_in servaddr_2, cli_2;


    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockfd_2 = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1 || sockfd_2 == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else printf("Socket successfully created..\n");



    bzero(&servaddr, sizeof(servaddr));
    bzero(&servaddr_2, sizeof(servaddr_2));


    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    servaddr_2.sin_family = AF_INET;
    servaddr_2.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr_2.sin_port = htons(PORT_2);





    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0 || (bind(sockfd_2, (SA*)&servaddr_2, sizeof(servaddr_2))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else  printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0 || (listen(sockfd_2, 5)) != 0 ) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
    len_2 = sizeof(cli_2);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    connfd_2 = accept(sockfd_2, (SA*)&cli_2, &len_2);

    if (connfd < 0 || connfd_2 < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");


     // Function for chatting between client and server
    func(connfd, connfd_2);

    // After chatting close the socket
    close(sockfd);
    close(sockfd_2);
}
