
/**
 * @file timeser.c
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
#include <time.h>
#define MAXLINE 1024 
  
int main() { 
    int sockfd; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Create socket file descriptor 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sockfd < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    servaddr.sin_family    = AF_INET; 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(8181); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    printf("\nServer Running....\n");
  
    int n; 
    socklen_t len;
    char buffer[MAXLINE]; 
 
    len = sizeof(cliaddr);
        // Receive a message from the client
        int recv_len = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len);

        // Get the current time
        time_t current_time = time(NULL);
        struct tm *time_info = localtime(&current_time);

        // Format the time as a string
        char time_str[20];
        strftime(time_str, 20, "%Y-%m-%d %H:%M:%S", time_info);

        // Send the time back to the client
        int sent_len = sendto(sockfd, time_str, strlen(time_str), 0, (const struct sockaddr *) &cliaddr, len);
        if (sent_len < 0) {
            perror("sendto failed");
            exit(EXIT_FAILURE);
        }
    close(sockfd);
    return 0; 
} 