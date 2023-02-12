#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int arrgc, char **argv)
{
    srand(time(0)) ;
    int sockfd, newsockfd;
    int clilen;
    struct sockaddr_in cli_addr, serv_addr;
    int i;
    char buf[100];
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Cannot create socket\n");
        exit(0);
    }
    printf("****** Socket created *****\n\n Waiting for client .....\n");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Unable to bind local address\n");
        exit(0);
    }
    listen(sockfd, 5);
    while (1)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                           &clilen);

        if (newsockfd < 0)
        {
            perror("Accept error\n");
            exit(0);
        }
        for( int i = 0 ; i < 100 ; i++ )buf[i] = '\0' ;
        recv(newsockfd ,  buf , 100 , 0 ) ;
        if (strcmp(buf, "Send load") == 0)
        {
            int val = rand() % 100;
            printf("Load sent : %d\n", val);
            sprintf(buf, "%d", val);
            send(newsockfd ,buf , 100 , 0 ) ;
        }

        else if (strcmp(buf, "Send time") == 0)
        {
            time_t rawtime;
            struct tm *timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strcpy(buf, asctime(timeinfo));
            send(newsockfd ,buf ,100 , 0 ) ;
        }
    }
}