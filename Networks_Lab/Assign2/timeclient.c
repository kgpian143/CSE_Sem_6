// A Simple Client Implementation
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
#include <poll.h>
int main() { 
    int sockfd; 
    struct sockaddr_in servaddr; 
    int cnt = 0 ;
    // Creating socket file descriptor 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ( sockfd < 0 ) { 
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

      
    // sendto(sockfd, (const char *)hello, strlen(hello), 0, 
	// 		(const struct sockaddr *) &servaddr, sizeof(servaddr)); 
    // printf("Hello message sent from client\n"); 
    char buffer[100] ;
    while (1) {
    // Send a request for the time
    int sent_len = sendto(sockfd, "Send the current time", 100, 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    if (sent_len < 0) {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }

    // Wait for a response using poll()
    struct pollfd fds;
    fds.fd = sockfd;
    fds.events = POLLIN;
    int res = poll(&fds, 1, 3000);

    // Check the result of poll()
    if (res == 0) {
        // Timeout exceeded
        cnt++;
        printf("Request send again\n") ;
        if (cnt >= 5) {
            printf("Timeout exceeded\n");
            exit(EXIT_FAILURE);
        }
    } else if (res < 0) {
        // Error
        perror("poll failed");
        exit(EXIT_FAILURE);
    } else {
        // Data received
        socklen_t addr_len = sizeof(servaddr);
        int recv_len = recvfrom(sockfd, buffer, 100, 0, (struct sockaddr *) &servaddr, &addr_len);
        if (recv_len < 0) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }
        buffer[recv_len] = '\0';
        printf("Current time: %s\n", buffer);
        break;
    }
}     
    close(sockfd); 
    return 0; 
} 