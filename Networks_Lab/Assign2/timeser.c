// A Simple UDP Server that sends a HELLO message
/*
            NETWORK PROGRAMMING WITH SOCKETS

*** Vinod Meena 
*** 20CS10074
*** Network Lab : Assignment 2 
*** Problem 1 

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>
#define MAXLINE 1024

int main()
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;

    // Create socket file descriptor
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(8181);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("\nServer Running....\n");

    int n;
    socklen_t len;
    char buffer[MAXLINE];

    len = sizeof(cliaddr);
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0,
                 (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    printf("Message from client : %s\n", buffer);
    time_t rawtime;
	struct tm *timeinfo;
    time(&rawtime);
	timeinfo = localtime(&rawtime);
		// printf("Current local time and date: %s", asctime(timeinfo));

    // Send the time back to the client
    int sent_len = sendto(sockfd, asctime(timeinfo), strlen(asctime(timeinfo)), 0, (const struct sockaddr *)&cliaddr, len);
    if (sent_len < 0)
    {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }
    close(sockfd);
    return 0;
}