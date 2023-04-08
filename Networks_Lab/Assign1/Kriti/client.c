// NETWORKS LAB (Assignent: 01, Qn: 02, CLIENT PROCESS)
// Name: Kriti Bhardwaj
// Roll No: 20CS30028



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CHUNK_SIZE 10

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;

    int i;
    char buf[100];

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Unable to create socket\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;
    inet_aton("127.0.0.1", &serv_addr.sin_addr);
    serv_addr.sin_port = htons(20000);

    if((connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0)
    {
        perror("Unable to connect to server\n");
        exit(0);
    }

    printf("\nConnected to server\n");

    char c=' ';
    char inpt_buf[CHUNK_SIZE];
    for(i=0; i<CHUNK_SIZE; i++) inpt_buf[i] = '\0';
    i=0;
    int iteration=0;
    printf("Enter expression: ");
    while(1)
    {
        scanf("%c", &inpt_buf[i]);
        c = inpt_buf[i];

        if(inpt_buf[i]=='\n')
        {
            if(iteration==0 && i==2)
            {
                if(inpt_buf[0]=='-' && inpt_buf[1]=='1')
                {
                    printf("Terminated\n\n");
                    break;
                }
            }

            send(sockfd, inpt_buf, strlen(inpt_buf), 0);

            for(i=0; i<100; i++) buf[i] = '\0';
            recv(sockfd, buf, 100, 0);
            printf("Ans received by the server: %s\n", buf);
            for(i=0; i<100; i++) buf[i] = '\0';


            for(i=0; i<CHUNK_SIZE; i++) inpt_buf[i] = '\0';
            printf("\nEnter expression: ");
            iteration=0;
            i=0;
        }
        else if(i == (CHUNK_SIZE-1))
        {
            iteration++;
            send(sockfd, inpt_buf, strlen(inpt_buf), 0);
            for(i=0; i<CHUNK_SIZE; i++) inpt_buf[i] = '\0';
            i=0;
        } 
        else i++;
    }

    close(sockfd);
    return 0;
}