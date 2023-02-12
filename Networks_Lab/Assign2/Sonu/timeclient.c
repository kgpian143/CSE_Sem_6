// A Simple Client Implementation
/**
 * @file timeclient.c
 * @author Sonu Yadav 
 * @brief  20CS10061
 * @version 0.1
 * @date 2023-01-20
 * @category Network Assignment 2 : Problem 1
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <time.h>
int main()
{
    int sockfd;
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8181);
    inet_aton("127.0.0.1", &servaddr.sin_addr);

    int n;
    socklen_t len;
    char *hello = "Send the local time : ";
    int flag = 0;
    for (int i = 0; i < 5; i++)
    {
        int sent_len = sendto(sockfd, "Send the current time", 100, 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        if (sent_len < 0)
        {
            perror("sendto failed");
            exit(EXIT_FAILURE);
        }

        // Wait for a response using poll()
        struct pollfd fds;
        fds.fd = sockfd;
        fds.events = POLLIN;
        int res = poll(&fds, 1, 3000);

        // Check the result of poll()
        if (res == 0)
        {
            printf("Request send again\n");
        }
        else
        {
            // Data received
            char buffer[100];
            socklen_t addr_len = sizeof(servaddr);
            recvfrom(sockfd, buffer, 100, 0, (struct sockaddr *)&servaddr, &addr_len);
            printf("Current time: %s\n", buffer);
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        printf("Timeout\n");
    }
    close(sockfd);
    return 0;
}